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

Share::Share(Database &db) {
	path = "/home/cameron";
	lastChanged = 0;
	shareID = 1;
	depth = 3;
	symlink = 0;
	hidden = 1;
	database = &db;
	watch = new Watcher(*this, *database);
	int start = time(NULL);
	readDir(path);
	std::cout << "Initial Scan took " << time(NULL)-start << " Seconds" << std::endl;
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

void Share::readDir(std::string searchPath) {
	DIR *dir = NULL;
	dirent *tmp = NULL;
	File *file = NULL;
	std::string newpath;
	struct stat fstats;
	int currentDepth = std::count(searchPath.begin(), searchPath.end(), '/')-std::count(path.begin(), path.end(), '/');
	watch->addWatch(searchPath);
	if (depth - currentDepth > 0) {

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


