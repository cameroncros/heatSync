/*
 * Main.h
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <cstdlib>
#include <vector>
#include <map>
#include <string>

class Share;
class Database;
class Network;

class Main {
private:
	std::string settingsDir;
	std::string settingsFile;
	std::map<int, Share *> shares;
	std::map<std::string, std::string> settings;
	Database *database;
	Network *network;
public:
	Main();
	virtual ~Main();

	void addShare(Share &);
	void removeShare(Share &);
	void createSettingsDir();
	void readSettings();

	Share *getShareList();
	//Host *getHostList();
};

#endif /* MAIN_H_ */
