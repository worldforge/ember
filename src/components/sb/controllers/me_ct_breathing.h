/**
* \file me_ct_breathing.h
* \brief The skeleton animation breathing controller
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


# ifndef ME_CT_BREATHING_H
# define ME_CT_BREATHING_H

# include <sr/sr_buffer.h>
# include <sk/sk_motion.h>
# include <controllers/me_controller.h>
# include <controllers/me_ct_breathing_interface.h>
#include <sb/SBSubject.h>
#include <sb/SBController.h>
#include <list>
using std::list;

/**
* \brief The skeleton animation breathing controller
*
* This controller implements the breathing interface - MeCtBreathingInterface - 
* using skeleton animation. Precisely, uses animations which tend to rely
* spine1-spine5 (mostly spine1-spine3) to simulate breathing.
*
* SBM commands:
* Sintax: breathing <controller_name> <options>
* Options:
*  help
*		Shows this information.
*  bpm <value> [isSmooth]   
*		Sets the breaths-per-minute rate. 
*		Set 'isSmooth' to 'true' or 'false' to define whether to apply a 
*		smooth transition. Defaults to 'true'.
*  min <value> [transition]    
*		Sets the minimum respiratory volume. Use with the linear and sine 
*		breath cycles only. 
*		Set 'transition' to the transition duration in seconds. Default 
*		value is 1.0s.
*  max <value> [transition]    
*		Sets the maximum respiratory volume. Use with the linear and sine 
*		breath cycles only. 
*		Set 'transition' to the transition duration in seconds. Default 
*		value is 1.0s.
*  motion <name>
*		Sets the breathing motion.
*  incremental <bool> 
*		Sets whether the breathing motion is additive.
*  push linear
*		Pushes to the top of the stack the (default) linear breathing cycle.
*  push sine
*		Pushes to the top of the stack a sine breathing cycle.
*  push keyframe <time1> <value1> <time2> <value2> ...
*		Pushes to the top of the stack a custom keyframe breathing cycle.
*  push spline knot1_x knot1_y knot2_x knot2_y ...
*		Pushes to the top of the stack a spline breathing cycle.
*  pop
*		Pops the breathing cycle from the top of the stack. 
*  print  
*		Prints the controller's state
*
* SBM sequence file: test-breathing.seq
*/
class MeCtBreathing : public MeCtBreathingInterface, public SmartBody::SBController
{
private :
	/**
	* \brief The breathing layers stack
	*
	* Each layer maintains a breathing cycle for a certain number of cycles.
	* The topmost layer defines the character animation.
	*/
	list<BreathLayer*> _breath_layers;
	/**
	* \brief The default linear breath cycle
	*
	* This breath cycle is loaded by default in the stack for an infinite
	* number of cycles.
	*/
	LinearBreathCycle* _default_breath_cycle;
	
	/**
	* \brief The breathing motion
	*
	* The breathing motion must have the following properties:
	* - 'start' to 'ready': must animate maximum expiration to neutral position.
	* Anatomically this uses the interosseus portion of the internal intercostal
	* and abdominal muscles;
	* - 'ready' must equal 'stroke start';
	* - 'stroke start' to 'stroke emphasis': must animate neutral position to 
	* maximum inspiration. Anatomically, normal inspiration (beginning of this
	* sequence) uses the diaphragm, external intercostal muscles and interchondral
	* portion of the internal intercostal muscles. Forced inspiration (latter part
	* of this sequence) uses the scalenes and sternocleidomastoid muscles (in the 
	* upper chest);
	* - 'stroke emphasis' to 'rest': can have symmetrical animation but, this isn't
	* actually used by the controller.
	*/
	SkMotion* _motion;
	/**
	* \brief Whether the breathing motion is additive
	*
	* Additive motion means that breathing adds relative transformations thus, 
	* allowing composition with other motions (which might act on the bones 
	* used for breathing simulation). The default value is 'false'.
	*/
	bool _incremental;
	/**
	* \brief Used to optimize motion evaluation
	*/
	int _last_apply_frame;
	/**
	* \brief Motion's channels to context's buffer index
	*/
	SrBuffer<int> _mChan_to_buff;

	/**
	* \brief The breaths-per-minute (bpm) rate
	*/
	float _bpm;
	/**
	* \brief The offset to apply to the local time calculation
	*
	* This is used to reset the local time whenever the topmost layer in the 
	* stack changes.
	*/
	double _local_time_offset;
	/**
	* \brief Whether the topmost layer's breathing cycle was inspiring on the 
	* previous frame
	*
	* Used to determine cycle count.
	*/
	bool _previous_breath_is_inspiring;
	/**
	* \brief This is the motion frame time corresponding to neutral position
	*
	* Notice that frame time 0 corresponds to maximum expiratory volume.
	*/
	float _expiratory_reserve_volume_threshold;
	
