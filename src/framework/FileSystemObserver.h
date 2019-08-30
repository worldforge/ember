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

#ifndef EMBER_FILESYSTEMOBSERVER_H
#define EMBER_FILESYSTEMOBSERVER_H

#include "framework/dir_monitor/include/dir_monitor/dir_monitor.hpp"
#include "Singleton.h"

#include <boost/asio/io_service.hpp>
#include <memory>

namespace Ember {

/**
 * Observes directories for changes.
 */
class FileSystemObserver : public Singleton<FileSystemObserver> {

public:
	/**
	 * A file system event.
	 */
	struct FileSystemEvent {
		/**
		 * The directory monitoring event.
		 */
		const boost::asio::dir_monitor_event& ev;

		/**
		 * The path relative to the watched directory.
		 */
		const boost::filesystem::path relativePath;
	};

	explicit FileSystemObserver(boost::asio::io_service& ioService);

	~FileSystemObserver() override;

	/**
	 * Adds a directory recursively.
	 * @param dirname The path.
	 * @param callback A callback, called whenever a change is detected.
	 */
	void add_directory(const boost::filesystem::path& dirname, std::function<void(const FileSystemEvent&)> callback);

	/**
	 * Removes a previously watched directory.
	 * @param dirname
	 */
	void remove_directory(const boost::filesystem::path& dirname);

private:
	std::unique_ptr<boost::asio::dir_monitor> mDirectoryMonitor;

	std::map<boost::filesystem::path, std::function<void(const FileSystemEvent&)>> mCallBacks;

	void observe();
};
}


#endif //EMBER_FILESYSTEMOBSERVER_H
