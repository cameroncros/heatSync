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

class Main {
private:
	std::vector<Share> shares;
	std::map<std::string, std::string> settings;
	Database *database;
public:
	Main();
	virtual ~Main();

	void addShare(Share &);
	void removeShare(Share &);
	void readSettings(std::string);

	Share *getShareList();
	//Host *getHostList();
};

#endif /* MAIN_H_ */
