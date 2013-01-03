/*
 * File.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#include "File.h"
#include <iostream>
#include <sys/stat.h>


File::File(std::string fname) {
	struct stat fstats;
	stat(fname.c_str(), &fstats);
	fileid = 0;
	perm = 0;

	size = fstats.st_size;
	owner = fstats.st_uid;
	group = fstats.st_gid;
	changed = fstats.st_mtim.tv_sec;
	scanned = time(NULL);

	name = fname;

	// TODO Auto-generated constructor stub

}

File::~File() {
	// TODO Auto-generated destructor stub
}

std::string File::getHash(){
	return hash;
}

std::string File::getName() {
	return name;
}

int File::getSize() {
	return size;
}

int File::getScanned() {
	return scanned;
}

int File::getChanged() {
	return changed;
}

int File::getPerm() {
	return perm;
}

std::string File::getOwner() {
	return owner;
}

std::string File::getGroup() {
	return group;
}

int File::getFileId(){
	return fileid;
}

