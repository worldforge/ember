/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

#ifndef MODELMANAGER_H_
#define MODELMANAGER_H_

#include "framework/Singleton.h"

namespace Ember {
class EmberEntity;
namespace OgreView {
namespace Model {

class Model;
class ModelRepresentation;

class ModelRepresentationManager : public Singleton<ModelRepresentationManager>
{
public:
	ModelRepresentationManager();

	~ModelRepresentationManager() override;

	Model* getModelForEntity(EmberEntity& entity) const;

	ModelRepresentation* getRepresentationForEntity(EmberEntity& entity) const;
};

}
}

}

#endif /* MODELMANAGER_H_ */
