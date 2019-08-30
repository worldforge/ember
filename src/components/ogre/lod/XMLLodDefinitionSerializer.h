/*
 * Copyright (c) 2013 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef XMLLODDEFINITIONSERIALIZER_H
#define XMLLODDEFINITIONSERIALIZER_H

#include "OgreDataStream.h"
#include "LodDefinition.h"

#include <string>
#include <boost/filesystem/path.hpp>

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
	explicit XMLLodDefinitionSerializer(boost::filesystem::path exportDirectory);

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
	const boost::filesystem::path mExportDirectory;
};

}
}
}
#endif // ifndef XMLLODDEFINITIONSERIALIZER_H
