/**
* \file me_ct_breathing_interface.h
* \brief The breathing controller interface and breath cycle classes
*
* Part of Motion Engine and SmartBody-lib.
* Copyright (C) 2008  University of Southern California
*
*  SmartBody-lib is free software: you can redistribute it and/or
*  modify it under the terms of the Lesser GNU General Public License
*  as published by the Free Software Foundation, version 3 of the
*  license.
*
*  SmartBody-lib is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Lesser GNU General Public License for more details.
*
*  You should have received a copy of the Lesser GNU General Public
*  License along with SmartBody-lib.  If not, see:
*      http://www.gnu.org/licenses/lgpl-3.0.txt
*
*  CONTRIBUTORS:
*   - Celso M. de Melo, demelo@usc.edu, USC
*
* \author University of Southern California
* \author Institute for Creative Technologies, USC
*/


# ifndef ME_CT_BREATHING_INTERFACE_H
# define ME_CT_BREATHING_INTERFACE_H

# include <sk/sk_motion.h>
# include <sbm/gwiz_math.h>
#include <controllers/me_spline_1d.hpp>

#include <vector>
using std::vector;
#include <algorithm>

class BreathLayer;
class BreathCycle;
class NumericalInterpolationKey;

/**
* \brief The breathing controller interface
*
* Defines a set of breathing functions which can be implemented by different
* breathing models (e.g., skeleton animation or physical simulation of the 
* breathing organs). 
*
* The interface is characterized by:
* - a stack of breathing cycles. This is based on the idea of layers. Each 
* layer executes a breathing cycle for a certain number of cycles. The actual
* animation is defined by the topmost layer;
* - general breathing parameters such as breaths-per-minute.
*/
class MeCtBreathingInterface
{ 
protected:
	/**
	* \brief The constructor
	*/
	MeCtBreathingInterface(){}

public :
	/**
	* \brief Pushes a breathing cycle into the stack
	*
	* \param[in] cycle The breathing cycle
	* \param[in] cyclesRemaining The number of cycles to run the breathing cycle. 
	* Pass -1 if the breathing cycle is to loop forever
	*/
	virtual void push_breath_layer(BreathCycle* cycle, int cyclesRemaining=-1, bool smoothTransition=true) = 0;
	/**
	* \brief Pops the top breathing layer from the stack
	*
	* The stack can become empty if the last cycle is popped. In this case, no 
	* breathing is simulated. 
	*/
	virtual void pop_breath_layer() = 0;
	/**
	* \brief Clear breathing layer stack
	*/
	virtual void clear_breath_layers() = 0;
	/**
	* \brief The current number of breathing layers in the stack
	*/
	virtual int breath_layers_count() = 0;
	/**
	* \brief The current breathing layer
	*/
	virtual BreathLayer* current_breath_layer() = 0;

	/**
	* \brief Sets the breaths-per-minute rate
	*
	* \param[in] bpm The desired breaths-per-minute
	* \param[in] smooth_transition Whether the new bpm rate should be immediately
	* effective or only at the beggining of the new breathing cycle (thus avoiding
	* animation discontinuities)
	*/
	virtual void breaths_per_minute ( float bpm, bool smoothTransition=true ) = 0;
	/**
	* \brief Gets the current breaths-per-minute rate
	*/
	virtual float breaths_per_minute () = 0;
};

/**
* \brief A breathing layer
*
* Used in the breathing cycle stack in the breathing interface
*/
class BreathLayer
{
public:
	/**
	* \brief Constructor
	*
	* Assumes the breathing cycle will loop forever.
	*
	* \param[in] cycle The breathing cycle
	*/
	BreathLayer(BreathCycle* cycle)
	{
		this->cycle = cycle;
		this->cycles_remaining = -1;
	}
	/**
	* \brief Constructor
	*
	* \param[in] cycle The breathing cycle
	* \param[in] cyclesRemaining The number of cycles to run the breathing cycle
	*/
	BreathLayer(BreathCycle* cycle, int cyclesRemaining)
	{
		this->cycle = cycle;
		this->cycles_remaining = cyclesRemaining;
	}

	/**
	* \brief The breathing cycle
	*/
	BreathCycle* cycle;
	/**
	* \brief The number of cycles to run the breathing cycle
	*/
	int cycles_remaining;
};

/**
* \brief A generic breathing cycle
*
* A breathing cycle can be understood as a function which defines the 
* respiratory volume (image) at each instant of time (domain). The image
* should be in the range [0.0,1.0]. The domain range can be arbitrary
* as the cycle will be time warped to match a certain breaths-per-minute
* (bpm) rate.
*/
class BreathCycle
{
protected:
	/**
	* \brief The type
	*/
	const char* _type;

