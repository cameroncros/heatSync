/*
 * Main.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#include "Main.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

#include "Database.h"
#include "Sqlite.h"

//TODO: not needed
#include "File.h"
#include "Share.h"
#include "Avahi.h"
#include "SecureConnection.h"


/*
 * Necessary Threads:
 * 	1 per share for watching filesystem
 * 	1 per share for actual syncing
 * 	  - must consult each available connection
 * 	  for most up to date changes
 *  1 thread for database
 *  1 thread for discovery
 */

Main::Main() {
	database = NULL;
	std::cout << "Welcome to HeatSync." << std::endl;
	settingsDir = getenv("HOME");
	settingsDir.append("/.heatSync/");
	settingsFile = settingsDir;
	settingsFile.append("config");
	createSettingsDir();

	readSettings();



	database = new Sqlite();
	database->getShares(shares);
	//TODO: temp shit

	/*Network *net;
	net = new Network();
	delete(net);*/

	pause();
}

Main::~Main() {
	delete(database);
	std::cout << "Good Bye" << std::endl;
	// TODO Auto-generated destructor stub
}

void Main::createSettingsDir() {
	struct stat tmp;
	if (stat(settingsDir.c_str(), &tmp) != 0) {
		if (mkdir(settingsDir.c_str(), 0755) != 0) {
			std::cerr << "Can't make " << settingsDir << " directory" << std::endl;
			exit(1);
		}
	}
	if (S_ISDIR(tmp.st_mode) == 0) {
		if (remove(settingsDir.c_str()) != 0) {
			std::cerr << "Can't delete " << settingsDir << " file" << std::endl;
			exit(1);
		}
		if (mkdir(settingsDir.c_str(), 0755) != 0) {
			std::cerr << "Can't make " << settingsDir << " directory" << std::endl;
			exit(1);
		}
	}
}

void Main::readSettings() {
	struct stat tmp;

	if (stat(settingsFile.c_str(), &tmp) == 0 && S_ISREG(tmp.st_mode)) {
		std::cout << "Reading Settings" << std::endl;
		std::ifstream file;
		file.open(settingsFile, std::ios::in);
		std::string name, value;
		while (file.is_open() && !file.eof()) {
			std::getline(file, name, '=');
			std::getline(file, value, '\n');
			//std::cout << name << "/" << value << std::endl;
			settings[name]=value;
		}
	}
}



int main(int argc, char **argv) {
	Main *mn = new Main();
	delete(mn);
	return 0;
}

