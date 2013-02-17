/*
 * Share.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#include "Share.h"
#include "File.h"
#include "Watcher.h"

#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <sys/stat.h>
#include <algorithm>

Share::Share(Database *db, const unsigned char *nm, const unsigned char *pth, int dep, int sym, int hid, int id, int last) {
	std::cout << nm << " - " << pth << std::endl;

	name = (char *)nm;
	path = (char *)pth;
	lastChanged = last;
	shareID = id;
	depth = dep;
	symlink = sym;
	hidden = hid;
	database = db;
	watch = new Watcher(*this, *database);
	if (hidden == 0) {
		//database->removeHiddenFiles(*this);
	}
	db->insertShareDetails(*this);
	// TODO Auto-generated constructor stub

}

Share::~Share() {
	// TODO Auto-generated destructor stub
}

bool Share::checkPassword() {
	return true;
}

int Share::getDepth() const {
	return depth;
}

void Share::setDepth(int depth) {
	this->depth = depth;
}

bool Share::isHidden() const {
	return hidden;
}

void Share::setHidden(bool hidden) {
	this->hidden = hidden;
}

int Share::getLastChanged() const {
	return lastChanged;
}

void Share::setLastChanged(int lastChanged) {
	this->lastChanged = lastChanged;
}

const std::string& Share::getName() const {
	return name;
}

void Share::setName(const std::string& name) {
	this->name = name;
}

const std::string& Share::getPasshash() const {
	return passhash;
}

void Share::setPasshash(const std::string& passhash) {
	this->passhash = passhash;
}

const std::string& Share::getPath() const {
	return path;
}

void Share::setPath(const std::string& path) {
	this->path = path;
}

int Share::getShareId() const {
	return shareID;
}

void Share::setShareId(int shareId) {
	shareID = shareId;
}

bool Share::isSymlink() const {
	return symlink;
}

void Share::startScan() {
	int start = time(NULL);
	readDir(path);
	std::cout << "Initial Scan took " << time(NULL)-start << " Seconds" << std::endl;
}

void Share::setSymlink(bool symlink) {
	this->symlink = symlink;
}

void Share::readDir(std::string &searchPath) {
	DIR *dir = NULL;
	dirent *tmp = NULL;
	File *file = NULL;
	std::string newpath;
	struct stat fstats;
	int currentDepth = std::count(searchPath.begin(), searchPath.end(), '/')-std::count(path.begin(), path.end(), '/');
	watch->addWatch(searchPath);
	if (depth - currentDepth > 0 || depth == 0) { //warning. depth should not be infinite if symlinks are also followed, otherwise infinite loops will occur.

		dir = opendir(searchPath.c_str());
		if (dir == NULL) {
			std::cerr << "Can't open directory: " << searchPath << std::endl;
			closedir(dir);
			return;
		}

		while ((tmp = readdir(dir))) {
			if (tmp->d_name[0] == '.' && (
					(tmp->d_name[1] == '.' && tmp->d_name[2] == '\0') ||
					(tmp->d_name[1] == '\0') ||
					(hidden == 0))
			) {
			} else {
				switch (tmp->d_type) {
				case DT_DIR:
					newpath = searchPath;
					newpath.append("/").append(tmp->d_name);
					readDir(newpath);

					break;
				case DT_LNK:
					if (symlink) {
						newpath = searchPath;
						newpath.append("/").append(tmp->d_name);
						if (lstat(newpath.c_str(), &fstats)) {
							std::cerr << "Cant stat symlink" << std::endl;
						}
						if (S_ISDIR(fstats.st_mode)) {

							readDir(newpath);
						} else {
							file = new File(newpath);
							database->insertFileDetails(*file, *this);
							delete(file);
							file = NULL;
						}
					}
					break;
				case DT_REG:
					newpath = searchPath;
					file = new File(newpath.append("/").append(tmp->d_name));
					std::cout << newpath << std::endl;
					database->insertFileDetails(*file, *this);
					delete(file);
					file = NULL;
					break;
				default:
					std::cerr << "Unknown File" << std::endl;
					break;
				}
			}

		}


		closedir(dir);
	}

}


