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

Share::Share(Database &db) {
	path = "/home/cameron/Projects";
	lastChanged = 0;
	shareID = 1;
	depth = 3;
	symlink = 0;
	hidden = 0;
	database = &db;
	readDir(path, depth);
		if (hidden == 0) {
			//database->removeHiddenFiles(*this);
		}

	// TODO Auto-generated constructor stub

}

Share::~Share() {
	// TODO Auto-generated destructor stub
}

int Share::getShareID() {
	return shareID;
}

std::string Share::getPath() {
	return path;
}

int Share::getDepth() {
	return depth;
}

bool Share::getSymlink() {
	return symlink;
}

bool Share::getHidden() {
	return hidden;
}

bool Share::checkPassword() {
	return true;
}

void Share::readDir(std::string path, int depth) {
	DIR *dir = NULL;
	dirent *tmp = NULL;
	File *file = NULL;
	std::string newpath;
	if (depth >= 0) {

		dir = opendir(path.c_str());
		if (dir == NULL) {
			std::cerr << "Can't open directory: " << path << std::endl;
			closedir(dir);
			return;
		}

		while ((tmp = readdir(dir))) {
			if (std::string(tmp->d_name).compare("..") == 0 ||
					std::string(tmp->d_name).compare(".") == 0  ||
					(hidden == 0 && tmp->d_name[0] == '.')) {
			} else {
				switch (tmp->d_type) {
				case DT_DIR:
					newpath = path;
					newpath.append("/").append(tmp->d_name);
					watches.push_back(new Watcher(*this, *database, newpath));
					readDir(newpath.c_str(), depth - 1);

					break;
				case DT_LNK:
					if (symlink) {
						newpath = path;
						newpath.append("/").append(tmp->d_name);
						watches.push_back(new Watcher(*this, *database, newpath));
						readDir(newpath.c_str(), depth - 1);
					}
					break;
				case DT_UNKNOWN:
					std::cerr << "Unknown File" << std::endl;
					break;
				default:
					newpath = path;
					file = new File(newpath.append("/").append(tmp->d_name));
					std::cout << newpath << std::endl;
					database->insertFileDetails(*file, *this);
					delete(file);
					file = NULL;
					break;
				}
			}

		}


		closedir(dir);
	}

}


