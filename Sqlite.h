/*
 * Sqlite.h
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#ifndef SQLITE_H_
#define SQLITE_H_

#include "Database.h"
#include "File.h"
#include "Share.h"

#include "sqlite3.h"
#include <cstdlib>

class Sqlite: public Database {
private:
	sqlite3 *dbHandle;
public:
	Sqlite();
	virtual ~Sqlite();

	File *getFileDetails(); //returns a single file, with all the fields completed;
	void insertFileDetails(File &, Share &);

	Share *getShareDetails(); //todo: return array of all shares;
	void insertShareDetails(Share &);

	Host *getHostDetails(); //todo: returns array of hosts;
	void insertHostDetails(Host &);

	void removeHiddenFiles(Share &);
	void initialiseDatabase();
};

#endif /* SQLITE_H_ */
