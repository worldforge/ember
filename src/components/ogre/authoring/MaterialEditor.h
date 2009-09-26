//
// C++ Interface: MaterialEditor
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGREMATERIALEDITOR_H
#define EMBEROGREMATERIALEDITOR_H

#include <sigc++/trackable.h>

#include "../EmberOgrePrerequisites.h"

#include "framework/ConsoleObject.h"

namespace EmberOgre
{
namespace Authoring
{
/**
 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class MaterialEditor: public sigc::trackable, public Ember::ConsoleObject
{
public:
	MaterialEditor();

	~MaterialEditor();

	//updateMaterial(const std::string& materialName, const std::string& );
	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	virtual void runCommand(const std::string &command, const std::string &args);

	const Ember::ConsoleCommandWrapper AlterMaterial;

};

}
}

#endif
