/*
 *  me_controller.h - part of Motion Engine and SmartBody-lib
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
 *      Marcelo Kallmann, USC (currently at UC Merced)
 *      Andrew n marshall, USC
 *      Marcus Thiebaux, USC
 */

#ifndef ME_CONTROLLER_H
#define ME_CONTROLLER_H

#include <string>
#include <list>
#include <vector>

#include <sb/SBTypes.h>
#include <sr/sr_hash_table.h>
#include <sr/sr_buffer.h>
#include <sr/sr_shared_class.h>
#include <sk/sk_channel_array.h>
#include <sk/sk_motion.h>
#include <sk/sk_posture.h>
#include <sk/sk_skeleton.h>
#include <sb/SBObject.h>
#include <sb/SBAttribute.h>
#include <sb/DefaultAttributeTable.h>
#include <controllers/me_controller_context.hpp>


// Predeclare class because of circular reference:
class MeController;
class SbmPawn;
class MeControllerContext;
class MePrunePolicy;

//=================================== MeController =====================================

/*! The Base class of all controllers. It contains few parameters which are
    considered to be used for all derived controllers, and in particular
    it maintains its buffer of float values keeping the result of the
    last call to evaluate() and as well an integer buffer to be used as
    a mapping of indices to another buffer of float values */
