/*
 *  sk_motion.h - part of Motion Engine and SmartBody-lib
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
 *      Ashok Basawapatna, USC (no longer)
 *      Marcus Thiebaux, USC
 */

# ifndef SK_MOTION_H
# define SK_MOTION_H

#include <sb/SBTypes.h>
# include <sr/sr_input.h>
# include <sr/sr_shared_class.h>
# include <sk/sk_channel_array.h>
# include <sbm/sr_synch_points.h>
# include <sb/SBAsset.h>
# include <queue>

class SkPosture;
class SkSkeleton;

namespace SmartBody {
	class SBMotionEvent;
	class SBRetarget;
}


/*! Maintains a motion defined as a sequence of keyframes, each
with a time stamp */
class SkMotion : public SmartBody::SBAsset
{
public:
	struct Frame { 
			float keytime; 
			float* posture; 
	};

	protected :
	int _postsize;            // size of each posture
	SkSkeleton* _skeleton;    // the connected skeleton
	float* _floatbuffer;      // or the connected buffer
	std::string _filename;          // file name (optional)
	std::vector<Frame> _frames;   // frame data
	SkChannelArray _channels; // channels
	int _last_apply_frame;    // used to speed up playing with monotone time

	std::vector<SmartBody::SBMotionEvent*> _motionEvents;

	// Unset times are given a value of -1
#if 0
	float _time_ready;
	float _time_stroke_start;
	float _time_stroke_emphasis;
	float _time_stroke_end;
	float _time_relax;
#endif

	std::vector<SrVec> _frameOffset;
	std::vector<SrQuat> _frameOrientation;
	bool _isRegistered;


public :
	// Timing Metadata:
	srSynchPoints synch_points;

	/*! Constructor */
	SkMotion();

	/*! Destructor is public but pay attention to the use of ref()/unref() */
	virtual ~SkMotion();

	/*! Set a file name to be associated with the motion.
	This information is not saved in the motion file and is 
	not used by SkMotion. It is here just as a convenient 
	place to store the information */
	void filename ( const char* n ) { _filename = n; }

	/*! Get the file name associated with the motion */
	const std::string& filename () const { return _filename; }

	/*! Clears all data, creating an empty motion */
	void init ();

	/*! Creates an empty motion with the given channels */
	void init ( const SkChannelArray& ca );

	/*! Compress internal arrays */
	void compress ();

	/*! Loads a motion file and returns true if no errors.
	Both .skm and .bvh formats are read here */
	bool load ( SrInput& in, double scale = 1.0 );

	/*! Save the motion to a file and returns true if no errors */
	bool save ( SrOutput& out );

	/*! Creates a motion by setting frames as given keypostures and
	at the given keytimes. Only the channels with a varying value,
	in relation to the first keyposture, are stored in the motion.
	Arrays keypostures and keytimes must have the same size.
	True is returned if the motion could be created, false otherwise. */
	bool create_from_postures ( const std::vector<SkPosture*>& keypost, 
									 const std::vector<float>& keytime );
	/*! Inserts one channel at position i, with the given name, type and value.
	This implies inserting new position(s) in all frames. Returns true if
	all parameters are ok. Note that the channel will only take effect when
	reconnecting the motion to a skeleton. */
	bool insert_channel ( int pos, const char* name, SkChannel::Type type, float* values );

	/*! Inserts a new frame at position pos, with key time kt. True returned if success. */
	bool insert_frame ( int pos, float kt );

	/*! Returns the internal channel array, which should not be modified */
	SkChannelArray& channels () { return _channels; }

	/*! Number of frames in the current motion */
	int frames () const { return _frames.size(); }

	/*! The size of the float array representing one posture. */
	int posture_size () const { return _postsize; }

	/*! Returns a float array of size posture_size(), containing the
	posture at frame f. It is the user responsibility to
	ensure that 0<=f and f<frames() */
	float* posture ( int f ) { return _frames[f].posture; }

	/*! Returns the keytime of frame f. It is the user
	responsibility to ensure that 0<=f and f<frames() */
	const float& keytime ( int f ) const { return _frames[f].keytime; }

	/* Sets the keytime of a particular frame */
	void keytime ( int f, float time ) { if ((int) _frames.size() > f && f >= 0) _frames[f].keytime = time; }

	/*! Returns the keytime of the last frame */
	float last_keytime () const { if (_frames.size() == 0) return -1.0f; return _frames[frames()-1].keytime; }

