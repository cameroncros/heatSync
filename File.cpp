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



File::File(std::string fname) {
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
	computeHash();

	// TODO Auto-generated constructor stub

}

File::~File() {
	// TODO Auto-generated destructor stub
}

void File::computeHash() {
	unsigned char *hashArray;
	unsigned char *fData;
	std::ostringstream hashStr;
	int file;
	file = open(name.c_str(), O_RDONLY);

	if (file == -1) {
		std::cerr << "Failed to open file for hashing: " << strerror(errno) << std::endl;
		return;
	}

	fData = (unsigned char *)mmap(0, size, PROT_READ, MAP_SHARED, file, 0);
	hashArray = MD5(fData, size, NULL);
	hashStr << std::hex << std::setfill( '0' );

	for(int i=0; i <MD5_DIGEST_LENGTH; i++) {
        hashStr << std::setw(2) << (int)hashArray[i];
    }

    hash = hashStr.str();
	munmap(fData, size);
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

