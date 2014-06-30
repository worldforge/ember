/*
 *  me_ct_quick_draw.h - part of SmartBody-lib
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
 *      Marcus Thiebaux, USC
 */

# ifndef ME_CT_QUICK_DRAW_H
# define ME_CT_QUICK_DRAW_H

//////////////////////////////////////////////////////////////////////////////////

#include <sr/sr_hash_table.h>
#include <sr/sr_buffer.h>
#include <sk/sk_motion.h>
#include <sk/sk_skeleton.h>
#include <sb/SBController.h>

#include "sbm/gwiz_math.h"

// ASSUME: right handed quick-draw motion only
// DETERMINE: if handedness can be extracted from raw motion

/*
SBM test commands:

        ctrl <> quickdraw <qdraw-motion-name>
        quickdraw <> <dur-sec> local|world <pitch-deg> <heading-deg> <roll-deg>
#        quickdraw <> L|R <sec|dps> local|world <pitch-deg> <heading-deg> <roll-deg>


 * The input drawing motion can be any direction, any reasonable magnitude.
 * Tokens 'dur' and 'time' are synonymous.

QuickDraw motion:

        sf-smartbody/data/sbm-test/common-sk/qdraw/AdultM_FastDraw001.skm

Example sequence:

        0.0 load motions ../../../../data/sbm-test/common-sk/qdraw
        0.0 ctrl Q quickdraw AdultM_FastDraw001
        0.0 quickdraw Q 0.9 point 1000.0 200.0 1000.0
        0.0 char doctor ctrl Q begin

 * This will induce the doctor to shoot 45 degrees to his left.

Testing sequence:

        seq sbm-qdraw-init
        seq sbm-qdraw
*/

class MeCtQuickDraw : public SmartBody::SBController	{ 

	private:
//		enum aim_coord_enum_set	{
//			AIM_LOCAL,
//			AIM_WORLD
//		};
		enum draw_mode_enum_set	{
			DRAW_DISABLED,
			DRAW_READY,
			DRAW_AIMING,
			DRAW_TRACKING,
			DRAW_RETURN,
			DRAW_COMPLETE
		};
		enum arm_joint_enum_set	{
			ARM_JOINT_SHOULDER,
			ARM_JOINT_ELBOW,
			ARM_JOINT_FOREARM,
			ARM_JOINT_WRIST,
			NUM_ARM_JOINTS
		};

		struct joint_state_t	{
			gwiz::vector_t	parent_pos; // world coord of immediate parent joint
			gwiz::quat_t		parent_rot;
			gwiz::vector_t	local_pos;
			gwiz::quat_t		local_rot;
			gwiz::vector_t	world_pos;
			gwiz::quat_t		world_rot;
		};

		SkMotion*            _gundraw_motion; 
		SkMotion*            _holster_motion; 
		SkMotion*            _curr_motion_p; 

		SkMotion::InterpType _play_mode; // its play mode
		double               _duration;  // the time-warped duration
		int                  _last_apply_frame; // to optimize shared motion evaluation
		SrBuffer<int>        _motion_chan_to_buff; // motion's channels to context's buffer index
		SrBuffer<int>        _arm_chan_indices; // arm channels in raw motion

		SkSkeleton* 	skeleton_ref_p;
		gwiz::vector_t		point_target_pos;
		char*			target_ref_joint_str;
		SkJoint*		target_ref_joint_p;

		float smooth;
		int start;
		double prev_time;
		gwiz::vector_t prev_w_point;
		
		float curr_motion_dur;
		float curr_motion_scale;
		float curr_play_dur;
		float curr_hard_dur;

		float blend_in;
		float blend_out;

		float raw_gundraw_dur;
		float raw_gundraw_scale;
		float play_gundraw_dur;
		float hard_gundraw_dur; // without embedded leadup
		int has_gundraw_leadup;
		
		float raw_holster_dur;
		float raw_holster_scale;
		float play_holster_dur;
		float hard_holster_dur; // without embedded leadaway
		int has_holster_leadaway;
		
		float track_dur;
		float reholster_time;
		float complete_time;

		int draw_mode;

		gwiz::quat_t   l_final_hand_in_body_rot;
		gwiz::quat_t   l_wrist_offset_rot;

		float * interim_pose_buff_p;
		
	public:
		static std::string type_name;

		/*! Constructor */
		MeCtQuickDraw ();

		/*! Destructor is public but pay attention to the use of ref()/unref() */
		virtual ~MeCtQuickDraw ();

		/*! Set the motion to be used. A valid motion must be set using
    		this method before calling any other method.
    		The old motion is unreferenced, and the new one is referenced.
    		(SkMotion derives SrSharedClass and has ref/unref methods)
    		The keytimes of m are translated to ensure start from zero. 
    		MeController::init() is automatically called. */
		void init( SbmPawn* pawn, SkMotion* mot_p, SkMotion* mot2_p = NULL );

		void set_gundraw_duration( float sec );
		void set_holster_duration( float sec );
		void set_motion_duration( float gundraw_sec, float holster_sec );
		void set_motion_duration( float sec );
		float get_gundraw_duration() { return play_gundraw_dur; }; // NOTE: must subtract lead-up/away
		float get_holster_duration() { return play_holster_dur; }; // ''

		void set_smooth( float sm ) { smooth = sm; }
		void set_aim_offset( float p, float h, float r );

		// Target API
		void set_target_point( float x, float y, float z );
		void set_target_coord_joint( SkJoint* joint_p );
		void set_target_joint( float x, float y, float z, SkJoint* ref_joint_p = NULL );
		
		// Track-dur: if less than 0.0, track indefinitely
		void set_track_duration( float dur ) { track_dur = dur; }
		void set_reholster( void ) { set_track_duration( 0.0 ); }
		
		/*! Set the play mode, default is linear */
		void play_mode ( SkMotion::InterpType it ) { _play_mode=it; }

		/*! Returns the current play mode */
		SkMotion::InterpType play_mode () const { return _play_mode; }

#if 0
		/**
		 *  Return initial blend time, when body can blend to the source animation's first frame.
		 *  Defaults to 1/3rd of a second.
		 */
		float indt() { return 0.3f; }

		/**
		 *  Return final blend time, when body can blend from the source animation's first frame.
		 *  Defaults to 1/3rd of a second.
		 */
		float outdt() { return 0.3f; }
#endif

	private:
		gwiz::quat_t get_final_joint_rot( SkMotion* mot_p, char *joint_name );
		joint_state_t capture_joint_state( SkJoint *joint_p );
		gwiz::quat_t rotation_to_target( gwiz::vector_t l_forward_dir, gwiz::vector_t w_target_pos, joint_state_t* state_p );

		SkJoint*	find_joint( char *joint_str, SkJoint **joint_pp );
		SkJoint*	target_ref_joint( void );
		gwiz::vector_t	world_target_point( void );
		
		void reset_blend( void );
		
		// callbacks for the base class
		virtual void context_updated( void );
		virtual void controller_start();
		virtual void controller_map_updated();
		virtual bool controller_evaluate ( double t, MeFrameData& frame );
		virtual SkChannelArray& controller_channels ();
		virtual double controller_duration ();
		virtual const std::string& controller_type () const;
		virtual void print_state( int tabCount );
};

//////////////////////////////////////////////////////////////////////////////////
# endif // ME_CT_QUICK_DRAW_H

