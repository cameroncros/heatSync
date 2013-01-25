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
	std::string name;
	std::string path;
	int depth;
	bool symlink;
	bool hidden;
	std::string passhash;
	int lastChanged;
	Database *database;
	Watcher *watch;
public:
	Share(Database *db, const unsigned char *nm, const unsigned char *pth, int dep, int sym, int hid, int id, int last);
	virtual ~Share();

	bool checkPassword();
	void readDir(std::string &path);


	const Database*& getDatabase() const;
	void setDatabase(const Database*& database);
	int getDepth() const;
	void setDepth(int depth);
	bool isHidden() const;
	void setHidden(bool hidden);
	int getLastChanged() const;
	void setLastChanged(int lastChanged);
	const std::string& getName() const;
	void setName(const std::string& name);
	const std::string& getPasshash() const;
	void setPasshash(const std::string& passhash);
	const std::string& getPath() const;
	void setPath(const std::string& path);
	int getShareId() const;
	void setShareId(int shareId);
	bool isSymlink() const;
	void setSymlink(bool symlink);
	const Watcher& getWatch() const;
	void setWatch(const Watcher& watch);
};

#endif /* SHARE_H_ */
