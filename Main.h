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
class Host;
class SyncClient;

class Main {
private:
	std::string settingsDir;
	std::string settingsFile;
	std::map<int, Share *> shares;
	std::map<std::string, Host *> hosts;
	std::map<std::string, std::string> settings;
	std::map<Share *, std::map<Host *, SyncClient *>> syncClients;
	Database *database;
	Network *network;
public:
	Main();
	virtual ~Main();

	void mainLoop();

	void setupSyncClients();
	void addShare(Share &);
	void removeShare(Share &);
	void createSettingsDir();
	void readSettings();

	Share *getShareList();
	void addHost(Host *);
	void deleteHost(char *name);

	//Host *getHostList();
};

#endif /* MAIN_H_ */