	/*! Returns the final keytime of the motion minus the first one.
	This method requires the motion to not be empty. */
	float duration () {  if (_frames.size() == 0) return 0; return _frames[_frames.size() - 1].keytime-_frames[0].keytime; }

	/*! Matches the joint names defined in SkMotion and in SkSkeleton,
	stablishing a direct link between each channel in the motion
	to the corresponding channels in the skeleton's joints.
	The number of channels matched is returned. A null
	parameter will disconnect the motion and set to null all joints
	pointers in the associated channel array. The motion must be
	disconnected in case the skeleton changes or is deleted. */
	int connect ( SkSkeleton* s );

	/*! Connects to the given buffer. The apply methods will then put
	values in the given buffer, which size must match with the
	result of channels().count_floats(). A null parameter will
	disconnect the motion (but will not touch the channel array).
	This call simply saves the pointer (thus is very efficient) */
	virtual void connect ( float* buffer );

	/*! disconnect the motion by calling connect((SkSkeleton*)0) */
	virtual void disconnect () { connect((SkSkeleton*)0); }

	/*! Returns the current connected skeleton (can be a null pointer)*/
	SkSkeleton* connected_skeleton () const { return _skeleton; }

	/*! Returns the current connected buffer (can be a null pointer)*/
	float* connected_buffer () const { return _floatbuffer; }

	/*! Apply frame f to the attached skeleton or connected ChannelArray.
	Only sucessfully matched channels are used. Parameter f can
	be out of range to specify extreme postures. */
	SBAPI void apply_frame ( int f );

	/*! Apply frame f to the attached skeleton. Will only work if an
	skeleton was connected to the motion with method connect.
	Only sucessfully matched channels are used. Parameter f can
	be out of range to specify extreme postures. */
	SBAPI void apply_frame ( int f, float* buffer, SrBuffer<int>* map_p, bool isAdditive = false );	

	SBAPI void getChannelValueAtFrame(int f, int index, float* val);

	
	/*! Interpolation type used by apply */
	enum InterpType { Linear, CubicSpline };

	/*! Evaluates and apply the motion at time t to the connected skeleton or buffer.
	The search for the 2 keyframes to be interpolated (adjacent to t) is done linearly,
	however as 99% of the cases the evaluation is monotone, the last frame applied
	serves as a starting point for the search, resulting in maximum efficiency.
	To optimize evaluations from several controllers sharing a same motion file,
	parameter lastframe can be used and will store the last frame used per controller. */
	SBAPI void apply ( float t, InterpType=Linear, int* lastframe=NULL, SmartBody::SBRetarget* retarget = NULL );

	/*! Evaluates and apply the motion at time t to any float* buffer (or channel joints if NULL).
	Unless map_p is specified, the buffer is assumed to be in motion's channel order with all channels present.
	The search for the 2 keyframes to be interpolated (adjacent to t) is done linearly,
	however as 99% of the cases the evaluation is monotone, the last frame applied
	serves as a starting point for the search, resulting in maximum efficiency.
	To optimize evaluations from several controllers sharing a same motion file,
	parameter lastframe can be used and will store the last frame used per controller. */
	SBAPI void apply ( float t, float* buffer, SrBuffer<int>* map_p, InterpType=Linear, int* lastframe=NULL, bool isAdditive = false, SmartBody::SBRetarget* retarget = NULL );

	void applyNew ( float t, float* buffer, SrBuffer<int>* map_p, InterpType=Linear, int* lastframe=NULL, bool isAdditive = false, SmartBody::SBRetarget* retarget = NULL );


	/*! Returns a string describing the interpolation type */
	static const char* interp_type_name ( InterpType type );

	/*! Returns the type relative to the string description */
	static InterpType interp_type_name ( const char* type );

	/*! Copy operator. Copies all motion data and connection status from m */
	void operator = ( const SkMotion& m );

	/*! Move all keytimes so that the first keytime starts at the given time */
	void move_keytimes ( float startkt );
	
	SkMotion* copyMotion();

	SkMotion* buildPrestrokeHoldMotion(float holdTime, SkMotion* idleMotion = NULL);
	SkMotion* buildPoststrokeHoldMotion(float holdTime, std::vector<std::string>& joints, float scale = 1.0f, float freq = -1.0f, SkMotion* idleMotion = NULL);
	
