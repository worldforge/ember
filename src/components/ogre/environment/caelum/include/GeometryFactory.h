/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2007 Caelum team. See Contributors.txt for details.

Caelum is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Caelum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Caelum. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GEOMETRYFACTORY_H
#define GEOMETRYFACTORY_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** A constructor for a number of caelum geometric objects.
	@author Jes�s Alonso Abad.
	@version 0.1
 */
class CAELUM_EXPORT GeometryFactory {
// Types and enums ------------------------------------------------------------
	public:
		/** Enumeration of types of sky domes.
		 */
		enum DomeType {DT_GRADIENTS, DT_STARFIELD};

// Methods --------------------------------------------------------------------
	public:
		/** Creates a longitude-latitude sky dome.
			@note Does nothing if the sphere already exists.
			@param name The name of the mesh to be created.
			@param segments The number of sphere segments.
			@param type The type of sky dome to create.
		 */
		static void generateSphericDome (const Ogre::String &name, int segments, DomeType type = DT_GRADIENTS);

	private:
		/** Fills the vertex and index buffers for a sky gradients type dome.
			@param pVertex Pointer to the vertex buffer.
			@param pIndices Pointer to the index buffer.
			@param segments Subdivision detail.
		 */
		static void fillGradientsDomeBuffers (float *pVertex, unsigned short *pIndices, int segments);

		/** Fills the vertex and index buffers for a stardield type dome.
			@param pVertex Pointer to the vertex buffer.
			@param pIndices Pointer to the index buffer.
			@param segments Subdivision detail.
		 */
		static void fillStarfieldDomeBuffers (float *pVertex, unsigned short *pIndices, int segments);
};

} // namespace caelum

#endif //GEOMETRYFACTORY_H
