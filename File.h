/*
 * File.h
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#ifndef FILE_H_
#define FILE_H_

#include <string>

class File {
private:
	std::string hash;
	std::string name;
	int	size;
	int scanned;
	int changed;
	int perm;
	std::string owner;
	std::string group;
	int fileid;
	int deleted;
	void computeHash();
public:
	File(std::string);
	virtual ~File();

	std::string getHash();
	std::string getName();
	int getSize();
	int getScanned();
	int getChanged();
	int getPerm();
	std::string getOwner();
	std::string getGroup();
	int getFileId();
	int getDeleted();

};

#endif /* FILE_H_ */
