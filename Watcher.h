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
#include <string>

#include "Share.h"
#include "Database.h"
class Database;
class Share;

#define NAME_MAX 256

class Watcher {
private:
	int watchfile;
	struct inotify_event *event;
	Share *shr;
	Database *db;
	std::string path;
public:
	Watcher(Share &, Database &, std::string);
	virtual ~Watcher();

	void work(long int);
};

#endif /* WATCHER_H_ */
