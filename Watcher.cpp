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


class File;




Watcher::Watcher(Share &share, Database &data) {
	shr = &share;
	db = &data;
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

void Watcher::addWatch(std::string path) {
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
		/*deleted*/
		/*struct stat fstats;
		stat(fname.c_str(), &fstats);
		if (S_ISDIR(fstats.st_mode)) {
			shr->readDir(fname);
		} else if(S_ISREG(fstats.st_mode)) {
			File *fl;
			fl = new File(fname);
			db->insertFileDetails(*fl, *shr);
			free(fl);
		} else if (S_ISLNK(fstats.st_mode) && shr->getSymlink()) {
			if (lstat(fname.c_str(), &fstats)) {
				std::cerr << "Cant stat symlink" << std::endl;
			}
			if (S_ISDIR(fstats.st_mode)) {
				shr->readDir(fname);
			} else {
				File *fl;
				fl = new File(fname);
				db->insertFileDetails(*fl, *shr);
				free(fl);
			}

		}*/
		std::cout << fname << " Was Changed" << std::endl;
		break;
	case IN_DELETE_SELF:
	case IN_MOVE_SELF:
		inotify_rm_watch(watchfile,event->wd);
		watchPath.erase(event->wd);
		std::cout << watchPath[event->wd] << "was deleted or moved" << std::endl;
		break;
	default:
		std::cout << fname << " Something Else Happened - " << event->mask << std::endl;
		throw std::exception();
		break;

	}

}







