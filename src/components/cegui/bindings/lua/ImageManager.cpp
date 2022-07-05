/*
 Copyright (C) 2022 Erik Ogenvik

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
#include "LuaFunctor.h"

using namespace CEGUI;
template <>
void registerLua<ImageManager>(sol::table& space) {
	auto imagePair = space.new_usertype<ImageManager::ImagePair>("ImagePair", sol::no_constructor);
	imagePair["first"] = &ImageManager::ImagePair::first;
	imagePair["second"] = &ImageManager::ImagePair::second;

	auto imageManager = space.new_usertype<ImageManager>("ImageManager", sol::no_constructor);
	imageManager["getSingleton"] = &ImageManager::getSingleton;
//	imageManager["getIterator"] = [](ImageManager* self) { return std::unique_ptr<ImageManager::ImageIterator>(new ImageManager::ImageIterator(self->getIterator())); };
	imageManager["getIterator"] = &ImageManager::getIterator;
	imageManager["get"] = string_setter(&ImageManager::get);

	auto imageIterator = space.new_usertype<ImageManager::ImageIterator>("ImageIterator", sol::no_constructor);
	imageIterator["key"] = string_getter(&ImageManager::ImageIterator::getCurrentKey);
	imageIterator["value"] = &ImageManager::ImageIterator::getCurrentValue;
	imageIterator["isAtEnd"] = &ImageManager::ImageIterator::isAtEnd;
	imageIterator["isAtStart"] = &ImageManager::ImageIterator::isAtStart;
	imageIterator["next"] = [](ImageManager::ImageIterator* self) {
		(*self)++;
		return self;
	};
	imageIterator["previous"] = [](ImageManager::ImageIterator* self) {
		(*self)--;
		return self;
	};
	imageIterator["toStart"] = &ImageManager::ImageIterator::toStart;
	imageIterator["toEnd"] = &ImageManager::ImageIterator::toEnd;

	auto image = space.new_usertype<Image>("Image", sol::no_constructor);
	image["getName"] = [](Image* self) { return std::string(self->getName().c_str()); };
}