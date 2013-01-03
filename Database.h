/*
 * Database.h
 *
 *	Maintains the database
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include "File.h"
#include "Host.h"
#include "Share.h"
class Host;
class Share;

class Database {
public:
	Database();
	virtual ~Database();

	virtual File *getFileDetails()=0;
	virtual void insertFileDetails(File &, Share &)=0;

	virtual Share *getShareDetails()=0;
	virtual void insertShareDetails(Share &)=0;

	virtual Host *getHostDetails()=0;
	virtual void insertHostDetails(Host &)=0;

	virtual void removeHiddenFiles(Share &)=0;
	virtual void initialiseDatabase()=0;
};

#endif /* DATABASE_H_ */
