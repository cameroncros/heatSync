/*
 * File.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#include "File.h"
#include <iostream>
#include <iomanip>

#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <sys/mman.h>
#include <openssl/md5.h>
#include <errno.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>



File::File(std::string &fname) {
	struct stat fstats;
	stat(fname.c_str(), &fstats);
	fileid = 0;
	perm = 0; //TODO
	name = fname;
	size = fstats.st_size;
	owner = fstats.st_uid;
	group = fstats.st_gid;
	changed = fstats.st_mtim.tv_sec;
	scanned = time(NULL);
	deleted = 0;
	try {
		computeHash();
	} catch (...) {
		deleted = 1;
		std::cerr << "hashing failed: " << name << std::endl;
	}

	// TODO Auto-generated constructor stub

}

File::~File() {
	// TODO Auto-generated destructor stub
}

void File::computeHash() {
	MD5_CTX mdContext;
	int bytesRead;
	unsigned char hashArray[MD5_DIGEST_LENGTH];
	unsigned char data[1024];
	std::ostringstream hashStr;

	FILE *file = fopen(name.c_str(), "rb");
	if (file == NULL) {
		std::cerr << "Failed to open file for hashing: " << strerror(errno) << std::endl;
		throw std::exception();
	}
	MD5_Init(&mdContext);
	while ((bytesRead = fread(data, 1, 1024, file)) != 0) {
		MD5_Update(&mdContext, data, bytesRead);
	}
	fclose(file);
	MD5_Final (hashArray, &mdContext);

	hashStr << std::hex << std::setfill( '0' );

	for(int i=0; i <MD5_DIGEST_LENGTH; i++) {
		hashStr << std::setw(2) << (int)hashArray[i];
	}

	hash = hashStr.str();
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

int File::getFileId() {
	return fileid;
}

int File::getDeleted() {
	return deleted;
}

