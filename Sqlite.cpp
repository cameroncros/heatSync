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
	std::string sqlString;
	std::string dbFile;
	dbFile = "file:";
	dbFile.append(getenv("HOME"));
	dbFile.append("/.heatSync/files.db");
	out = sqlite3_open_v2(dbFile.c_str(), &dbHandle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, NULL);
	if (out != SQLITE_OK || dbHandle == NULL) {
		std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") " << out << dbFile.c_str() << " - " << sqlite3_errmsg(dbHandle) << std::endl;
	}
	initialiseDatabase();
	sqlString = "INSERT OR ABORT INTO files "
			"(fname,fhash,fdatemod,fsize,fperm,fowner,fgroup,fdeleted,fscanned)"
			" VALUES (?,?,?,?,?,?,?,?,?);";
	out = sqlite3_prepare_v2(dbHandle, sqlString.c_str(), sqlString.length(), &insertFileStmt, NULL);
	if (out != SQLITE_OK) {
		std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") " << out << " - " << sqlite3_errmsg(dbHandle) << std::endl;
	}
	sqlString = "UPDATE files SET "
			"fhash = ?,"
			"fdatemod = ?,"
			"fsize = ?,"
			"fperm = ?,"
			"fowner = ?,"
			"fgroup = ?,"
			"fdeleted = ?,"
			"fscanned = ?"
			"WHERE fname = ?";
	out = sqlite3_prepare_v2(dbHandle, sqlString.c_str(), sqlString.length(), &updateFileStmt, NULL);
	if (out != SQLITE_OK) {
		std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") " << out << " - " << sqlite3_errmsg(dbHandle) << std::endl;
	}
	sqlString = "INSERT OR REPLACE INTO sharefiles "
			"(shareid, fileid) "
			"SELECT ?,fileid "
			"FROM files WHERE "
			"fhash IS ? AND "
			"fname IS ? AND "
			"fdatemod IS ?;";
	out = sqlite3_prepare_v2(dbHandle, sqlString.c_str(), sqlString.length(), &insertShareFile, NULL);
	if (out != SQLITE_OK) {
		std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") " << out << " - " << sqlite3_errmsg(dbHandle) << std::endl;
	}
	// TODO Auto-generated constructor stub

}

Sqlite::~Sqlite() {
	sqlite3_finalize(insertFileStmt);
	sqlite3_finalize(updateFileStmt);
	sqlite3_finalize(insertShareFile);
	sqlite3_close(dbHandle);
	// TODO Auto-generated destructor stub
}

File *Sqlite::getFileDetails() {
	return NULL;
}
void Sqlite::insertFileDetails(File &nFile, Share &shr) {
	mut.lock();
	int out;
	sqlite3_bind_text(insertFileStmt, 1, nFile.getName().c_str(), nFile.getName().length(), 0);
	sqlite3_bind_text(insertFileStmt, 2, nFile.getHash().c_str(), nFile.getHash().length(), 0);
	sqlite3_bind_int (insertFileStmt, 3, nFile.getChanged());
	sqlite3_bind_int (insertFileStmt, 4, nFile.getSize());
	sqlite3_bind_int (insertFileStmt, 5, nFile.getPerm());
	sqlite3_bind_text(insertFileStmt, 6, nFile.getOwner().c_str(), nFile.getOwner().length(), 0);
	sqlite3_bind_text(insertFileStmt, 7, nFile.getGroup().c_str(), nFile.getGroup().length(), 0);
	sqlite3_bind_int (insertFileStmt, 8, nFile.getDeleted());
	sqlite3_bind_int (insertFileStmt, 9, nFile.getScanned());
	while ((out = sqlite3_step(insertFileStmt)) != SQLITE_DONE) {
		//std::cerr << out << " - " << sqlite3_errmsg(dbHandle) << std::endl;
		if (out == SQLITE_ERROR) {
			throw std::exception();
		} else if (out == SQLITE_ABORT) {
			break;
		}else if (out == SQLITE_CONSTRAINT) {
			break;
		}
	}
	if (out != SQLITE_OK) {

		sqlite3_bind_text(updateFileStmt, 1, nFile.getHash().c_str(), nFile.getHash().length(), 0);
		sqlite3_bind_int (updateFileStmt, 2, nFile.getChanged());
		sqlite3_bind_int (updateFileStmt, 3, nFile.getSize());
		sqlite3_bind_int (updateFileStmt, 4, nFile.getPerm());
		sqlite3_bind_text(updateFileStmt, 5, nFile.getOwner().c_str(), nFile.getOwner().length(), 0);
		sqlite3_bind_text(updateFileStmt, 6, nFile.getGroup().c_str(), nFile.getGroup().length(), 0);
		sqlite3_bind_int (updateFileStmt, 7, nFile.getDeleted());
		sqlite3_bind_int (updateFileStmt, 8, nFile.getScanned());
		sqlite3_bind_text(updateFileStmt, 9, nFile.getName().c_str(), nFile.getName().length(), 0);
		while ((out = sqlite3_step(updateFileStmt)) != SQLITE_DONE) {
			if (out == SQLITE_ERROR) {
				std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") "<< sqlite3_errmsg(dbHandle) << std::endl;
				throw std::exception();
			}
		}
	} else {
		sqlite3_bind_int (insertShareFile, 1, shr.getShareID());
		sqlite3_bind_text(insertShareFile, 2, nFile.getHash().c_str(), nFile.getHash().length(), 0);
		sqlite3_bind_text(insertShareFile, 3, nFile.getHash().c_str(), nFile.getHash().length(), 0);
		sqlite3_bind_int (insertShareFile, 4, nFile.getChanged());

		while ((out = sqlite3_step(insertShareFile)) != SQLITE_DONE) {
			if (out == SQLITE_ERROR) {
				std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") "<< sqlite3_errmsg(dbHandle) << std::endl;
				throw std::exception();
			}
		}
	}
	sqlite3_reset(insertShareFile);
	sqlite3_reset(updateFileStmt);
	sqlite3_reset(insertFileStmt);

	mut.unlock();

}

