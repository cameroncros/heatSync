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
#include "Network.h"
#include "SyncClient.h"
#include "SecureConnection.h"

Main *mn;

/*
 * Necessary Threads:
 * 	1 per share for watching filesystem
 * 	2 per share for actual syncing
 * 	  - must consult each available connection
 * 	  for most up to date changes
 *  1 thread for database
 *  1 thread for discovery
 */

Main::Main() {
	database = NULL;
	network = NULL;

}

Main::~Main() {
	delete(database);
	delete(network);
	std::cout << "Good Bye" << std::endl;
	// TODO Auto-generated destructor stub
}

void Main::mainLoop() {
	database = new Sqlite();
	network = new Network();
	std::cout << "Welcome to HeatSync." << std::endl;

	settingsDir = getenv("HOME");
	settingsDir.append("/.heatSync/");
	settingsFile = settingsDir;
	settingsFile.append("config");
	createSettingsDir();

	readSettings();




	database->getShares(shares);


	//new SyncClient(new SecureConnection((char *)"192.168.1.110", (char *)"19669"));
	setupSyncClients();

	pause();
}

void Main::setupSyncClients() {
	std::map<int, Share *>::iterator i;
	std::map<std::string, Host *>::iterator j;
	if (shares.empty() || hosts.empty()) {
		return;
	}
	for(j = hosts.begin(); j != hosts.end(); j++) {
		std::cout << j->first << " - " << j->second << std::endl;
	}
	for (i = shares.begin(); i != shares.end(); i++) {
		std::cout << i->first << " - " << i->second << std::endl;
	}
	for(j = hosts.begin(); j != hosts.end(); j++) {
		for (i = shares.begin(); i != shares.end(); i++) {

			if (syncClients.find(i->second) != syncClients.end() &&
					syncClients[i->second].find(j->second) != syncClients[i->second].end()) {
				if (!syncClients[i->second][j->second]->isConnected()) {
					delete(syncClients[i->second][j->second]);
					syncClients[i->second][j->second] = new SyncClient(new SecureConnection(j->second->getHostname().c_str(), j->second->getPort().c_str()), i->second);
				}
			} else {
				syncClients[i->second][j->second] = new SyncClient(new SecureConnection(j->second->getHostname().c_str(), j->second->getPort().c_str()), i->second);
			}
		}
		//todo: Add solo connection for discovering shared resources, alternatively, publish shares individually with avahi etc.
	}
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
		file.open(settingsFile.c_str(), std::ios::in);
		std::string name, value;
		while (file.is_open() && !file.eof()) {
			std::getline(file, name, '=');
			std::getline(file, value, '\n');
			//std::cout << name << "/" << value << std::endl;
			settings[name]=value;
		}
	}
}

void Main::addHost(Host* hst) {//todo: check if host already exists
	if (hosts.find(hst->getHostname()) == hosts.end()) {
		hosts[hst->getName()]=hst;
		setupSyncClients();
	} else {
		free(hst);
	}

}

void Main::deleteHost(char* name) {
	std::string nm = name;
	if (hosts.find(nm) == hosts.end()) {
		delete(hosts[nm]);
		hosts.erase(nm);
	}
}




int main(int argc, char **argv) {
	mn = new Main();
	mn->mainLoop();
	delete(mn);
	return 0;
}