	/**
	* \brief Whether there is a pending breath layer to be pushed into the stack
	*
	* Transition will occur at the beginning of the next breath cycle.
	*/
	BreathLayer* _pending_breath_layer;
	/**
	* \brief Whether there is a pending motion to be loaded
	*
	* Transition will occur at the beginning of the next breath cycle.
	*/
	SkMotion* _pending_motion;
	/**
	* \brief Whether there is a pending pop operation on the stack
	*
	* Transition will occur at the beginning of the next breath cycle.
	*/
	bool _pending_pop;
	/**
	* \brief Whether there is a pending request to change bpm
	*
	* Transition will occur at the beginning of the next breath cycle.
	*/
	float _pending_bpm;

	SkChannelArray _channels;

	bool _useBlendChannels;

	/**
	* \brief Whether there is a pending motion to be loaded
	*
	* Transition will occur at the beginning of the next breath cycle.
	*/
	SkMotion* _blendChannelBreathingMotion;

public :
	/**
	* \brief The controller type
	*/
	static std::string type_name;

public :
	/** 
	* \brief Constructor 
	*/
	MeCtBreathing ();
	/** 
	* \brief Destructor 
	*
	* It's public but pay attention to the use of ref()/unref().
	*/
	virtual ~MeCtBreathing ();


	void init (SbmPawn* pawn);
	/** 
	* \brief Initializes the controller
	*
	* \param[in] motion The initial breathing motion
	*/
	void setMotion ( SkMotion* motion );
	SkMotion* getMotion ( );
	/**

	* \brief Implements MeCtBreathingInterface::push_breath_layer
	*/
	void push_breath_layer(BreathCycle* cycle, int cyclesRemaining=-1, bool smoothTransition=true);
	/**
	* \brief Implements MeCtBreathingInterface::pop_breath_layer
	*/
	void pop_breath_layer();
	/**
	* \brief Implements MeCtBreathingInterface::clear_breath_layers
	*/
	void clear_breath_layers();
	/**
	* \brief Implements MeCtBreathingInterface::breath_layers_count
	*/
	int breath_layers_count(){ return _breath_layers.size(); }
	/**
	* \brief Implements MeCtBreathingInterface::current_breath_layer
	*/
	BreathLayer* current_breath_layer();

	/** 
	* \brief Gets the default linear breath cycle
	*
	* This breath cycle is loaded by default in the stack for an infinite
	* number of cycles.
	*/
	LinearBreathCycle* default_breath_cycle(){ return _default_breath_cycle; }

	/** 
	* \brief Gets the current breathing motion
	*/
	SkMotion* motion(){ return _motion; }
	/** 
	* \brief Sets the breathing motion
	*
	* The change will be effective only at the beginning of the next cycle
	* so as to avoid animation discontinuities.
	*/
	void motion( SkMotion* motion );
	/** 
	* \brief Gets whether the breathing motion is additive
	*/
	bool incremental(){ return _incremental; }
	/** 
	* \brief Sets whether the breathing motion is additive
	*/
	void incremental(bool value){ _incremental = value; }

	/**
	* \brief Implements MeCtBreathingInterface::breaths_per_minute
	*/
	void breaths_per_minute ( float bpm, bool smooth_transition = true );
	/**
	* \brief Implements MeCtBreathingInterface::breaths_per_minute
	*/
	float breaths_per_minute () { return _bpm; }
	/** 
	* \brief Gets the motion frame time corresponding to neutral position
	*
	* Notice that frame time 0 corresponds to maximum expiratory volume.
	*/
	float expiratory_reserve_volume_threshold(){ return _expiratory_reserve_volume_threshold; }

	/** 
	* \brief Returns -1 to signal that this controller has infinite duration
	*/
	virtual double controller_duration () { return -1; }
	/** 
	* \brief Prints the controller state
	*/
	virtual void print_state( int tabCount );

	/** 
	* \brief Implements MeController::controller_evaluate
	*/
	virtual bool controller_evaluate ( double t, MeFrameData& frame );
	/** 
	* \brief Implements MeController::controller_channels
	*/
	virtual SkChannelArray& controller_channels ();
	/** 
	* \brief Implements MeController::controller_type
	*/
	virtual const std::string& controller_type () const;

	/** 
	* \brief Use channels to control breathing parameters, rather than using an animation.
	*/
	virtual void setUseBlendChannels(bool val);

	/** 
	* \brief Check whether channels are used control breathing parameters, rather than using an animation.
	*/
	virtual bool getUseBlendChannels();

	virtual void notify(SBSubject* subject);


private:
	/** 
	* \brief Immediately pushes a new breath layer into the stack
	*/
	void immediate_push_breath_layer(BreathLayer* layer);
	/** 
	* \brief Immediately pops the topmost breath layer from the stack
	*/
	void immediate_pop_breath_layer();
	/** 
	* \brief Immediately updates the bpm rate 
	*/
	void immediate_breaths_per_minute(float bpm);
	/** 
	* \brief Immediately updates the breathing motion
	*/
	void immediate_motion(SkMotion* motion);

	/** 
	* \brief Implements MeController::controller_map_updated
	*/
	virtual void controller_map_updated();

};

# endif // ME_CT_BREATHING_H
