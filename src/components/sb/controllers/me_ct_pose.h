/*
 *  me_ct_pose.h - part of Motion Engine and SmartBody-lib
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


# ifndef ME_CT_POSE_H
# define ME_CT_POSE_H

//=================================== MeCtPose =====================================

# include <sb/SBController.h>
# include <sk/sk_posture.h>
# include <sk/sk_motion.h>

/*! This controller sends always a same posture, ie, makes the associated
    channels to rest in a same pose. The rest pose can be defined in several
    ways: through SkMotion, SkPosture, SkPose or with a channels/buffer pair. */
class MeCtPose : public SmartBody::SBController
 { private :
    SkChannelArray		_channels;	// used channels
	SrBuffer<int>		_toBuff;	// map _channels index to _buffer index
    float				_duration;	// specified duration, or -1
    const char* 		_posture_name;
	SrBuffer<float>		_buffer;    // store float data of the pose

   public :
	   static std::string type_name;

   public :
    /*! Constructor */
    MeCtPose ();

    /*! Destructor is public but pay attention to the use of ref()/unref() */
   virtual ~MeCtPose ();

	void init_channel_map( void );

    /*! Defines the channels to be used and fill the rest posture with zeros */
    void init ( SbmPawn* pawn,  const SkChannelArray& ca);

    /*! Defines the channels and the values from the given posture p */
    void init ( SbmPawn* pawn, const SkPosture& p );

    /*! Defines the channels as those declared in the given motion, and set the
        MeCtPose posture from the motion at time t. After this call, motion m
        will be disconnected to any previous connection in the motion. */
    void init ( SbmPawn* pawn, SkMotion* m, float t );

	/*! Initialize a controller by cloning another */
	void init( SbmPawn* pawn, MeCtPose* other);

    /*! Sets the name of the SkPosture used to initialize this pose controller. */
    void posture_name ( const char* posture_name ) { _posture_name = posture_name; }
    
    /*! Retrieves the name of the SkPosture used to initial this  */
    const char* posture_name () { return _posture_name; }

	/*! Set the values of the postures. They must match with the used channels. */
    void values ( const SrBuffer<float>& values ) { _buffer=values; }

    /*! Defines the desired duration of this rest controller. Default is 1 second.
        For undetermined duration, -1 can be specified. */
    void duration ( float d ) { _duration = d; }
    
    /*! Retrieves the duration, which can be -1. */
    float duration () { return _duration; }

    /*! Output data */
    void output ( SrOutput& out );

    /*! Reads the data. Returns false if the read pose name is not in the
        given hash table of SkPostures */
    bool input ( SrInput& inp, const SrHashTable<SkPosture*>& postures );

	virtual void print_state( int tabCount );


   private : // callbacks for the base class

    virtual bool controller_evaluate ( double t, MeFrameData& frame );
    virtual SkChannelArray& controller_channels ();
    virtual double controller_duration ();
    virtual const std::string& controller_type () const;
 };

//======================================= EOF =====================================

# endif // ME_CT_POSE_H

