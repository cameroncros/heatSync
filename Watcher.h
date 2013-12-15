/*
 * Watcher.h
 *
 *	Watches the filesystem for changes and pushes these changes to the database
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#ifndef WATCHER_H_
#define WATCHER_H_

#include <sys/inotify.h>
#include <linux/limits.h>
#include <string>
#include <thread>
#include <map>

#include "Share.h"
#include "Database.h"
class Database;
class Share;

class Watcher {
private:
	int watchfile;
	int depth;
	int hidden;
	int symbolic;
	Share *share;
	Database *db;
	std::string path;
	std::thread *trd;
	std::map<int, std::string> watchPath;
public:
	Watcher(Share &, Database &, int depth, int hidden, int symboliclink);
	virtual ~Watcher();
	void addWatch(std::string &path);
	void parseEvent(inotify_event *event);
	void monitor();
	void readDir(std::string &searchPath);
};

#endif /* WATCHER_H_ */
