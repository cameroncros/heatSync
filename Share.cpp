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
	symbolic = sym;
	hidden = hid;
	database = db;
	watch = new Watcher(*this, *database, depth, hidden, symbolic);
	if (hidden == 0) {
		//TODO database->removeHiddenFiles(*this);
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
	return symbolic;
}

void Share::startScan() {
	int start = time(NULL);
	watch->readDir(path);
	std::cout << "Initial Scan took " << time(NULL)-start << " Seconds" << std::endl;
}

void Share::setSymlink(bool symlink) {
	this->symbolic = symlink;
}


