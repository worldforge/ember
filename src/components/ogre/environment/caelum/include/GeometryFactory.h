#ifndef GEOMETRYFACTORY_H
#define GEOMETRYFACTORY_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** A constructor for a number of caelum geometric objects.
	@author Jesús Alonso Abad.
	@version 0.1
 */
class DllExport GeometryFactory {
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
		static void generateSphericDome (const Ogre::String &name, const unsigned int segments, DomeType type = DT_GRADIENTS);

	private:
		/** Fills the vertex and index buffers for a sky gradients type dome.
			@param pVertex Pointer to the vertex buffer.
			@param pIndices Pointer to the index buffer.
			@param segments Subdivision detail.
		 */
		static void fillGradientsDomeBuffers (float *pVertex, unsigned short *pIndices, unsigned int segments);

		/** Fills the vertex and index buffers for a stardield type dome.
			@param pVertex Pointer to the vertex buffer.
			@param pIndices Pointer to the index buffer.
			@param segments Subdivision detail.
		 */
		static void fillStarfieldDomeBuffers (float *pVertex, unsigned short *pIndices, unsigned int segments);
};

} // namespace caelum

#endif //GEOMETRYFACTORY_H
