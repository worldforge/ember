/*
-----------------------------------------------------------------------------
This source file is based on source files that are part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Copyright (c) 2008 Erik Ogenvik <erik@ogenvik.org>
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA., or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/

#include "EmberOgreFileSystem.h"
#include <OgreFileSystem.h>
#include <OgreLogManager.h>
#include <boost/algorithm/string/replace.hpp>

using namespace Ogre;

namespace {

boost::filesystem::path concatenate_path(const boost::filesystem::path& base, const boost::filesystem::path& name) {
	if (base.empty() || name.is_absolute()) {
		return name;
	} else {
		return base / name;
	}
}

}

namespace Ember {
namespace OgreView {
//-----------------------------------------------------------------------
FileSystemArchive::FileSystemArchive(const String& name, const String& archType)
		: Archive(name, archType), mBaseName(name) {
}

//-----------------------------------------------------------------------
bool FileSystemArchive::isCaseSensitive() const {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	return false;
#else
	return true;
#endif

}

void FileSystemArchive::findFiles(String pattern, bool recursive,
								  bool dirs, StringVector* simpleList, FileInfoList* detailList) const {

	std::unique_ptr<std::regex> regex = nullptr;

	// pattern can contain a directory name, separate it from mask
	size_t pos1 = pattern.rfind('/');
	size_t pos2 = pattern.rfind('\\');
	if (pos1 == String::npos || ((pos2 != String::npos) && (pos1 < pos2)))
		pos1 = pos2;
	String directory;
	if (pos1 != String::npos) {
		directory = pattern.substr(0, pos1 + 1);
		pattern = pattern.substr(pos1 + 1);
	}

	auto base_dir = mBaseName;
	if (!directory.empty()) {
		base_dir = concatenate_path(mBaseName, directory);
	}

	if (pattern != "*") {
		auto patternEscaped = pattern;
		boost::replace_all(patternEscaped, "\\", "\\\\");
		boost::replace_all(patternEscaped, "^", "\\^");
		boost::replace_all(patternEscaped, ".", "\\.");
		boost::replace_all(patternEscaped, "$", "\\$");
		boost::replace_all(patternEscaped, "|", "\\|");
		boost::replace_all(patternEscaped, "(", "\\(");
		boost::replace_all(patternEscaped, ")", "\\)");
		boost::replace_all(patternEscaped, "[", "\\[");
		boost::replace_all(patternEscaped, "]", "\\]");
		boost::replace_all(patternEscaped, "*", "\\*");
		boost::replace_all(patternEscaped, "+", "\\+");
		boost::replace_all(patternEscaped, "?", "\\?");
		boost::replace_all(patternEscaped, "/", "\\/");
		boost::replace_all(patternEscaped, "{", "\\{");
		boost::replace_all(patternEscaped, "}", "\\}");

		boost::replace_all(patternEscaped, "\\?", ".");
		boost::replace_all(patternEscaped, "\\*", ".*");

		regex = std::make_unique<std::regex>(patternEscaped);
	}

	if (boost::filesystem::exists(base_dir)) {
		findFiles(base_dir, regex, recursive, dirs, simpleList, detailList);
	}

}

//-----------------------------------------------------------------------
void FileSystemArchive::findFiles(const boost::filesystem::path& directory,
								  const std::unique_ptr<std::regex>& pattern,
								  bool recursive,
								  bool dirs, StringVector* simpleList, FileInfoList* detailList) const {

	//if there's a file with the name "norecurse" we shouldn't recurse further
	if (boost::filesystem::exists(directory / "norecurse")) {
		return;
	}

	boost::filesystem::directory_iterator it(directory);
	boost::filesystem::directory_iterator end{};
	while (it != end) {
		if (boost::filesystem::is_directory(it->path())) {
			if (recursive) {
				if (it->path().filename().string() != "source") {
					findFiles(it->path(), pattern, recursive, dirs, simpleList, detailList);
				}
			}
		} else {
			if (!pattern || std::regex_match(it->path().filename().string(), *pattern)) {
				if (simpleList) {
					simpleList->emplace_back(boost::filesystem::relative(it->path(), mBaseName).generic_string());
				} else if (detailList) {
					auto relativePath = boost::filesystem::relative(it->path(), mBaseName);
					auto parentPath = relativePath.parent_path();
					FileInfo fi;
					fi.archive = this;
					fi.filename = relativePath.generic_string();
					fi.basename = it->path().filename().generic_string();
					fi.path = parentPath.empty() ? "" : parentPath.generic_string() + "/";
					fi.compressedSize = boost::filesystem::file_size(it->path());
					fi.uncompressedSize = fi.compressedSize;
					detailList->emplace_back(std::move(fi));
				}
			}
		}
		it++;
	}

}

//-----------------------------------------------------------------------
FileSystemArchive::~FileSystemArchive() {
	unload();
}

//-----------------------------------------------------------------------
void FileSystemArchive::load() {
	// do nothing here, what has to be said will be said later
}

//-----------------------------------------------------------------------
void FileSystemArchive::unload() {
	// nothing to see here, move along
}

//-----------------------------------------------------------------------
DataStreamPtr FileSystemArchive::open(const String& filename, bool readOnly) const {
	auto full_path = concatenate_path(mName, filename);

	// Always open in binary mode
	auto* origStream = OGRE_NEW_T(std::ifstream, MEMCATEGORY_GENERAL)();
	origStream->open(full_path.c_str(), std::ios::in | std::ios::binary);

	// Should check ensure open succeeded, in case fail for some reason.
	if (origStream->fail()) {
		OGRE_DELETE_T(origStream, basic_ifstream, MEMCATEGORY_GENERAL);
		OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND,
					"Cannot open file: " + filename,
					"FileSystemArchive::open");
	}

	auto size = boost::filesystem::file_size(full_path);
	// Construct return stream, tell it to delete on destroy
	auto* stream = OGRE_NEW FileStreamDataStream(filename,
												 origStream, size, true);
	return DataStreamPtr(stream);
}

//-----------------------------------------------------------------------
StringVectorPtr FileSystemArchive::list(bool recursive, bool dirs) const {
	// directory change requires locking due to saved returns
	StringVectorPtr ret(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

	findFiles("*", recursive, dirs, ret.get(), nullptr);

	return ret;
}

//-----------------------------------------------------------------------
FileInfoListPtr FileSystemArchive::listFileInfo(bool recursive, bool dirs) const {
	FileInfoListPtr ret(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

	findFiles("*", recursive, dirs, nullptr, ret.get());

	return ret;
}

//-----------------------------------------------------------------------
StringVectorPtr FileSystemArchive::find(const String& pattern,
										bool recursive, bool dirs) const {
	StringVectorPtr ret(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

	findFiles(pattern, recursive, dirs, ret.get(), nullptr);

	return ret;

}

//-----------------------------------------------------------------------
FileInfoListPtr FileSystemArchive::findFileInfo(const String& pattern,
												bool recursive, bool dirs) const {
	FileInfoListPtr ret(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

	findFiles(pattern, recursive, dirs, nullptr, ret.get());

	return ret;
}

//-----------------------------------------------------------------------
bool FileSystemArchive::exists(const String& filename) const {
	auto full_path = concatenate_path(mBaseName, filename);
	return boost::filesystem::exists(full_path);
}

time_t FileSystemArchive::getModifiedTime(const String& filename) const {
	auto full_path = concatenate_path(mBaseName, filename);
	return boost::filesystem::last_write_time(full_path);
}

//-----------------------------------------------------------------------
const String& FileSystemArchiveFactory::getType() const {
	static String name = "EmberFileSystem";
	return name;
}

}
}
