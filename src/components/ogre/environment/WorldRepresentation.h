/*
 Copyright (C) 2012 Erik Ogenvik

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

#ifndef WORLDREPRESENTATION_H_
#define WORLDREPRESENTATION_H_

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

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Represents the world, along with firmament.
 *
 * This representation is used to represent the world, along with any firmament.
 *
 */
class WorldRepresentation: public IGraphicalRepresentation, public virtual sigc::trackable
{
public:

	/**
	 * @brief Ctor.
	 * @param entity The entity which represents the world.
	 * @param environment The main environment instance.
	 */
	WorldRepresentation(EmberEntity& entity, Environment& environment);

	/**
	 * @brief Dtor.
	 */
	virtual ~WorldRepresentation();

	/**
	 * @brief Gets the unique type name for a subclass of this.
	 * The main purpose of this is to allow late binding casts, so that it's possible to more safely cast an instance into a concrete subclass.
	 */
	virtual const std::string& getType() const;

	/**
	 * @brief Gets the shared class type name, which is the same one returned through getType().
	 * This can be used to do lookups to see whether any instance of IGraphicalRepresentation if an instance of this class.
	 * @returns The string "WorldRepresentation".
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
	EmberEntity& mEntity;

	/**
	 * @brief The world environment.
	 */
	Environment& mEnvironment;

	/**
	 * @brief The type name for the class.
	 */
	static const std::string sTypeName;

};

}
}
}
#endif /* WORLDREPRESENTATION_H_ */
