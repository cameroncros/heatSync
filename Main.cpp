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
#include "Network.h"


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
	// TODO Auto-generated constructor stub
	struct stat tmp;
	if (stat(".heatSync", &tmp) != 0) {
		if (mkdir(".heatSync", 0755) != 0) {
			std::cerr << "Can't make .heatSync directory" << std::endl;
			exit(1);
		}
	}
	if (S_ISDIR(tmp.st_mode) == 0) {
		if (remove(".heatSync") != 0) {
			std::cerr << "Can't delete .heatSync file" << std::endl;
			exit(1);
		}
		if (mkdir(".heatSync", 0755) != 0) {
			std::cerr << "Can't make .heatSync directory" << std::endl;
			exit(1);
		}
	}
	if (stat(".heatSync/config", &tmp) == 0 && S_ISREG(tmp.st_mode)) {
		readSettings(".heatSync/config");
	}

	database = NULL;
	std::cout << "Welcome to HeatSync." << std::endl;
	database = new Sqlite();

	//TODO: temp shit

	network = new Network();
	free(network);
	//shares.push_back(Share(*database));

	pause();
}

Main::~Main() {
	delete(database);
	std::cout << "Good Bye" << std::endl;
	// TODO Auto-generated destructor stub
}

void Main::readSettings(std::string settingsFile) {
	std::cout << "Reading Settings" << std::endl;
	std::ifstream file;
	file.open(settingsFile.c_str(), std::ios::in);
	std::string name, value;
	while (file.is_open() && !file.eof()) {
		std::getline(file, name, '=');
		std::getline(file, value, '\n');
		std::cout << name << "/" << value << std::endl;
		settings[name]=value;
	}
}



int main(int argc, char **argv) {
	Main *mn = new Main();
	delete(mn);
	return 0;
}