	/**
	* \brief Whether it is inspiring
	*
	* Precisely, the inspiration phase is defined to be from the beginning of 
	* the cycle to the time corresponding to the max (respiratory volume) value. 
	* If it is not inspiring, it is assumed to be expiring. 
	*/
	bool _is_inspiring;

protected:
	/**
	* \brief Constructor
	*/
	BreathCycle();

public:
	virtual ~BreathCycle() {}

	/**
	* \brief Gets the type
	*/
	const char* type(){ return _type; }

	/**
	* \brief Whether it is inspiring
	*/
	bool is_inspiring(){ return _is_inspiring; }

	/**
	* \brief The cycle duration
	*
	* This is the domain range.
	*/
	virtual float duration() = 0;
	/**
	* \brief The respiratory volume at a certain time
	*
	* \param[in] realTime The application time. This is necessary as the code
	* might need the actual clock time
	* \param[in] cycleTime The cycle time. This is the warped time (so as to
	* maintain a bpm rate) and modulus the cycle duration
	*/
	virtual float value(double realTime, double cycleTime) = 0;
};

/**
* \brief A generic min-max cycle
*
* This cycle defines some interpolation function from a minimum to a maximum.
*/
class MinMaxBreathCycle: public BreathCycle
{
protected:
	/**
	* \brief The minimum value
	*/
	float _min;
	/**
	* \brief The maximum value
	*/
	float _max;
	/**
	* \brief The minimum value interpolation key
	*/
	NumericalInterpolationKey* _min_key;
	/**
	* \brief The maximum value interpolation key
	*/
	NumericalInterpolationKey* _max_key;

public:
	/**
	* \brief Gets the minimum value
	*/
	float min(){ return _min; }
	/**
	* \brief Sets the minimum value
	*
	* \param[in] value The new value
	*/
	void min(float value){ _min = value; }
	/**
	* \brief Setups a transition for the minimum value
	*
	* \param[in] value The new value
	* \param[in] transitionDuration The transition duration
	*/
	void min(float value, float transitionDuration);
	/**
	* \brief Gets the maximum value
	*/
	float max(){ return _max; }
	/**
	* \brief Sets the maximum value
	*
	* \param[in] value The new value
	*/
	void max(float value){ _max = value; }
	/**
	* \brief Setups a transition for the maximum value
	*
	* \param[in] value The new value
	* \param[in] transitionDuration The transition duration
	*/
	void max(float value, float transitionDuration);

	/**
	* \brief Implements BreathCycle::duration
	*/
	virtual float duration() = 0;
	/**
	* \brief Implements BreathCycle::value
	*/
	virtual float value(double realTime, double cycleTime) = 0;

protected:
	/**
	* \brief Constructor
	*/
	MinMaxBreathCycle();
	/**
	* \brief Constructor
	*
	* \param[in] min The initial minimum value
	* \param[in] max The initial maximum value
	*/
	MinMaxBreathCycle(float min, float max);
	/**
	* \brief Updates the interpolation keys
	*
	* Should be invoked at each cycle.
	*
	* \param[in] realTime The real application time
	*/
	void updateInterpolationKeys(double realTime);
};

/**
* \brief Linear interpolation breath cycle
*
* This breath cycle is the back and forth linear interpolation from a minimum 
* to a maximum
*/
class LinearBreathCycle: public MinMaxBreathCycle
{
public:
	/**
	* \brief Constructor
	*/
	LinearBreathCycle();
	/**
	* \brief Constructor
	*
	* \param[in] min The initial minimum value
	* \param[in] max The initial maximum value
	*/
	LinearBreathCycle(float min, float max);

	/**
	* \brief Implements BreathCycle::duration
	*/
	float duration(){ return 2.0f; }
	/**
	* \brief Implements BreathCycle::value
	*/
	float value(double realTime, double cycleTime);
};

/**
* \brief Sine interpolation breath cycle
*
* This breath cycle is the sinusoidal interpolation from a minimum to a maximum
*/
class SineBreathCycle: public MinMaxBreathCycle
{
public:
	/**
	* \brief Constructor
	*/
	SineBreathCycle();
	/**
	* \brief Constructor
	*
	* \param[in] min The initial minimum value
	* \param[in] max The initial maximum value
	*/
	SineBreathCycle(float min, float max);

	/**
	* \brief Implements BreathCycle::duration
	*/
	float duration(){ return float(M_PI); }
	/**
	* \brief Implements BreathCycle::value
	*/
	float value(double realTime, double cycleTime);
};

