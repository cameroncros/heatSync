/*
 * Watcher.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#include "Watcher.h"
#include "File.h"
#include <sys/inotify.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <cstring>
#include <errno.h>
#include <sys/stat.h>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <algorithm>


class File;




Watcher::Watcher(Share &share, Database &data, int depth, int hidden, int symbolic) {
	this->depth = depth;
	this->hidden = hidden;
	this->share = &share;
	this->db = &data;
	this->symbolic = symbolic;
	watchfile = inotify_init();
	if (watchfile == -1) {
		std::cerr << "Can't init inotify: " <<strerror(errno) << std::endl;
		throw std::exception();
	}
	trd = new std::thread(&Watcher::monitor, this);
	// TODO Auto-generated constructor stub

}

Watcher::~Watcher() {
	trd->join();
	close(watchfile);
	// TODO Auto-generated destructor stub
}

void Watcher::addWatch(std::string &path) {
	int wd = inotify_add_watch(watchfile,path.c_str(),IN_CLOSE_WRITE|IN_ATTRIB|IN_CREATE|IN_DELETE|IN_MODIFY|IN_DELETE|IN_DELETE_SELF|IN_MOVE_SELF|IN_MOVED_FROM|IN_MOVED_TO);
	if (wd == -1) {
		std::cerr << "Can't add watch: " << path << " - "<<strerror(errno) << std::endl;
	} else {
		watchPath[wd]= path;
	}

}

void Watcher::monitor() {
	struct inotify_event *event;
	event = (inotify_event*)malloc(sizeof(struct inotify_event) + NAME_MAX + 1);
	while(true) {
		read(watchfile, event, sizeof(struct inotify_event) + NAME_MAX + 1);
		parseEvent(event);
	}
	free(event);
}

void Watcher::parseEvent(inotify_event *event) {
	std::string fname = watchPath[event->wd];
	fname.append("/").append(event->name);
	switch (event->mask) {
	case IN_MODIFY:
	case IN_CLOSE_WRITE:
	case IN_ATTRIB:
		/*moddified*/
	case IN_CREATE:
	case IN_MOVED_TO:
		/*created*/
	case IN_DELETE:
	case IN_MOVED_FROM:
	default:
		/*deleted*/
		struct stat fstats;
		stat(fname.c_str(), &fstats);
		if (S_ISDIR(fstats.st_mode)) {
			readDir(fname);
		} else if(S_ISREG(fstats.st_mode)) {
			File *fl;
			fl = new File(fname);
			db->insertFileDetails(*fl, *share);
			free(fl);
		} else if (S_ISLNK(fstats.st_mode) && share->isSymlink()) {
			if (lstat(fname.c_str(), &fstats)) {
				std::cerr << "Cant stat symlink" << std::endl;
			}
			if (S_ISDIR(fstats.st_mode)) {
				readDir(fname);
			} else {
				File *fl;
				fl = new File(fname);
				db->insertFileDetails(*fl, *share);
				free(fl);
			}

		}
		std::cout << fname << " Was Changed" << std::endl;
		break;
	case IN_DELETE_SELF:
	case IN_MOVE_SELF:
		inotify_rm_watch(watchfile,event->wd);
		watchPath.erase(event->wd);
		std::cout << watchPath[event->wd] << "was deleted or moved" << std::endl;
		break;
	/*default:
		std::cout << fname << " Something Else Happened - " << event->mask << std::endl;
		throw std::exception();
		break;*/

	}

}

void Watcher::readDir(std::string &searchPath) {
	DIR *dir = NULL;
	dirent *tmp = NULL;
	File *file = NULL;
	std::string newpath;
	struct stat fstats;
	int currentDepth = std::count(searchPath.begin(), searchPath.end(), '/')-std::count(path.begin(), path.end(), '/');
	addWatch(searchPath);
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
					if (symbolic) {
						newpath = searchPath;
						newpath.append("/").append(tmp->d_name);
						if (lstat(newpath.c_str(), &fstats)) {
							std::cerr << "Cant stat symlink" << std::endl;
						}
						if (S_ISDIR(fstats.st_mode)) {

							readDir(newpath);
						} else {
							file = new File(newpath);
							db->insertFileDetails(*file, *share);
							delete(file);
							file = NULL;
						}
					}
					break;
				case DT_REG:
					newpath = searchPath;
					file = new File(newpath.append("/").append(tmp->d_name));
					std::cout << newpath << std::endl;
					db->insertFileDetails(*file, *share);
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







