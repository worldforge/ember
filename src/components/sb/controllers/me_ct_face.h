/*
 *  me_ct_face.h - part of SmartBody-lib
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
 *      Andrew n marshall, USC
 */

# ifndef ME_CT_FACE_H
# define ME_CT_FACE_H

//////////////////////////////////////////////////////////////////////////////////

#include <sr/sr_hash_table.h>
#include <sr/sr_buffer.h>
#include <sk/sk_motion.h>
#include <sk/sk_skeleton.h>
#include <controllers/me_controller.h>
#include <sb/SBFaceDefinition.h>
#include "sbm/sr_hash_map.h"
#include "sb/SBController.h"
#include "sb/SBCharacter.h"
#include <vector>

/*
	First Pass: 09/05/07
		
	FUNCTION:
		- CT is initialized with a base (neutral) motion-pose for reference
		- base pose channels are added to the CT channels
		- each key is also added to the CT channels as a weight for each associated pose
		- weight channels are manipulated outside of this CT
		- frame buffer is assigned the base pose plus the weighted key poses
	
	ASSUMPTIONS:
		- all input motion-poses have the same joint params in the same order
		- only the first posture in a motion is used
		- once keys have been added, followed by finish_adding(), no more keys are added.
*/

class MeCtFace : public SmartBody::SBController	{ 

	private:

		double              _duration;  // the time-warped duration
		std::vector<int>       _bChan_to_buff; // base-motion's channels to context's buffer index
		std::vector<int>       _kChan_to_buff; // weight key's channels to context's buffer index
		SkChannelArray		_channels; // override motion channels, to include world_offset
		
		SkMotion*				_base_pose_p;
		srHashMap <SkMotion>	_key_pose_map;
		
		SkSkeleton* 	    _skeleton_ref_p;
		SmartBody::SBCharacter* _character;

		SrBuffer<bool>			_include_chan_flag; // key channels to exclude from calculation
		std::map<std::string, std::vector<int> > _visemeChannelMap;
		std::vector<int> _baseChannelToBufferIndex;
		bool				_useVisemeClamping;

		SkMotion*			_customized_motion;
		double				_motionStartTime;
		SrBuffer<int>        _mChan_to_buff; // motion's channels to context's buffer index
		
	public:
		static std::string type_name;

		/*! Constructor */
		MeCtFace( void );

		/*! Destructor is public but pay attention to the use of ref()/unref() */
		virtual ~MeCtFace( void );
		
		void clear( void );

		void customizeMotion(const std::string& motionName, double startTime);
		
		virtual void init (SbmPawn* pawn);
		void remove_joint( const char *joint_name );
		void remove_channel( const char *joint_name, SkChannel::Type ch_type );
		void add_key( const char *weight_key, SkMotion* key_pose_p );
		void finish_adding( void );

		virtual double controller_duration ();

	private:
		bool updateMotion(double t, MeFrameData& frame);

		// callbacks for the base class
		virtual void context_updated( void );
		virtual void controller_start();
		virtual void controller_map_updated();
		virtual bool controller_evaluate ( double t, MeFrameData& frame );
		virtual SkChannelArray& controller_channels ();
		virtual const std::string& controller_type () const;
		virtual void print_state( int tabCount );
};

//////////////////////////////////////////////////////////////////////////////////
# endif // ME_CT_FACE_H