void Sqlite::getShares(std::vector<Share> &shares) {
	sqlite3_stmt *shrsql;
	int out;
	std::string sqlString = "SELECT sid, sname, sdir, sdep, ssym, shid, slast FROM shares;";
	out = sqlite3_prepare_v2(dbHandle, sqlString.c_str(), sqlString.length(), &shrsql, NULL);
	if (out != SQLITE_OK) {
		std::cout << "(" << __FILE__ << ":" << __LINE__ << ") " << out << " - " << sqlite3_errmsg(dbHandle) << std::endl;
	}
	while ((out = sqlite3_step(shrsql)) != SQLITE_DONE) {
		if (out == SQLITE_ROW) {
			shares.push_back(Share((Database *)this,
					sqlite3_column_text(shrsql, 1), //nm
					sqlite3_column_text(shrsql, 2), //pth
					sqlite3_column_int(shrsql, 3),	//dep
					sqlite3_column_int(shrsql, 4),	//sym
					sqlite3_column_int(shrsql, 5),	//hid
					sqlite3_column_int(shrsql, 0),	//id
					sqlite3_column_int(shrsql, 6)	//last
					));
		} else {
			std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") " << sqlite3_errmsg(dbHandle) << std::endl;
		}
	}


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
			"sdir TEXT NOT NULL,"
			"sdep INTEGER,"
			"ssym INTEGER,"
			"shid INTEGER,"
			"slast INTEGER"
			")", NULL, 0, NULL ) ) {
		std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") " << sqlite3_errmsg(dbHandle) << std::endl;
	}
	if (sqlite3_exec(dbHandle, "CREATE TABLE IF NOT EXISTS user ("
			"userid INTEGER PRIMARY KEY AUTOINCREMENT,"
			"username TEXT NOT NULL,"
			"password TEXT NOT NULL"
			")", NULL, 0, NULL ) ) {
		std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") " << sqlite3_errmsg(dbHandle) << std::endl;
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
		std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") " << sqlite3_errmsg(dbHandle) << std::endl;
	}
	if (sqlite3_exec(dbHandle, "CREATE TABLE IF NOT EXISTS usershares ("
			"userid INTEGER REFERENCES users(userid),"
			"shareid INTEGER REFERENCES shares(shareid),"
			"PRIMARY KEY (userid, shareid)"
			")", NULL, 0, NULL) ) {
		std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") " << sqlite3_errmsg(dbHandle) << std::endl;
	}
	if (sqlite3_exec(dbHandle, "CREATE TABLE IF NOT EXISTS sharefiles ("
			"shareid INTEGER REFERENCES shares(shareid),"
			"fileid INTEGER REFERENCES files(shareid),"
			"PRIMARY KEY (shareid, fileid)"
			")", NULL, 0, NULL) ) {
		std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") " << sqlite3_errmsg(dbHandle) << std::endl;
	}
}
