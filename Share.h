/*
 * Share.h
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#ifndef SHARE_H_
#define SHARE_H_

#include <string>
#include <vector>

#include "Database.h"
#include "Watcher.h"
class Database;
class Watcher;


class Share {
private:
	int shareID;
	std::string path;
	int depth;
	bool symlink;
	bool hidden;
	std::string passhash;
	int lastChanged;
	Database *database;
	Watcher *watch;
public:
	Share(Database &);
	virtual ~Share();

	int getShareID();
	std::string getPath();
	int getDepth();
	bool getSymlink();
	bool getHidden();
	bool checkPassword();
	void readDir(std::string path);

};

#endif /* SHARE_H_ */
