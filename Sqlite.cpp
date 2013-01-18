/*
 * Sqlite.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#include "Sqlite.h"
#include "File.h"

#include <sqlite3.h>
#include <cstdlib>
#include <iostream>



Sqlite::Sqlite() {
	dbHandle = NULL;
	int out;
	sqlite3_config(SQLITE_CONFIG_URI, 1);
	out = sqlite3_open_v2("file:.heatSync/files.db", &dbHandle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, NULL);
	if (out != SQLITE_OK || dbHandle == NULL) {
		std::cout << out << " - " << sqlite3_errmsg(dbHandle) << std::endl;
	}
	initialiseDatabase();
	// TODO Auto-generated constructor stub

}

Sqlite::~Sqlite() {
	sqlite3_close(dbHandle);
	// TODO Auto-generated destructor stub
}

File *Sqlite::getFileDetails() {
	return NULL;
}
void Sqlite::insertFileDetails(File &nFile, Share &shr) {
	int out;
	char *sqlString = NULL;
	sqlString = sqlite3_mprintf("INSERT OR ABORT INTO files "
			"(fname,fhash,fdatemod,fsize,fperm,fowner,fgroup,fdeleted,fscanned)"
			" VALUES ('%q','%q','%i','%i','%i','%q','%q','%i','%i');\n",
			nFile.getName().c_str(),
			nFile.getHash().c_str(),
			nFile.getChanged(),
			nFile.getSize(),
			nFile.getPerm(),
			nFile.getOwner().c_str(),
			nFile.getGroup().c_str(),
			nFile.getDeleted(),
			nFile.getScanned()
	);
	std::cout << "Inserting file:" << nFile.getName() << " - " << sqlString <<std::endl;
	if ((out = sqlite3_exec(dbHandle,sqlString, NULL, 0, NULL )) != SQLITE_OK) {
		sqlite3_free(sqlString);
		sqlString = NULL;
		if (out == SQLITE_ABORT) {
			std::cout << "File Exists, Updating instead;" << std::endl;
			sqlString = sqlite3_mprintf("UPDATE files SET"
					"fhash = '%q',"
					"fscanned = '%i'"
					"fdatemod = '%i',"
					"fsize = '%i',"
					"fperm = '%i',"
					"fowner = '%q',"
					"fgroup = '%q',"
					"fdeleted = '%i',"
					"fscanned = '%i'"
					"WHERE fname = '%q'",
					nFile.getHash().c_str(),
					nFile.getScanned(),
					nFile.getChanged(),
					nFile.getSize(),
					nFile.getPerm(),
					nFile.getOwner().c_str(),
					nFile.getGroup().c_str(),
					nFile.getDeleted(),
					nFile.getScanned(),
					nFile.getName().c_str()
			);
			if (sqlite3_exec(dbHandle,sqlString, NULL, 0, NULL ) != SQLITE_OK) {
				std::cerr << sqlString << std::endl;
				std::cerr << sqlite3_errmsg(dbHandle) << std::endl;
			}
		}else {
			std::cerr << sqlString << std::endl;
			std::cerr << sqlite3_errmsg(dbHandle) << std::endl;
		}
		sqlite3_free(sqlString);
		sqlString = NULL;
	} else {
		sqlite3_free(sqlString);
		sqlString = NULL;
	}

	sqlString = sqlite3_mprintf("INSERT OR REPLACE INTO sharefiles "
			"(shareid, fileid) "
			"SELECT '%i',fileid "
			"FROM files WHERE "
			"fhash IS '%q' AND "
			"fname IS '%q' AND "
			"fdatemod IS '%i'"
			";\n",

			shr.getShareID(),
			nFile.getHash().c_str(),
			nFile.getName().c_str(),
			nFile.getChanged()
	);
	if (sqlite3_exec(dbHandle,sqlString, NULL, 0, NULL ) != SQLITE_OK) {
		std::cerr << sqlString << std::endl;
		std::cerr << sqlite3_errmsg(dbHandle) << std::endl;
	}
	sqlite3_free(sqlString);
	sqlString = NULL;
}

Share *Sqlite::getShareDetails() {
	return NULL;
}
void Sqlite::insertShareDetails(Share &) {

}

Host *Sqlite::getHostDetails() {
	return NULL;
}
void Sqlite::insertHostDetails(Host &) {

}

void Sqlite::removeHiddenFiles(Share &shr) {
	//	char *sqlString = NULL;
	//	sqlString = sqlite3_mprintf("DELETE FROM sharefiles.sf, files.f "
	//			"INNER JOIN sf.shareid = f.shareid"
	//			"WHERE (f.fname GLOB '*/.*' OR f.fname GLOB '*~') "
	//			"AND sf.shareid IS '%i'", shr.getShareID());
	//	if (sqlite3_exec(dbHandle,sqlString, NULL, 0, NULL ) != SQLITE_OK) {
	//		std::cout << sqlString << std::endl;
	//		std::cerr << sqlite3_errmsg(dbHandle) << std::endl;
	//	}
	//	sqlString = sqlite3_mprintf("DELETE FROM files "
	//			"WHERE (fname GLOB '*/.*' OR fname GLOB '*~') "
	//			"AND shareid IS '%i'", shr.getShareID());
	//	if (sqlite3_exec(dbHandle,sqlString, NULL, 0, NULL ) != SQLITE_OK) {
	//		std::cout << sqlString << std::endl;
	//		std::cerr << sqlite3_errmsg(dbHandle) << std::endl;
	//	}
}

