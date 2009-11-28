/*
 * ShaderNormalMappedPassCoverageBatch.h
 *
 *  Created on: 26 nov 2009
 *      Author: erik
 */

#ifndef EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPEDPASSCOVERAGEBATCH_H_
#define EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPEDPASSCOVERAGEBATCH_H_

#include "ShaderPassCoverageBatch.h"

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

class ShaderNormalMappedPassCoverageBatch : public ShaderPassCoverageBatch
{
public:
	ShaderNormalMappedPassCoverageBatch(ShaderPass& shaderPass, Ogre::TexturePtr combinedCoverageTexture);
	virtual ~ShaderNormalMappedPassCoverageBatch() {}

	virtual void finalize();

protected:

};

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPEDPASSCOVERAGEBATCH_H_ */
