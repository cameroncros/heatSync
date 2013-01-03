/*
 * Main.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#include "Main.h"
#include <string>
#include <iostream>
#include <sys/stat.h>

#include "Database.h"
#include "Sqlite.h"

//TODO: not needed
#include "File.h"
#include "Share.h"


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
	int out;
	readSettings(".heatSync/config");
	out = mkdir(".heatSync", 0755); //todo: add check for preexisting directory
	if (out != 0) {
		std::cerr << "Cant make .heatSync directory" << std::endl;
	}

	database = NULL;
	std::cout << "Welcome to HeatSync." << std::endl;
	database = new Sqlite();

	//TODO: temp shit
	shares.push_back(Share(*database));
	//std::iterator<Share> iter = shares.begin();
	//Scanner *scan = new Scanner(*shares.begin(), *database);
	//delete(scan);


}

Main::~Main() {
	delete(database);
	std::cout << "Good Bye" << std::endl;
	// TODO Auto-generated destructor stub
}

void Main::readSettings(std::string settingsFile) {

}



int main(int argc, char **argv) {
	Main *mn = new Main();
	delete(mn);
	return 0;
}

