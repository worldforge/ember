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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MODELDEFINITION_HELPER_H_
#define MODELDEFINITION_HELPER_H_
namespace {
int _ModelDefinition_getUseScaleOf(Ember::OgreView::Model::ModelDefinition* modelDefinition) {
	return static_cast<int>(modelDefinition->getUseScaleOf());
}

void _ModelDefinition_setUseScaleOf(Ember::OgreView::Model::ModelDefinition* modelDefinition, int useScaleOf) {
	modelDefinition->setUseScaleOf(static_cast<Ember::OgreView::Model::ModelDefinition::UseScaleOf>(useScaleOf));
}
}

#endif /* MODELDEFINITION_HELPER_H_ */