void Sqlite::initialiseDatabase() {
	if (sqlite3_exec(dbHandle, "CREATE TABLE IF NOT EXISTS shares ("
			"sid INTEGER PRIMARY KEY AUTOINCREMENT,"
			"sname TEXT NOT NULL,"
			"sdir TEXT NOT NULL"
			")", NULL, 0, NULL ) ) {
		std::cerr << sqlite3_errmsg(dbHandle) << std::endl;
	}
	if (sqlite3_exec(dbHandle, "CREATE TABLE IF NOT EXISTS user ("
			"userid INTEGER PRIMARY KEY AUTOINCREMENT,"
			"username TEXT NOT NULL,"
			"password TEXT NOT NULL"
			")", NULL, 0, NULL ) ) {
		std::cerr << sqlite3_errmsg(dbHandle) << std::endl;
	}
	if (sqlite3_exec(dbHandle, "CREATE TABLE IF NOT EXISTS files ("
			"fileid INTEGER PRIMARY KEY AUTOINCREMENT,"
			"fname TEXT NOT NULL UNIQUE,"
			"fhash TEXT NOT NULL,"
			"fscanned INTEGER NOT NULL,"
			"fdatemod INTEGER NOT NULL,"
			"fsize INTEGER NOT NULL,"
			"fperm INTEGER,"
			"fowner TEXT,"
			"fgroup TEXT,"
			"fdeleted INTEGER"
			")", NULL, 0, NULL) ) {
		std::cerr << sqlite3_errmsg(dbHandle) << std::endl;
	}
	if (sqlite3_exec(dbHandle, "CREATE TABLE IF NOT EXISTS usershares ("
			"userid INTEGER REFERENCES users(userid),"
			"shareid INTEGER REFERENCES shares(shareid),"
			"PRIMARY KEY (userid, shareid)"
			")", NULL, 0, NULL) ) {
		std::cerr << sqlite3_errmsg(dbHandle) << std::endl;
	}
	if (sqlite3_exec(dbHandle, "CREATE TABLE IF NOT EXISTS sharefiles ("
			"shareid INTEGER REFERENCES shares(shareid),"
			"fileid INTEGER REFERENCES files(shareid),"
			"PRIMARY KEY (shareid, fileid)"
			")", NULL, 0, NULL) ) {
		std::cerr << sqlite3_errmsg(dbHandle) << std::endl;
	}
}