	void smoothAtFrame(int frameId, int interval, int maskSize = 3, int maskType = 0);
	void smoothByMask(std::vector<int>& frameIds, std::vector<float>& mask);
	void addPerlinNoise(std::vector<std::string>& affectedJoints, int startFrame = -1, int endFrame = -1, float amp = 1.0f, float freq = -1.0f);
	

	// Build and return a mirror motion from the motion. The mirror axis is assumed to be y-z plane with the character center at origin.
	SkMotion* buildMirrorMotion(SkSkeleton* skeleton);
	SkMotion* buildMirrorMotionJoints(SkSkeleton* skeleton, const std::map<std::string,bool>& jointNameList);

	// smooth out the window between [0,timeInterval] and [duration-timeInterval, duration] so the motion will look smooth when cycling
	SkMotion* buildSmoothMotionCycle(float timeInterval);

	// retarget the motion from source skeleton to target skeleton
	SBAPI SkMotion* buildRetargetMotionV2(SkSkeleton* sourceSk, SkSkeleton* targetSk, std::vector<std::string>& endJoints, 
									std::vector<std::string>& relativeJoints, std::map<std::string, SrVec>& offsetJoints);

#if 0
	SkMotion* buildRetargetMotion(SkSkeleton* sourceSk, SkSkeleton* targetSk, std::vector<std::string>& endJoints, 
								  std::vector<std::string>& relativeJoints, std::map<std::string, SrVec>& offsetJoints);	
	SkMotion* buildRetargetMotion2(SkSkeleton* sourceSk, SkSkeleton* targetSk);
	SkMotion* buildRetargetMotion3( SkSkeleton* sourceSk, SkSkeleton* targetSk );
#endif

	/*! Change the angle values of all channels in euler angles type by
	adding +-2PI, in order to have the smallest distance between frames,
	e.g., to obtain interpolation from 0 to -60, instead of 0 to 300.
	Note that this might not work depending on the joint limits defined */
	void correct_euler_angles ();

	/*! Change the values of the given channel index, from all frames from f1 to f2, according to
	the given multiplication factor and offset. First the actual channel value is multiplied
	by factor, and then added with offset. If mfactor is 0, offset becomes the new value of
	the channel. For the Swing and Quat types of channels, mfactor acts as a multiplication
	of the current rotational angle encoded in the rotation. Offset will contain up to
	4 valid positions, according to the channel type */
	void change_channel_values ( int f1, int f2, int channel, float mfactor, const float* offset );

#if 0
	float time_ready()
	{	return _time_ready; }
	float time_stroke_start()
	{	return _time_stroke_start; }
	float time_stroke_emphasis()
	{	return _time_stroke_emphasis; }
	float time_stroke_end()
	{	return _time_stroke_end; }
	float time_relax()
	{	return _time_relax; }
#else

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

#endif

#if 0
	void time_ready( float time_ready )
	{	_time_ready = time_ready;}
	void time_stroke_start_time( float time_stroke_start )
	{	_time_stroke_start = time_stroke_start; }
	void time_stroke_emphasis( float time_stroke_emphasis )
	{	_time_stroke_emphasis = time_stroke_emphasis; }
	void time_stroke_end( float time_stroke_end )
	{	_time_stroke_end = time_stroke_end; }
	void time_relax(float time_relax )
	{	_time_relax = time_relax; }
#endif

	/*! Registers the animation by placing the world offset and orientation into 
		the _frameOffset and _frameOrientation variables. These values will
		then be removed from the animation playback.
	*/
	void registerAnimation();
	void unregisterAnimation();
	bool isRegistered();
	void setFrameOrientation(std::vector<SrQuat>& orientation);
	std::vector<SrQuat>& getFrameOrientation();
	void setFrameOffset(std::vector<SrVec>& offset);
	std::vector<SrVec>& getFrameOffset();

	void addMotionEvent(SmartBody::SBMotionEvent* motionEvent);
	std::vector<SmartBody::SBMotionEvent*>& getMotionEvents();

private : 
	bool _load_bvh ( SrInput& in );
public:
	//static void convertBoneOrientation( std::string &pjointName, SkSkeleton* interSk, SkSkeleton* tempSrcSk, std::queue<std::string> &jointQueues, std::map<std::string, SrQuat> &jointRotationMap, std::vector<std::string>& endJoints );
	static void convertBoneOrientation( std::string &pjointName, SkSkeleton* interSk, SkSkeleton* tempSrcSk, std::vector<std::string>& endJoints );

};

//================================ End of File =================================================

# endif  // SK_MOTION_H
