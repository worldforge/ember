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

#ifndef EMBER_SQUALLARCHIVE_H
#define EMBER_SQUALLARCHIVE_H

#include <squall/core/Iterator.h>
#include <OgreArchive.h>
#include <OgreArchiveFactory.h>
#include <regex>
#include <utility>

namespace Ember::OgreView {
class SquallArchive : public Ogre::Archive {
public:
	SquallArchive(Squall::Repository repository, Squall::Signature rootSignature);

	~SquallArchive() override = default;

	bool isCaseSensitive() const override;

	void load() override;

	void unload() override;

	Ogre::DataStreamPtr open(const Ogre::String& filename, bool readOnly) const override;

	Ogre::StringVectorPtr list(bool recursive, bool dirs) const override;

	Ogre::FileInfoListPtr listFileInfo(bool recursive, bool dirs) const override;

	Ogre::StringVectorPtr find(const Ogre::String& pattern, bool recursive, bool dirs) const override;

	bool exists(const Ogre::String& filename) const override;

	time_t getModifiedTime(const Ogre::String& filename) const override;

	Ogre::FileInfoListPtr findFileInfo(const Ogre::String& pattern, bool recursive, bool dirs) const override;

protected:
	Squall::Repository mRepository;
	Squall::Manifest mRootManifest;

	void findFiles(std::regex* pattern,
				   bool recursive,
				   bool dirs,
				   Ogre::StringVector* simpleList,
				   Ogre::FileInfoList* detailList) const;


};

struct SquallArchiveFactory : public Ogre::ArchiveFactory {
		Squall::Repository mRepository;

		explicit SquallArchiveFactory(Squall::Repository repository) : mRepository(std::move(repository)) {}

		~SquallArchiveFactory() override = default;

		const Ogre::String& getType() const override {
			static std::string name = "Squall";
			return name;
		}

		Ogre::Archive* createInstance(const Ogre::String& name, bool readOnly) override {
			return new SquallArchive(mRepository, Squall::Signature(name));
		}

		void destroyInstance(Ogre::Archive* arch) override { delete arch; }
	};
}

#endif //EMBER_SQUALLARCHIVE_H
