/*
 * Copyright (C) 2012 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef XMLMODELDEFINITIONSERIALIZER_H
#define XMLMODELDEFINITIONSERIALIZER_H

#include "OgreDataStream.h"
#include "LodDefinition.h"

#include <string>

namespace Ember
{
namespace OgreView
{
namespace Lod
{

/**
 * @brief Helper class to import and export LodDefinition files.
 */
class XMLLodDefinitionSerializer
{
public:

	/**
	 * @brief Ctor.
	 * @param exportDirectory Specifies the directory, where the changed *.loddef files will be saved.
	 */
	XMLLodDefinitionSerializer(const std::string& exportDirectory);

	/**
	 * @brief Reads the DataStream and configures the passed LodDefinition.
	 * @param stream The DataStream containing XML data.(input)
	 * @param lodDef The Lod definition to configure.(output)
	 */
	void importLodDefinition(const Ogre::DataStreamPtr& stream, LodDefinition& lodDef) const;

	/**
	 * @brief Exports the Lod definition to a file.
	 * @param modelDef The Lod definition to export.
	 * @param filename The name of the file to which the script will be written with extension.
	 *                 This is usually equal to the mesh name + .loddef.
	 * @return True if the script was successfully written.
	 */
	bool exportScript(const LodDefinitionPtr& lodDef, const std::string& fileName) const;
private:
	const std::string mExportDirectory;
};

}
}
}
#endif // ifndef XMLMODELDEFINITIONSERIALIZER_H
