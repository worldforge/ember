#ifndef SMARTBODYPATHCONSTS_H
#define SMARTBODYPATHCONSTS_H

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#define EMBER_SMARTBODY_NAMEPREFIX "SB"
#define EMBER_SMARTBODY_MEDIAPATH (EmberServices::getSingleton().getConfigService().getSharedMediaDirectory() + "/motions")
#define EMBER_SMARTBODY_ASSETS_SKELETONS "skeletons/"
#define EMBER_SMARTBODY_ASSETS_LOCOMOTION "locomotion/"
#define EMBER_SMARTBODY_ASSETS_GESTURES "gestures/"
#define EMBER_SMARTBODY_ASSETS_MAPS (EMBER_SMARTBODY_MEDIAPATH + "maps/")
#define EMBER_SMARTBODY_MAP_EXTENSION ".map"
#define EMBER_SMARTBODY_SKELETON_EXTENSION ".xml"
#define EMBER_SMARTBODY_SKELETON_LOCOMOTION "Utah.sk"
#define EMBER_SMARTBODY_SKELETON_GESTURES "Brad.sk"

#endif //SMARTBODYPATHCONSTS_H