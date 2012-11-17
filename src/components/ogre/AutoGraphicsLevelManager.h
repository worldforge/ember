/*
 * Copyright (C) 2012 Arjun Kumar <arjun1991@gmail.com>
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

#include "GraphicalChangeAdapter.h"

#include "OgreIncludes.h"

#include <sigc++/signal.h>
#include <sigc++/connection.h>
#include <sigc++/trackable.h>

#include <string>

#include <boost/circular_buffer.hpp>
#include <boost/date_time.hpp>

namespace varconf
{
class Variable;
}

namespace Ember
{
class TimeFrame;

class MainLoopController;
class ConfigListenerContainer;
namespace OgreView
{

class GraphicalChangeAdapter;

/**
 * @brief Records the average time per frame.
 * 
 */
class FrameTimeRecorder: public virtual sigc::trackable
{
public:
	/**
	 * @brief Constructor
	 */
	FrameTimeRecorder(MainLoopController& mainLoopController);

	/**
	 * @brief Destructor
	 */
	virtual ~FrameTimeRecorder();

	/**
	 * @brief Signal sent out with the updated average time per frame.
	 */
	sigc::signal<void, const boost::posix_time::time_duration> EventAverageTimePerFrameUpdated;

protected:

	/**
	 * The amount of time in microseconds that the fps should be averaged over.
	 */
	boost::posix_time::time_duration mRequiredTimeSamples;

	/**
	 * Stores averaged time frames.
	 */
	boost::circular_buffer<boost::posix_time::time_duration> mTimePerFrameStore;

	/**
	 * @brief Accumulates frame times since last calculation.
	 */
	boost::posix_time::time_duration mAccumulatedFrameTimes;

	/**
	 * @brief Accumulates number of frames since last calculation.
	 */
	int mAccumulatedFrames;

	void frameCompleted(const TimeFrame& timeFrame, unsigned int frameActionMask);

};

/**
 *@brief Central class for automatic adjustment of graphics level
 *
 * This class maintains a current Graphics level. It connects to the fpsUpdated signal and thus 
 * checks the fps for a significant increase or decrease and then asks for a change in the level
 * by using the GraphicalChangeAdapter.
 */

class AutomaticGraphicsLevelManager
{
public:
	/**
	 * @brief Constructor
	 * @param mainLoopController The main loop controller.
	 */
	AutomaticGraphicsLevelManager(MainLoopController& mainLoopController);

	/**
	 * @brief Destructor
	 */
	~AutomaticGraphicsLevelManager();

	/**
	 * @brief Sets whether automatic adjustment is enabled
	 */
	void setEnabled(bool newEnabled);

	/**
	 * @brief Sets the FPS that the component tries to achieve.
	 * @param fps The fps that the manager tries to achieve.
	 */
	void setFps(float fps);

	/**
	 * @brief Used to check if automatic adjustment is enabled
	 */
	bool isEnabled() const;

	/**
	 * @brief Used to trigger a change in graphics level
	 * @param changeInFpsRequired Used to pass how much of a change in fps is required, positive for an increase in fps, negative for a decrease in fps
	 */
	void changeGraphicsLevel(float changeInFpsRequired);

	/**
	 * @brief Used to access the instance GraphicalChangeAdapter owned by this class.
	 */
	GraphicalChangeAdapter& getGraphicalAdapter();

protected:
	/**
	 * The fps this module will try to achieve once enabled
	 */
	float mDefaultFps;

	/**
	 * Boolean that holds whether automatic adjustment is enabled.
	 */
	bool mEnabled;

	/**
	 * Instance of FpsUpdater class owned by this class to get updates on when the fps is updated.
	 */
	FrameTimeRecorder mFrameTimeRecorder;

	/**
	 * The interface through which this central class communicates with the graphical subcomponents.
	 */
	GraphicalChangeAdapter mGraphicalChangeAdapter;

	/**
	 * @brief Used to listen for configuration changes.
	 */
	ConfigListenerContainer* mConfigListenerContainer;

	/**
	 * @brief The connection through which the automatic graphics manager listens for fps updates.
	 */
	sigc::connection mFpsUpdatedConnection;

	/**
	 * @brief This function is used to check if the fps is optimum, higher or lower as compared to mDefaultFps.
	 */
	void checkFps(float);

	/**
	 * Called from the FrameTimeRecorder when a new average time per frame has been calculated.
	 * @param timePerFrame Time per frame, in microseconds.
	 */
	void averageTimePerFrameUpdated(const boost::posix_time::time_duration timePerFrame);

	/**
	 * @brief Connected to the config service to listen for derired fps settings.
	 */
	void Config_DefaultFps(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * @brief Connected to the config service to listen for whether the automatic graphics manager should be enabled.
	 */
	void Config_Enabled(const std::string& section, const std::string& key, varconf::Variable& variable);

};

}
}