class MeController
    : public SrSharedClass, public SmartBody::SBObject, public DefaultAttributeTable
{
private :
	static int instance_count;

	int _instance_id;		// character being recorded
	int _invocation_count;	// number of times outputing recording files	

    float _indt;              // initial period for blending, eg static or prep phase
    float _outdt;             // final period for blending, eg static or retract phase
    float _emphasist;         // time point of "main importance" in the controller
		
	bool _initialized;
	bool _active;             // if the controller is still active
    double _lastEval;         // time at which the controller was evaluated last
	double _startTime;		  // time when the controller was started
	double _stopTime;         // time when the controller was stopped

	//std::string _handle;	  // handle of the controller - used to match requests to changes in the controller parameters
	MePrunePolicy* _prune_policy;  // controller tree pruning policy for this controller

protected :

	// Fields for new evaluation model

	/**
	 *  Pointer to the current evaluation context.
	 *  This variable is managed by MeControllerContext::add_controller() and ::remove_controller().
	 *  (The variable should be made private and accessed only via context().)
	 *  If no context is set, the value will be NULL.
	 *  MeController::context_updated() is called immediately after the _context value changes.
	 */
    MeControllerContext* _context;

	/**
	 *  Maps the controller_channels() indices to the equivalent channel indices of the _context.
	 *  Values mapping to -1 represent channels that do not exist in the evaluation context.
	 */
    SrBuffer<int>        _toContextCh;    // map local channel index to _context channel index (see remap())
	
	enum record_mode_enum_set {
		RECORD_NULL,
		RECORD_POSE,
		RECORD_MOTION,
		RECORD_BVH_MOTION
	};

	// motion recording state: output controller results to SkMotion file:
	int			_record_mode;
	std::string	_record_full_prefix;
	int			_record_frame_count;
	SrOutput	*_record_output; // for recording poses and motions of immediate local results
	double		_record_dt;
	double      _record_duration;
	typedef std::string FRAME;
	std::list<FRAME>	*_frames;	// buffer to store the frame data
	int			_record_max_frames; // maximum capacity of the buffer

	// debugging: recording controller's MeFrameData, inspecting the changes each controller make on the buffer data 
	bool		_buffer_changes_toggle;			// toggle, whenever it's being called, recording the changes that evaluation make
	std::vector<float>	_bufferRecord;              // data containing the changes made each time inside the controller evaluation
	bool		_buffer_changes_toggle_reset;	// flag to initialize the buffer_changes
	bool		_enable;						// determines if this controller does not get evaluated. 	
	std::string	_handle;

protected :
    /*! Constructor */
    MeController ();

	void clone_parameters( MeController* other );

public :
    /*! Destructor is public but pay attention to the use of ref()/unref() */
    virtual ~MeController ();

	srSynchPoints synch_points;

	double time_start()
	{	return synch_points.get_time( srSynchPoints::START ); }
	double time_ready()
	{	return synch_points.get_time( srSynchPoints::READY ); }
	double time_stroke_start()
	{	return synch_points.get_time( srSynchPoints::STROKE_START ); }
	double time_stroke_emphasis()
	{	return synch_points.get_time( srSynchPoints::STROKE ); }
	double time_stroke_end()
	{	return synch_points.get_time( srSynchPoints::STROKE_STOP ); }
	double time_relax()
	{	return synch_points.get_time( srSynchPoints::RELAX ); }
	double time_stop()
	{	return synch_points.get_time( srSynchPoints::STOP ); }

	const std::string& handle() const;
	void handle ( std::string handle );

	///////////////////////////////////////////////////////////////////////
    // Controller tree traversal methods

    /**
	 *  Returns number of child controllers.
	 *
	 *  Default implementation has no children.
	 * 
	 *  HACK: Marcelo's containers are not 64-bit (size_t) compliant.
	 *        There are explicit casts that will break elsewhere.
	 */
	virtual size_t count_children() {
		return 0;
	}

    /**
     *  Get Nth child.
	 *
     *  Returns NULL for invalid n.
     */
	virtual MeController* child( size_t n )
	{	return NULL; }

	/**
	 *  Removes child from this controller.
	 *  Returns true if child is not a child at the end of the method,
	 *  even if it wasn't at the beginning of the method
	 *  (in other words, the return value is not strictly an error flag).
	 *
	 *  While not all controllers have child controllers, this function is
	 *  needed as a hook for the context of parent controllers.
	 */
	virtual bool remove_child( MeController* child )
	{	return false; }

	/**
	 *  Removes all child controllers.
	 *
	 *  Default implementation utilizes the child(n) and remove_child(..) interfaces,
	 *  but the method is virtual to enable more optimized implementations.
	 */
	virtual void remove_all_children();

    /**
     *  Sets the evaluation context, or unset if context is NULL.
     */
    MeControllerContext* const context()
	{	return _context; }

    /*! returns the specified emphasis time, the default value is <0,
        meaning that the emphasis time point is unknown */
    float emphasist () const { return _emphasist; }
    
    /*! Specifies the desired emphasis time, a <0 value
        means that the emphasis time point is unknown */
    void emphasist ( float t );
    
  public:
    /*! Returns the activation state */
    bool active () const { return _active; }

    /*! Defines the duration of the initial and final periods of the controller, 
        which can be used for blending and other adjustment purposes.
        Given times are corrected to ensure not exceeding the duration. */
    void inoutdt ( float indt, float outdt );
    
    /*! Get the indt time */
    virtual float indt () const { return _indt; }

    /*! Get the outdt time */
    virtual float outdt () const { return _outdt; }
    
    /*! Allocates a buffer (and set zero values) for the channels maintained by the
        derived class, and calls the initialization method of the derived controller.
        Both controller_init() and controller_channels() are called here.
        The convention is that, if there is another init method in the derived
        class, the derived class will be responsible for calling MeController::init() */
    virtual void init (SbmPawn* pawn);


	/*! Returns the controller's prune policy, if set.  Otherwise, NULL. */
	MePrunePolicy* prune_policy();

	/*! Sets the controller's prune policy.  If set to NULL, previous policy is removed. */
	void prune_policy( MePrunePolicy* prune_policy );

    /*! This method is to be called before starting to evaluate the controller. */
    virtual void start (double time);

	/*! Returns the time that when this controller was started. */
    double start_time ();

    /*! This method will simply set the active flag of the controller to false and
        notify the derived class by calling the virtual method controller_stop(). */
    virtual void stop (double time);

	/*! Returns the time that when this controller was stopped. */
    double stop_time ();

	/*!
     *  /brief Update _toContextCh with current _channels and _context.
	 *
	 *  Update _map such that controller_channels()[n] is located at 
	 *  _context.channels()[ _toContextCh[n] ].  If a channel in 
	 *  controller_channels() is not a part of the context, the index will be 
	 *  saved as -1.  Called by _context when _context.channels() is updated.
     */
    void remap();

	/*!
	* Map from the local channels to the context.
	*/
	SBAPI int getContextChannel(int index);
   
    /*! Evaluates the controller at a local time t. */ 
    void evaluate ( double t, MeFrameData& frame );

//	void record_pose( const char *full_prefix );
	void record_motion( int max_num_of_frames );			//record start SKM
	void record_bvh( int max_num_of_frames, double dt );	//record start BVH
	void record_write( const char *full_prefix );			//write the buffer to files

	void saveMotionRecord( const std::string &recordname );
	void record_clear(void);								//clear the buffer
	void record_stop(void);									//stop the recording

	/*! Evaluates the controller changes */
	SBAPI void record_buffer_changes(bool val);				    // store the values of all channels changed by this controller
	/*SrBuffer<float>& get_buffer_changes();					// gets the channel data changed by this controller*/
	SBAPI std::vector<float>& get_buffer_changes();
	bool is_record_buffer_changes() { return _buffer_changes_toggle; }

	bool isEnabled() const;
	void setEnable(bool val);

	void dumpChannelMap();	

   protected :

	void updateDefaultVariables(SbmPawn* pawn);
	void print_tabs( int depth );
	bool print_bvh_hierarchy( SkJoint* joint_p, int depth );
	// NOTE: depth only used to hack STUPID-POLYTRANS ROOT bug
	bool print_bvh_motion( SkJoint* joint_p, int depth, FRAME& frame_data );
	void load_bvh_joint_hmap( void );

//	bool init_record( void );
	void cont_record( double time, MeFrameData& frame );
	void stop_record( void );

	void cal_buffer_changes( MeFrameData& frame ); // stores the channel values changed by this controller


    /*! Output of the generic controller data.
        This is a protected method: derived classes will be responsible to save 
        their generic controller data. */
    void output ( SrOutput& o );

    /*! Input of the generic controller data.
        This is a protected method: derived classes will be responsible to load 
        their generic controller data. */
    void input ( SrInput& i );

   protected :
	/*! This method is called at initialization time. The controller should 
        initialize itself, and get ready to start. */
	virtual void controller_init () {}  // No-op default
    
	/*!  Called immediately after a context has been set.
	 */
	virtual void context_updated() {}

	/*! This method will be called to notify that the controller is becoming active
        and therefore a new evaluation starting at time 0 will soon begin. */
	virtual void controller_start () {}  // No-op default

    /*! This method will be called to notify that the controller was requested to stop.
        At the moment this method is called, the active flag is already set to false. */
	virtual void controller_stop () {}  // No-op default

	/*!
	 *  Called when at the end of remap to allow the controller further processing.
	 */
	virtual void controller_map_updated() {}

	/*! This method is called at each time step (t starts with 0) and is where
        the derived controller will send the evaluation result to the controller
        buffer (retrieved with method buffer()). The buffer will have the exact 
        number of positions required by the used channels. The activation state
        must be returned: false if the controller has finished, and true otherwise.
        The general rule is that, if the controller has determined duration, 
        the controller should be able to answer queries which are non monotone
        in time, having more flexibility to be blended with other controllers.
        Otherwise the controller will only be evaluated in a monotone time fashion. */
    virtual bool controller_evaluate ( double t, MeFrameData& frame )=0;

   public :
    /*! This method will return the description of the channels used by the 
        derived controller. The channel array is defined and maintained by the derived
        class. The reason for that is to reduce redundancy, for ex, a SkMotion already
        has its channels definition that can be shared at the controller level. */
    virtual SkChannelArray& controller_channels ()=0;

    /*! This method will return the fixed total duration time that the controller
        will take, or will return -1 if the duration is undetermined */
    virtual double controller_duration ()=0;
    
    /*! Returns a string describing the type of the controller. The convention is that
        the string corresponds with the derived class name without the 'SrCn' prefix */
	virtual const std::string& controller_type () const = 0;

	/*! Print the info about the controller and its state to stdout.  The first 
	    line of output should begin immediately, and second and following lines 
		should be indented by tabCount number of tabs.  Child controller should 
		be indented by an additional tab. All output should end with a new line.  */
	virtual void print_state( int tab_count );

	/*! Convience method for printing children. Takes in the same tab_count as
	    print_state (that is, before it is increment for the child's print_state).  */
	virtual void print_children( int tab_count );

    virtual void notify(SmartBody::SBSubject* subject);

	friend class MeControllerContext;
};

//======================================= EOF =====================================

# endif // ME_CONTROLLER_H
