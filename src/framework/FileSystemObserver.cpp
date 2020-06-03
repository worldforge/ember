/*
 Copyright (C) 2017 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <boost/algorithm/string.hpp>
#include <boost/exception/exception.hpp>
#include "FileSystemObserver.h"
#include "LoggingInstance.h"

namespace Ember {

FileSystemObserver::FileSystemObserver(boost::asio::io_service& ioService) {
	try {
		mDirectoryMonitor = std::make_unique<boost::asio::dir_monitor>(ioService);
		observe();
	} catch (const boost::exception& e) {
		S_LOG_WARNING("Could not initialize file system observer; probably due to running out of file descriptors.");
	}

}

FileSystemObserver::~FileSystemObserver() {
	assert(mCallBacks.empty());
}


void FileSystemObserver::observe() {
	if (mDirectoryMonitor) {
		mDirectoryMonitor->async_monitor([this](const boost::system::error_code& ec, const boost::asio::dir_monitor_event& ev) {
			if (!ec && ev.type != boost::asio::dir_monitor_event::null) {
				for (const auto& I : mCallBacks) {
					if (boost::starts_with(ev.path.string(), I.first.string())) {
						auto relative = boost::filesystem::relative(ev.path, I.first);
						FileSystemEvent event{
								ev,
								relative
						};
						I.second(event);

						break;
					}
				}
				this->observe();
			}
		});
	}
}

void FileSystemObserver::add_directory(const boost::filesystem::path& dirname, std::function<void(const FileSystemObserver::FileSystemEvent&)> callback) {
	if (mDirectoryMonitor) {
		mCallBacks.insert(std::make_pair(dirname, std::move(callback)));
		mDirectoryMonitor->add_directory(dirname.string());
	}
}

void FileSystemObserver::remove_directory(const boost::filesystem::path& dirname) {
	if (mDirectoryMonitor) {
		mCallBacks.erase(dirname);
		try {
			mDirectoryMonitor->remove_directory(dirname.string());
		} catch (...) {
			//Just swallow exceptions when removing watches; this is often because the directory has been removed. Doesn't change anything.
		}
	}
}

}