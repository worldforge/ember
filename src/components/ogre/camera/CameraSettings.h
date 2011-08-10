/*
 Copyright (C) 2011 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef CAMERASETTINGS_H_
#define CAMERASETTINGS_H_

#include "services/config/ConfigListenerContainer.h"

namespace Ember
{
namespace OgreView
{
namespace Camera
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Shared camera settings.
 *
 * An instance of this is shared between multiple camera mounts and other camera functionality to handle common settings.
 */
class CameraSettings : ConfigListenerContainer
{
public:

	/**
	 * @brief Ctor.
	 */
	CameraSettings();

	/**
	 * @brief Dtor.
	 */
	virtual ~CameraSettings();

	/**
	 * @brief Accessor for the number of degrees to move the camera per mouse unit.
	 * One mouse unit represents one width or height of the screen, depending on the axis of movement.
	 * @return Number of degrees to move the camera per mouse unit.
	 */
	float getDegreesPerMouseUnit() const;


	/**
	 * @brief Accessor for whether the vertical movement of the camera should be inverted.
	 * This is useful for people used to flight simulators.
	 * @return True if the vertical movement of the camera should be inverted.
	 */
	float getInvertCamera() const;

private:

	/**
	 * @brief The current number of degrees to move the camera per mouse unit.
	 */
	float mDegreesPerMouseUnit;

	/**
	 * @brief If true, the vertical movement of the camera will be inverted.
	 *
	 * This is useful for people used to flight simulators.
	 */
	bool mInvertCamera;

	/**
	 * @brief Sets the degrees per mouse unit.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_DegreesPerMouseUnit(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * @brief Sets whether the vertical axis of the camera should be inverted.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_InvertCamera(const std::string& section, const std::string& key, varconf::Variable& variable);

};


inline float CameraSettings::getDegreesPerMouseUnit() const
{
	return mDegreesPerMouseUnit;
}

inline float CameraSettings::getInvertCamera() const
{
	return mInvertCamera;
}

}
}
}

#endif /* CAMERASETTINGS_H_ */
