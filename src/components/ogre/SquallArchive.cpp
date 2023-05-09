/*
 Copyright (C) 2023 Erik Ogenvik

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

#include "SquallArchive.h"

#include <utility>
#include <boost/algorithm/string/replace.hpp>


namespace {
std::optional<std::filesystem::path> resolveFile(const Squall::Repository& repository, Squall::Manifest manifest, const std::filesystem::path& path) {
	auto activeManifest = manifest;

	std::vector<std::string> elements;
	for (const auto& subpath: path) {
		elements.emplace_back(subpath);
	}
	size_t i = 0;
	for (auto subpath: elements) {
		++i;
		auto manifestI = std::find_if(activeManifest.entries.begin(), activeManifest.entries.end(), [&subpath](const Squall::FileEntry& entry) {
			if (entry.type == Squall::FileEntryType::DIRECTORY) {
				return entry.fileName.substr(0, entry.fileName.length() - 1) == subpath;
			} else {
				return entry.fileName == subpath;
			}
		});
		if (manifestI != activeManifest.entries.end()) {
			if (manifestI->type == Squall::FileEntryType::DIRECTORY) {
				auto fetchResult = repository.fetchManifest(manifestI->signature);
				if (!fetchResult.manifest) {
					return {};
				} else {
					activeManifest = *fetchResult.manifest;
				}
			} else {
				if (i == elements.size()) {
					auto fetchResult = repository.fetch(manifestI->signature);
					if (fetchResult.status == Squall::FetchStatus::SUCCESS) {
						return {fetchResult.localPath};
					} else {
						return {};
					}
				} else {
					return {};
				}
			}
		} else {
			return {};
		}
	}

	return {};
}

std::unique_ptr<std::regex> regexFromPattern(const std::string& pattern) {
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

		return std::make_unique<std::regex>(patternEscaped);
	}
	return {};
}

}

namespace Ember::OgreView {

SquallArchive::SquallArchive(Squall::Repository repository, Squall::Signature rootSignature)
		: Archive(std::string("squall:/") + rootSignature.str(), "Squall"),
		  mRepository(std::move(repository)) {
	auto fetchResult = mRepository.fetchManifest(rootSignature);
	if (fetchResult.fetchResult.status == Squall::FetchStatus::FAILURE) {
		throw std::runtime_error("Could not fetch manifest from Squall.");
	}
	mRootManifest = *fetchResult.manifest;
}


bool SquallArchive::isCaseSensitive() const {
	return true;
}

void SquallArchive::load() {

}

void SquallArchive::unload() {

}


Ogre::DataStreamPtr SquallArchive::open(const Ogre::String& filename, bool) const {
	auto resolvedFile = resolveFile(mRepository, mRootManifest, filename);
	if (resolvedFile.has_value()) {
		// Always open in binary mode
		auto origStream = std::make_unique<std::ifstream>(*resolvedFile, std::ios::in | std::ios::binary);

		// Should check ensure open succeeded, in case fail for some reason.
		if (origStream->fail()) {
			OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "Cannot open file: " + filename, "SquallArchive::open");
		}
		auto size = file_size((*resolvedFile));
		// Construct return stream, tell it to delete on destroy
		auto stream = Ogre::DataStreamPtr(new Ogre::FileStreamDataStream(filename, origStream.get(), size, true));
		origStream.release();
		return stream;
	}
	return {};

}

Ogre::StringVectorPtr SquallArchive::list(bool recursive, bool dirs) const {
	auto result = std::make_shared<Ogre::StringVector>();
	findFiles(nullptr, recursive, dirs, result.get(), {});
	return result;
}

Ogre::FileInfoListPtr SquallArchive::listFileInfo(bool recursive, bool dirs) const {
	auto result = std::make_shared<Ogre::FileInfoList>();
	findFiles(nullptr, recursive, dirs, {}, result.get());
	return result;
}

Ogre::StringVectorPtr SquallArchive::find(const Ogre::String& pattern, bool recursive, bool dirs) const {
	auto result = std::make_shared<Ogre::StringVector>();
	findFiles(regexFromPattern(pattern).get(), recursive, dirs, result.get(), {});
	return result;
}

bool SquallArchive::exists(const Ogre::String& filename) const {
	auto resolvedFile = resolveFile(mRepository, mRootManifest, filename);
	return resolvedFile.has_value();
}

time_t SquallArchive::getModifiedTime(const Ogre::String& filename) const {
	auto resolvedFile = resolveFile(mRepository, mRootManifest, filename);
	if (resolvedFile) {
		return std::filesystem::last_write_time(*resolvedFile).time_since_epoch().count();
	} else {
		return 0;
	}
}

Ogre::FileInfoListPtr SquallArchive::findFileInfo(const Ogre::String& pattern, bool recursive, bool dirs) const {
	auto result = std::make_shared<Ogre::FileInfoList>();
	findFiles(regexFromPattern(pattern).get(), recursive, dirs, {}, result.get());
	return result;
}

void SquallArchive::findFiles(std::regex* pattern,
							  bool recursive,
							  bool dirs,
							  Ogre::StringVector* simpleList,
							  Ogre::FileInfoList* detailList) const {
	Squall::iterator it(mRepository, mRootManifest, recursive);
	for (; it != Squall::iterator(); ++it) {
		if (!dirs || (*it).fileEntry.type != Squall::FileEntryType::DIRECTORY) {
			if (!pattern || std::regex_match((*it).path.filename().string(), *pattern)) {
				if (simpleList) {
					simpleList->emplace_back((*it).path);
				}
				if (detailList) {
					Ogre::FileInfo fi;
					fi.archive = this;
					fi.filename = (*it).path;
					fi.basename = (*it).path.filename().generic_string();
					fi.path = (*it).path.parent_path();
					fi.compressedSize = (*it).fileEntry.size;
					fi.uncompressedSize = fi.compressedSize;
					detailList->emplace_back(std::move(fi));
				}
			}
		}
	}
}


}