/**
* \brief A keyframe breath cycle
*
* A breath cycle defined by a user-defined sequence of keyframes
*/
class KeyframeBreathCycle: public BreathCycle
{
private:
	/**
	* \brief Time associated to the maximum value
	*/
	float _max_time;
	/**
	* \brief Interpolation start keyframe in the previous cycle
	*
	* Maintained for efficiency reasons. 
	*/
	int _last_start_keyframe;

public:
	/**
	* \brief A keyframe
	*/
	class Keyframe
	{
	public:
		/**
		* \brief Constructor
		*/
		Keyframe(){}
		/**
		* \brief Constructor
		*
		* \param[in] value The value
		* \param[in] time The time
		*/
		Keyframe(float value, float time)
		{
			this->value = value;
			this->time = time;
		}

		/**
		* \brief The value
		*/
		float value;
		/**
		* \brief The time
		*/
		float time;
	};

	/**
	* \brief The keyframes vector
	*/
	vector<Keyframe*> keyframes;

public:
	/**
	* \brief Creates a standard 6-point keyframe breath-cycle
	*
	* \param[in] breathMin The minimum value
	* \param[in] breathMax The maximum value
	* \param[in] breathStartTime The start time
	* \param[in] inspirationStartTime The inspiration start time
	* \param[in] inspirationEndTime The inspiration end time
	* \param[in] expirationStartTime The expiration start time
	* \param[in] expirationEndTime The expiration end time
	* \param[in] breathEndTime The end time
	*/
	static KeyframeBreathCycle* custom(float breathMin, float breathMax, 
		float breathStartTime, float inspirationStartTime, float inspirationEndTime, 
		float expirationStartTime, float expirationEndTime, float breathEndTime);

	/**
	* \brief Constructor
	*/
	KeyframeBreathCycle();
	/**
	* \brief Destructor
	*/
	virtual ~KeyframeBreathCycle();

	/**
	* \brief Commits the keyframe sequence
	*
	* Should be invoked after filling in the keyframe vector.
	*/
	void update();

	/**
	* \brief Implements BreathCycle::duration
	*/
	float duration();
	/**
	* \brief Implements BreathCycle::value
	*/
	float value(double realTime, double cycleTime);
};

/**
* \brief A spline breath cycle
*
* The breath cycle is defined by an arbitrary spline.
*/
class SplineBreathCycle: public BreathCycle
{
private:
	/**
	* \brief The spline
	*
	* The spline is deleted with the cycle.
	*/
	MeSpline1D* _spline;
	/**
	* \brief Time associated to the maximum value
	*/
	float _max_time;

public:
	/**
	* \brief Constructor
	*
	* \param[in] spline The spline. The spline is deleted with the cycle
	*/
	SplineBreathCycle(MeSpline1D* spline);
	/**
	* \brief Destructor
	*/
	virtual ~SplineBreathCycle();

	/**
	* \brief Gets the spline
	*/
	MeSpline1D* spline() { return _spline; }

	/**
	* \brief Commits the spline
	*
	* Should be invoked whenever the spline is updated. 
	*/
	void update();

	/**
	* \brief Implements BreathCycle::duration
	*/
	float duration();
	/**
	* \brief Implements BreathCycle::value
	*/
	float value(double realTime, double cycleTime);
};

/**
* \brief A interpolation key for a number
*
* \todo This class need not be in this file. It is a generic interpolation key for a number.
*/
class NumericalInterpolationKey
{
private:
	/**
	* \brief The current value
	*/
	float _value;
	/**
	* \brief The start interpolation value
	*/
	float _startValue;
	/**
	* \brief The end interpolation value
	*/
	float _endValue;
	/**
	* \brief The start interpolation time
	*/
	float _startTime;
	/**
	* \brief The end interpolation time
	*/
	float _endTime;
	/**
	* \brief Whether the interpolation has finished
	*/
	bool _finished;

public:
	/**
	* \brief Constructor
	*
	* \param[in] startValue The start value
	* \param[in] endValue The end value
	*/
	NumericalInterpolationKey(float startValue, float endValue);

	/**
	* \brief Sets the start time
	*
	* This value should not be changed after interpolation has started.
	*/
	void setStartTime(float time){ _startTime = time; }
	/**
	* \brief Gets the start time
	*/
	float getStartTime(){ return _startTime; }
	/**
	* \brief Sets the end time
	*
	* This value should not be changed after interpolation has started.
	*/
	void setEndTime(float time){ _endTime = time; }
	/**
	* \brief Gets the end time
	*/
	float getEndTime(){ return _endTime; }
	/**
	* \brief Updates the interpolated value
	*
	* Should be invoked at each cycle.
	*/
	void update(float time);
	/**
	* \brief Whether the interpolation has finished
	*/
	bool isFinished(){ return _finished; }
	/**
	* \brief Gets the current interpolation value
	*/
	float getValue(){ return _value; }
};


# endif // ME_CT_BREATHING_INTERFACE_H

