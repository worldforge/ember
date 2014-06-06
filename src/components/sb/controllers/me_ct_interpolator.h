/*
 *  me_ct_interpolator.h - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
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
 *      Yuyu Xu, USC
 */

#ifndef _ME_CT_INTERPOLATOR_H_
#define _ME_CT_INTERPOLATOR_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <controllers/me_ct_container.hpp>
#include <controllers/me_ct_motion.h>
#include <map>
#include <string>
#include <controllers/me_spline_1d.hpp>
#include <sb/sbm_character.hpp>


class MeCtInterpolator : public MeCtContainer
{
public:
	static std::string CONTROLLER_TYPE;

	class Context : public MeCtContainer::Context 
	{
	protected:
		static std::string CONTEXT_TYPE;
	public:
		Context( MeCtInterpolator* container, MeControllerContext* context = NULL )
			:	MeCtContainer::Context( container, context )
		{}

		const std::string& context_type() const {	return CONTEXT_TYPE; }
		void child_channels_updated( MeController* child );
	};

public:
	// constructor
	MeCtInterpolator(MeController* child1 = NULL, MeController* child2 = NULL, double time = 0.0, double w = 0.0, bool loop = false, std::string controllerName = "");
	~MeCtInterpolator();

	// child accessor
	MeController* child(size_t n);
	int child(std::string name);

	double getWeight();
	void setWeight(float w);

	bool getLoop();
	void setLoop(bool l);

	bool getNewLoop();
	void setNewLoop(bool l);

	bool getNewCycle(int index);

	void setReverseWeight(bool rW)	{reverseWeight = rW;}
	bool getReverseWeight()			{return reverseWeight;}

	double getStartTime() {return startTime;}
	void setStartTime(double time) {startTime = time;}

	std::vector<double>& getKey(int index);

	double phaseDuration();
	int getNumLoops()	{return numLoops;}

	void initKeys();
	double getDuration(int index);
	void initDuration();
	void updateChildren(int index, MeController* newController);	// replace child[index] with new controller

	// callbacks for the base class
	virtual void controller_map_updated();
    virtual bool controller_evaluate( double t, MeFrameData& frame );
    virtual SkChannelArray& controller_channels();
    virtual double controller_duration();
	virtual const std::string& controller_type() const {return CONTROLLER_TYPE;}

private:
	void getTiming(double t, double& t1, double& t2);
	void adjustStartTime(double origW, double newW);
	void getFrame(int index, double t, SrBuffer<float>& buffer);

private:
	bool loop;					// loop mode flag
	bool newLoop;				// new loop flag
	int numLoops;				// number of loops
	bool newCycle1;				// whether child1 is in a new cycle
	bool newCycle2;				// wheter child2 is in a new cycle

	bool reverseWeight;			

	double startTime;			// start time of new loop
	double duration;			// duration of this controller

	double weight;				// weight of child1
	std::vector<double> key1;	// key times for child1
	std::vector<double> key2;	// key times for child2

	SkChannelArray channels;	// channels for this controller
	MeController* child1;		// child1 controller
	MeController* child2;		// child2 controller
};

#endif
