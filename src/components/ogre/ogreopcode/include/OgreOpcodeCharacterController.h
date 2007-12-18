///////////////////////////////////////////////////////////////////////////////
///  @file OgreOpcodeCharacterController.h
///  @brief 
///
///  @author The OgreOpcode Team
///  
///////////////////////////////////////////////////////////////////////////////
///  
///  This file is part of OgreOpcode.
///  
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
///  
///  OgreOpcode is free software; you can redistribute it and/or
///  modify it under the terms of the GNU General Public
///  License as published by the Free Software Foundation; either
///  version 2.1 of the License, or (at your option) any later version.
///  
///  OgreOpcode is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
///  Lesser General Public License for more details.
///  
///  You should have received a copy of the GNU General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///  
///////////////////////////////////////////////////////////////////////////////
#ifndef OGREOPCODECHARACTERCONTROLLER_H
#define OGREOPCODECHARACTERCONTROLLER_H

#include "OgreOpcode.h"
#include "Ogre.h"

namespace OgreOpcode
{
	class _OgreOpcode_Export CharacterController
	{
	public:
		CharacterController(Ogre::SceneNode* CharacterBaseNode, Ogre::Real CharacterRadius, Ogre::Real CharacterHeight, Ogre::Vector3 Gravity = Ogre::Vector3(0,-9.8,0), Ogre::Degree SlopeThreshold = Ogre::Degree(60), Ogre::Real UnitsPerMeter = 1.0);
		~CharacterController();

		struct CollisionPacket 
		{
			Ogre::Vector3 eRadius; // ellipsoid radius
			// Information about the move being requested: (in R3)
			Ogre::Vector3 R3Velocity;
			Ogre::Vector3 R3Position;
			// Information about the move being requested: (in eSpace)
			Ogre::Vector3 velocity;
			Ogre::Vector3 normalizedVelocity;
			Ogre::Vector3 basePoint;
			// Hit information
			bool foundCollision;
			Ogre::Real nearestDistance;
			Ogre::Vector3 intersectionPoint;
		};

		Ogre::SceneNode* getBaseNode();
		Ogre::Real getHeight();
		Ogre::Real getRadius();
		Ogre::Vector3 getGravity();
		Ogre::Degree getSlopeThreshold();

		void move(Ogre::Vector3 velocity);

		void setHeight(Ogre::Real Height);
		void setRadius(Ogre::Real Radius);
		void setGravity(const Ogre::Vector3& Gravity);
		void setSlopeThreshold(const Ogre::Degree& SlopeThreshold);

	protected:
		Ogre::SceneNode* mBaseNode;
		// radius of the capsule shape representing the character.
		Ogre::Real mRadius;
		// height of the capsule shape representing the character.
		Ogre::Real mHeight;
		// Force applied to character at all times. 
		// (optimized to not apply to characters at rest)
		Ogre::Vector3 mGravity;
		// The maximum slope the character will slide against.
		Ogre::Degree mSlopeThreshold;
		// used to derive close distances between objects.
		Ogre::Real mUnitsPerMeter;
		Ogre::Real mVeryCloseDistance;

		Ogre::String mContactName;

	private:
		/** Collides a sphere with world.
		@param pos
			The colliding sphere position.
		@param vel
			The colliding sphere velocity.
		@param radius
			The colliding sphere radius.
		@param gravity
			The amount of gravity in the world.
		@param slopeSlideThresold
			(optional) The steepness thresold for sliding.
		@return
			New position for collided sphere.
		*/
		Ogre::Vector3 _collideAndSlide(const Ogre::Vector3& vel);
		Ogre::Vector3 _collideWithWorld(int recursionDepth, const Ogre::Vector3& pos, const Ogre::Vector3& vel, CollisionPacket& colData, bool gravityStep, const Ogre::Degree& slopeSlideThresold);
		void _doOgreOpcodeCollision(CollisionPacket& colData, float sweepOffset);
	};
}

#endif
