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

#ifndef OCEANREPRESENTATION_H_
#define OCEANREPRESENTATION_H_

#include "domain/IGraphicalRepresentation.h"
#include <sigc++/trackable.h>

namespace Ember
{
class EmberEntity;
namespace OgreView
{
namespace Environment
{

class Environment;
class IWater;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief Represents an entity as the ocean.
 *
 * The ocean is a little special, so we can't use the normal ModelRepresentation for it. Instead this class will bind an entity to an IWater instance.
 * Currently the only logic there is is to alter the level of the water depending on the vertical position of the entity.
 */
class OceanRepresentation: public IGraphicalRepresentation, public virtual sigc::trackable
{
public:
	OceanRepresentation(EmberEntity& entity, Environment& environment);
	virtual ~OceanRepresentation();

	/**
	 * @brief Gets the unique type name for a subclass of this.
	 * The main purpose of this is to allow late binding casts, so that it's possible to more safely cast an instance into a concrete subclass.
	 */
	virtual const std::string& getType() const;

	/**
	 * @brief Gets the shared class type name, which is the same one returned through getType().
	 * This can be used to do lookups to see whether any instance of IGraphicalRepresentation if an instance of this class.
	 * @returns The string "OceanRepresentation".
	 */
	static const std::string& getTypeNameForClass();

	/**
	 * General method for turning on and off debug visualizations. Subclasses might support more types of visualizations than the ones defined here.
	 * @param visualization The type of visualization. Currently supports "OgreBBox".
	 * @param visualize Whether to visualize or not.
	 */
	virtual void setVisualize(const std::string& visualization, bool visualize);

	/**
	 * @brief Gets whether a certain visualization is turned on or off.
	 * @param visualization The type of visualization. Currently supports "OgreBBox".
	 * @return true if visualization is turned on, else false
	 */
	virtual bool getVisualize(const std::string& visualization) const;

private:
	/**
	 * @brief The entity which this representation is bound to.
	 */
	EmberEntity & mEntity;

	/**
	 * @brief The world environment.
	 */
	Environment& mEnvironment;

	/**
	 * @brief The type name for the class.
	 */
	static const std::string sTypeName;

	void entity_Moved();

	/**
	 * @brief Updates the height of the water level from the vertical position of the entity.
	 */
	void updateWaterPosition();

};
}
}

}

#endif /* OCEANREPRESENTATION_H_ */
