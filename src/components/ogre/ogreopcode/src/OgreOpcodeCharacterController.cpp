#include "OgreOpcodeCharacterController.h"

namespace OgreOpcode
{
	CharacterController::CharacterController(Ogre::SceneNode* CharacterBaseNode, Ogre::Real CharacterRadius, Ogre::Real CharacterHeight, Ogre::Vector3 Gravity, Ogre::Degree SlopeThreshold, Ogre::Real UnitsPerMeter) :
		mBaseNode(CharacterBaseNode),
		mRadius(CharacterRadius),
		mHeight(CharacterHeight),
		mGravity(Gravity),
		mSlopeThreshold(SlopeThreshold),
		mUnitsPerMeter(UnitsPerMeter),
		mContactName("")
	{
		mVeryCloseDistance = ((mUnitsPerMeter / 100) * 0.005) * 5; // 25 centimeters in Ogre 3d coordinates
	}

	CharacterController::~CharacterController()
	{
	}

	Ogre::Vector3 CharacterController::_collideAndSlide(const Ogre::Vector3& vel)
	{
		CollisionPacket colData;

		// Information about the move being requested: (in R3)
		colData.R3Position = mBaseNode->_getDerivedPosition();
		colData.R3Velocity = vel;

		// convert simple sphere radius into ellipsoid radius
		// (algorithm works with ellipsoids but for now ogreopcode works only with spheres)
		colData.eRadius = Ogre::Vector3(mRadius, mRadius, mRadius);

		// calculate position and velocity in eSpace
		Ogre::Vector3 eSpacePosition = colData.R3Position / colData.eRadius;
		Ogre::Vector3 eSpaceVelocity = colData.R3Velocity / colData.eRadius;

		// Iterate until we have our final position.
		Ogre::Vector3 finalPosition = _collideWithWorld(0, eSpacePosition, eSpaceVelocity, colData, false, Ogre::Degree(0));

		// Add gravity pull:
		if (mGravity != Ogre::Vector3::ZERO) {
			// Set the new R3 position (convert back from eSpace to R3)
			colData.R3Position = finalPosition * colData.eRadius;
			colData.R3Velocity = mGravity;
			eSpacePosition = colData.R3Position / colData.eRadius;
			eSpaceVelocity = mGravity / colData.eRadius;
			finalPosition = _collideWithWorld(0, eSpacePosition, eSpaceVelocity, colData, true, mSlopeThreshold);
		}

		// Convert final result back to R3:
		return finalPosition * colData.eRadius;
	}

	Ogre::Vector3 CharacterController::_collideWithWorld(int recursionDepth, const Ogre::Vector3& pos, const Ogre::Vector3& vel, CollisionPacket& colData, bool gravityStep, const Ogre::Degree& slopeSlideThresold)
	{
		// do we need to worry?
		if (recursionDepth > 5) 
			return pos;

		// Ok, we need to worry:
		colData.velocity = vel;
		colData.normalizedVelocity = vel;
		colData.normalizedVelocity.normalise();
		colData.basePoint = pos;
		colData.foundCollision = false;

		// ----------------------------
		// OgreOpcode part begin
		_doOgreOpcodeCollision(colData, mVeryCloseDistance);
		// OgreOpcode part end
		// ----------------------------

		// If no collision we just move along the velocity
		if (colData.foundCollision == false) {
			return pos + vel;
		}

		// *** Collision occured ***
		// The original destination point
		Ogre::Vector3 destinationPoint = pos + vel;
		Ogre::Vector3 newBasePoint = pos;

		// only update if we are not already very close
		// and if so we only move very close to intersection..not
		// to the exact spot.
		if (colData.nearestDistance >= mVeryCloseDistance) {
			Ogre::Vector3 V = vel;
			V.normalise();
			V = V * (colData.nearestDistance - mVeryCloseDistance);
			newBasePoint = colData.basePoint + V;
			// Adjust polygon intersection point (so sliding
			// plane will be unaffected by the fact that we
			// move slightly less than collision tells us)
			V.normalise();
			colData.intersectionPoint -= mVeryCloseDistance * V;
		}

		// Determine the sliding plane
		Ogre::Vector3 slidePlaneOrigin = colData.intersectionPoint;
		Ogre::Vector3 slidePlaneNormal = newBasePoint - colData.intersectionPoint;
		slidePlaneNormal.normalise();
		Ogre::Plane slidingPlane(slidePlaneNormal, slidePlaneOrigin);

		Ogre::Vector3 newDestinationPoint = destinationPoint - 
			slidingPlane.getDistance(destinationPoint) *
			slidePlaneNormal;

		// Generate the slide vector, which will become our new
		// velocity vector for the next iteration
		Ogre::Vector3 newVelocityVector = newDestinationPoint - colData.intersectionPoint;

		// Recurse:
		// dont recurse if the new velocity is very small
		if (newVelocityVector.length() < mVeryCloseDistance) {
			return newBasePoint;
		}

		// simulate "friction"
		if (gravityStep) {
			// apply gravity only if slope is steep enough
			const Ogre::Radian tolerance = Ogre::Radian(slopeSlideThresold);
			Ogre::Vector3 gravity = vel;
			gravity.normalise();
			if (slidePlaneNormal.directionEquals(-gravity, tolerance)) {
				return newBasePoint;
			}
		}

		return _collideWithWorld(recursionDepth++, newBasePoint, newVelocityVector, colData, gravityStep, slopeSlideThresold);
	}

