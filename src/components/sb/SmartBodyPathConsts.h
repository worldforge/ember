/*
 Copyright (C) 2014 Céline NOËL <celine.noel.7294@gmail.com>

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

#ifndef SMARTBODYPATHCONSTS_H
#define SMARTBODYPATHCONSTS_H

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#define EMBER_SMARTBODY_NAMEPREFIX "SB"
#define EMBER_SMARTBODY_MEDIAPATH (EmberServices::getSingleton().getConfigService().getSharedMediaDirectory() + "/motions/")
#define EMBER_SMARTBODY_ASSETS_SKELETONS "skeletons/"
#define EMBER_SMARTBODY_ASSETS_LOCOMOTION "locomotion/"
#define EMBER_SMARTBODY_ASSETS_GESTURES "gestures/"
#define EMBER_SMARTBODY_ASSETS_MAPS (EMBER_SMARTBODY_MEDIAPATH + "maps/")
#define EMBER_SMARTBODY_MAP_EXTENSION ".map"
#define EMBER_SMARTBODY_SKELETON_EXTENSION ".xml"
#define EMBER_SMARTBODY_SKELETON_LOCOMOTION "Utah.sk"
#define EMBER_SMARTBODY_SKELETON_GESTURES "Brad.sk"

#endif //SMARTBODYPATHCONSTS_H