	void CharacterController::_doOgreOpcodeCollision(CollisionPacket& colData, float sweepOffset)
	{
		Ogre::Vector3 pos_R3 = colData.basePoint * colData.eRadius;
		Ogre::Vector3 vel_R3 = colData.velocity * colData.eRadius;
		
		OgreOpcode::CollisionPair** reports;

		// TODO: sweptSphereCheck does not support ellipsoids, 
		// so we must use only one dimension!!!
		Ogre::Real radius = colData.eRadius.x;

		// Add a little offset to velocity so that we don't get too close.
		Ogre::Vector3 offset = vel_R3;
		offset.normalise();
		offset *= sweepOffset;

		OgreOpcode::Details::CollisionClass collClass = OgreOpcode::COLLTYPE_ALWAYS_EXACT;

		int count = OgreOpcode::CollisionManager::getSingletonPtr()->getDefaultContext()->
			sweptSphereCheck(pos_R3, vel_R3 + offset, radius, collClass, reports);

		if (count) 
		{
			// search for closest distance collision
			int closest = 0;
			Ogre::Real d = reports[0]->distance;
			for (int i = 1; i < count; i++) {
				if (reports[i]->distance < d) {
					d = reports[i]->distance;
					closest = i;
				}
			}

			colData.foundCollision = true;
			colData.nearestDistance = reports[closest]->distance;
			colData.intersectionPoint = reports[closest]->contact / colData.eRadius;

			mContactName = reports[closest]->other_object->getName();
		}
	}

	Ogre::SceneNode* CharacterController::getBaseNode()
	{
		return mBaseNode;
	}

	Ogre::Real CharacterController::getHeight()
	{
		return mHeight;
	}

	Ogre::Real CharacterController::getRadius()
	{
		return mRadius;
	}

	Ogre::Vector3 CharacterController::getGravity()
	{
		return mGravity;
	}

	Ogre::Degree CharacterController::getSlopeThreshold()
	{
		return mSlopeThreshold;
	}

	void CharacterController::move(Ogre::Vector3 velocity)
	{
		mBaseNode->setPosition(_collideAndSlide(velocity));
	}

	void CharacterController::setHeight(Ogre::Real Height)
	{
		mHeight = Height;
	}

	void CharacterController::setRadius(Ogre::Real Radius)
	{
		mRadius = Radius;
	}

	void CharacterController::setGravity(const Ogre::Vector3& Gravity)
	{
		mGravity = Gravity;
	}

	void CharacterController::setSlopeThreshold(const Ogre::Degree& SlopeThreshold)
	{
		mSlopeThreshold = SlopeThreshold;
	}
}
