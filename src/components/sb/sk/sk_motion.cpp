/*
 *  sk_motion.cpp - part of Motion Engine and SmartBody-lib
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
 *      Ed Fast, USC
 */

#include <vhcl/vhcl.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <queue>

# include <sk/sk_motion.h>
# include <sk/sk_posture.h>
# include <sk/sk_skeleton.h>
#include <sb/SBEvent.h>
#include <sbm/gwiz_math.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <sb/SBMotion.h>
#include <sb/SBSkeleton.h>
#include <sb/SBRetarget.h>
#include <external/perlin/perlin.h>

using namespace gwiz;

//============================= SkMotion ============================

SkMotion::SkMotion() : SBAsset(),
	_postsize( 0 ),
	_filename( "" ),
	_skeleton( NULL ),
	_floatbuffer( NULL ),
	_last_apply_frame( 0 )
#if 0
	,
	_time_ready( -1 ),
	_time_stroke_start( -1 ),
	_time_stroke_emphasis( -1 ),
	_time_stroke_end( -1 ),
	_time_relax( -1 )
#endif
{}

SkMotion::~SkMotion()
 {
   init ();

   for (size_t f = 0; f < _frames.size(); f++ )
   {
      free( _frames[f].posture );
      _frames[f].posture = NULL;
   }

   for (size_t x = 0; x < _motionEvents.size(); x++)
	   delete _motionEvents[x];
 }

void SkMotion::init()
 {
   _skeleton = 0;
   _floatbuffer = 0;
   _postsize = 0;

   for (size_t x = 0; x < _frames.size(); x++)
   {
	   free ( _frames[x].posture );
   }
	_frames.clear();

   _channels.init();

   _last_apply_frame = 0;
 }

void SkMotion::init ( const SkChannelArray& ca )
 {
   init ();
   _channels = ca;
   _postsize = _channels.floats();
 }
 
void SkMotion::compress()
 {
 //  _frames.compress ();
  // _channels.compress ();
 }

bool SkMotion::create_from_postures ( const std::vector<SkPosture*>& keypost, 
									 const std::vector<float>& keytime )
 {
   if ( keypost.size()<1 || keypost.size()!=keytime.size() ) return false;

    std::vector<int> index;
   if ( !_channels.get_used_channels(keypost,&index) ) return false;

   // create the frames:
   _postsize = _channels.floats();
   _frames.resize ( keypost.size() );
   _frames.clear();
   for (size_t  i=0; i<keypost.size(); i++ )
    { insert_frame ( i, keytime[i] );
      for (size_t j=0; j<index.size(); j++ )
       { if ( index[j]<0 ) continue;
         _frames[i].posture[index[j]] = keypost[i]->values[j];
       }
    }
   return true;
 }

bool SkMotion::insert_channel ( int pos, const char* name, SkChannel::Type type, float* values )
 {
    if ( !_channels.insert(pos,name,type) ) return false;
   
   // add position in all frames:
   int ins = SkChannel::size(type);
   int size;
   for (size_t f=0; f<_frames.size(); f++ )
    { size = _postsize; // we need to save size here as buffer_insert will update it
      _frames[f].posture =
       (float*) sr_buffer_insert ( _frames[f].posture, sizeof(float), size, pos, ins );
      for (int i=0; i<ins; i++ )
       _frames[f].posture[pos+i] = values[i];
    }

   _postsize+=ins;

   return true;
 }

bool SkMotion::insert_frame ( int pos, float kt )
 {
   if ( pos<0 || size_t(pos)>_frames.size() ) return false;
   int index = 0;
   if (pos == _frames.size())
   {
	   _frames.push_back(Frame());
   }
   else
   {
	   for (std::vector<Frame>::iterator iter = _frames.begin();
		   iter != _frames.end();
		   iter++)
	   {
		   if (pos == index)
		   {
			   _frames.insert(iter, Frame());
			   break;
		   }
	   }
   }
  
   _frames[pos].keytime = kt;
   _frames[pos].posture = (float*) malloc ( sizeof(float)*_postsize );
   return true;
 }

int SkMotion::connect ( SkSkeleton* s )
 {
   _skeleton = s;
   _floatbuffer = 0;
   return _channels.connect(s);
 }

void SkMotion::connect ( float* buffer )
 {
   _skeleton = 0;
   _floatbuffer = buffer;
 }

void SkMotion::apply_frame ( int f ) {
	apply_frame( f, _floatbuffer, NULL );
}


void SkMotion::getChannelValueAtFrame( int f, int index, float* val )
{
	if ( _frames.size()==0 )
		return;
	if (f > int(_frames.size())-1 || f < 0)
		return;
	if (index > _channels.size()-1 || index < 0)
		return;
	
	int i = _frames.size()-1;
	f = SR_BOUND(f,0,i);
	float* fp = _frames[f].posture;
	
	SkChannel& chan = _channels[index];
	int fidx = _channels.float_position(index);
	for (int i=0;i<chan.size();i++)
		val[i] = fp[fidx+i];
}

void SkMotion::apply_frame ( int f, float* buffer, SrBuffer<int>* map_p, bool isAdditive ) {
	if ( _frames.size()==0 )
		return;

	int i = _frames.size()-1;
	f = SR_BOUND(f,0,i);

	float* fp = _frames[f].posture;

	if ( buffer ) {
		if( map_p ) {
			int num;
			int csize = _channels.size();
			for ( int i=0; i<csize; i++ ) {
				// channel size
				num = _channels[i].size();

				// Find and copy parent data
				int index = map_p->get( i );
				if( index >= 0 ) {
					// channel exists in data
					float* v = buffer + map_p->get(i);  // pointer to start of this channel's floats
					if (isAdditive)
					{
						if (num == 1) // channel of size one, add previous value with current value
						{
							v[0] += fp[0];
						}
						else if (num == 4) // quaternion data, compose quaternions
						{
							SrQuat existingQuat(
										v[0 ],
										v[1 ],
										v[2 ],
										v[3 ]);
							SrQuat addQuat(
										fp[0 ],
										fp[1 ],
										fp[2 ],
										fp[3 ]);
							SrQuat finalQuat = existingQuat * addQuat;
							for( int j=0; j<num; ++j )
								v[j] += finalQuat.getData(j);

						}
						else // other channel type, add to old values
						{
							for( int j=0; j<num; ++j )
								v[j] += fp[j];
						}
					}
					else
					{
						for( int j=0; j<num; ++j )
							v[j] = fp[j];
					}
				} // else skip data

				// Increment motion data pointer by channel size
				fp+=num;
			}
		} else {
			// note: memcpy does not handle overlap
			memcpy ( buffer /*dest*/, fp /*src*/, sizeof(float)*_postsize /*bytes*/ );
		}
	}
	else
	{
		// Apply to channel joints
		int inserted;
		int size = _channels.size();

		for ( i=0; i<size; i++ ) {
			if ( _channels[i].joint )
				inserted = _channels[i].set ( fp );
			else
				inserted = _channels[i].size ();
			fp += inserted;
		}
	}
}

inline float _cubic ( float t, float tmin, float tmax ) {
   t = (t-tmin)/(tmax-tmin);    // normalize t to [0,1]
   t=-(2.0f*(t*t*t)) + (3.0f*(t*t));  // cubic spline
   return t*(tmax-tmin) + tmin; // scale back
   // shape comparison with sine for graphmatica:
   // y=-(2.0*(x*x*x)) + (3.0*(x*x))
   // y=sin((x-0.5)*3)/2+0.5
}


void SkMotion::apply ( float t, SkMotion::InterpType itype, int* lastframe, SmartBody::SBRetarget* retarget ) {
	apply( t, _floatbuffer, NULL, itype, lastframe, false, retarget );
}

#define DEBUG_T 0

void SkMotion::applyNew ( float t,  
                       float* buffer, SrBuffer<int>* map_p,
                       SkMotion::InterpType itype, int* lastframe, bool isAdditive, SmartBody::SBRetarget* retarget )
{
	// old version of apply skMotion
	int fsize=_frames.size();
	if ( fsize<=0 )
		return;
	if ( t!= 0.f && t<=_frames[0].keytime )	{
#if DEBUG_T
		LOG("SkMotion::apply NOTICE: t=%.16f < f[0]:%.16f \n", t, _frames[0].keytime );
#endif
		return;
	}

	if ( itype==CubicSpline )
		t = _cubic ( t, _frames[0].keytime, _frames[_frames.size() - 1].keytime );

#if DEBUG_T
	if ( t<_frames[0].keytime )	{
		LOG("SkMotion::apply ERR: cubic t=%.16f < f[0]:%.16f \n", t, _frames[0].keytime );
	}
#endif

	// optimize keytime search for sequenced calls to apply
	int fini=0;
	if ( lastframe )
		_last_apply_frame = *lastframe;
	if ( _last_apply_frame>0 && _last_apply_frame<fsize ) {
		if ( t>_frames[_last_apply_frame].keytime )
			fini=_last_apply_frame+1;
    }

	int f;
	for ( f=fini; f<fsize; f++ ) {
		if ( t<_frames[f].keytime )
			break;
	}

	if ( f==_frames.size() ) {
		// Apply last frame
		apply_frame( f-1, buffer, map_p );
		return;
	}

	f--;
	_last_apply_frame = f;
	if ( lastframe )
		*lastframe = _last_apply_frame;

	float* fp1 = _frames[f].posture;
	float* fp2 = _frames[f+1].posture;

	// convert t to [0,1] according to the keytimes:
	t = (t-_frames[f].keytime) / (_frames[f+1].keytime-_frames[f].keytime);

#if DEBUG_T
	if ( t<0.0 )	{
		LOG("SkMotion::apply ERR: mapped t=%.16f < 0.0 \n", t );
	}
#endif

	//sr_out<<"t: "<<t<<" frames: "<<f<<srspc<<(f+1)<<"\n";
	int num;
	int csize = _channels.size();

	if ( buffer ) {
		// Apply to float* buffer
		if ( map_p ) {
			for ( int i=0; i<csize; i++ ) {
				// Channel size
				num = _channels[i].size();

				// Find mapped buffer location
				int index = map_p->get( i );
				if( index >= 0 ) {
					// channel exists in data
//					float* v = buffer + map_p->get(i);  // pointer to start of this channel's floats
					float* v = buffer + index;  // pointer to start of this channel's floats
					if (isAdditive)
					{
						if (_channels[i].type == SkChannel::Quat)
						{
							float addQuat[4];
							num = _channels[i].interp ( addQuat, fp1, fp2, t );
							SrQuat orig(v[0], v[1], v[2], v[3]);
							SrQuat add(addQuat[0], addQuat[1], addQuat[2], addQuat[3]);
							SrQuat final = orig * add;							
							for (int x = 0; x < 4; x++)
								v[x] = final.getData(x);
						}
						else if (num <= 3)
						{
							float vec[3];
							num = _channels[i].interp ( vec, fp1, fp2, t );
							for (int x = 0; x < num; x++)
								v[x] += vec[x];
						}
						else
						{
							// more that 3 channels but not a quaternion? Don't add data, just replace it...
							num = _channels[i].interp ( v, fp1, fp2, t );
						}
					}
					else
					{
						
						num = _channels[i].interp ( v, fp1, fp2, t );
					}		
				}

				// Increment frame data pointers
				fp1+=num; fp2+=num;
			}
		} else {
			//  Assume float data is in motion's channel order
			float* v = buffer;  // point to the start of all floats
			for ( int i=0; i<csize; i++ )
			{
				if (isAdditive)
				{
					if (_channels[i].type == SkChannel::Quat)
					{
						float addQuat[4];
						num = _channels[i].interp ( addQuat, fp1, fp2, t );
						SrQuat orig(v[0], v[1], v[2], v[3]);
						SrQuat add(addQuat[0], addQuat[1], addQuat[2], addQuat[3]);
						SrQuat final = orig * add;
						for (int x = 0; x < 4; x++)
							v[x] = final.getData(x);
					}
					else if (num <= 3)
					{
						float vec[3];
						num = _channels[i].interp ( vec, fp1, fp2, t );
						for (int x = 0; x < num; x++)
							v[x] += vec[x];
					}
					else
					{
						// more that 3 channels but not a quaternion? Don't add data, just replace it...
						num = _channels[i].interp ( v, fp1, fp2, t );
					}
				}
				else
				{
					num = _channels[i].interp ( v, fp1, fp2, t );
				}
				v+=num; fp1+=num; fp2+=num;
			}
		}
	}
	else
	{
		// Apply to channel joints
		float values[4]; // 4 is the max num of values per channel
		for ( int i=0; i<csize; i++ ) {
			if ( _channels[i].joint ) {
				_channels[i].interp ( values, fp1, fp2, t );
				float origValues[4];
				_channels[i].get(origValues);
				if (isAdditive)
				{
					if (_channels[i].type == SkChannel::Quat)
					{
						SrQuat orig(origValues[0], origValues[1], origValues[2], origValues[3]);
						SrQuat add(values[0], values[1], values[2], values[3]);
						SrQuat final = orig * add;
						float finalVal[4];
						for (int x = 0; x < 4; x++)
							finalVal[x] = final.getData(x);
						_channels[i].set(finalVal);
					}
					else if (num <= 3)
					{
						float vec[3];
						num = _channels[i].interp ( vec, fp1, fp2, t );
						for (int x = 0; x < num; x++)
							origValues[x] += vec[x];
						_channels[i].set(origValues);
					}
					else
					{
						// more that 3 channels but not a quaternion? Don't add data, just replace it...
						num = _channels[i].set ( values );
					}
				}
				else
				{
					num = _channels[i].set ( values );
				}
			} else {
				num = _channels[i].size();
			}

			fp1+=num; fp2+=num;
		}
	}
}



void SkMotion::apply ( float t,  
	float* buffer, SrBuffer<int>* map_p,
	SkMotion::InterpType itype, int* lastframe, bool isAdditive, SmartBody::SBRetarget* retarget )
{
	// new version of apply skMotion
	int fsize=_frames.size();
	if ( fsize<=0 )
		return;
	if ( t!= 0.f && t<=_frames[0].keytime )	{
#if DEBUG_T
		LOG("SkMotion::apply NOTICE: t=%.16f < f[0]:%.16f \n", t, _frames[0].keytime );
#endif
		return;
	}

	if ( itype==CubicSpline )
		t = _cubic ( t, _frames[0].keytime, _frames[_frames.size() - 1].keytime );

#if DEBUG_T
	if ( t<_frames[0].keytime )	{
		LOG("SkMotion::apply ERR: cubic t=%.16f < f[0]:%.16f \n", t, _frames[0].keytime );
	}
#endif

	// optimize keytime search for sequenced calls to apply
	int fini=0;
	if ( lastframe )
		_last_apply_frame = *lastframe;
	if ( _last_apply_frame>0 && _last_apply_frame<fsize ) {
		if ( t>_frames[_last_apply_frame].keytime )
			fini=_last_apply_frame+1;
	}

	int f;
	for ( f=fini; f<fsize; f++ ) {
		if ( t<_frames[f].keytime )
			break;
	}

	if ( f==_frames.size() ) {
		// Apply last frame
		//apply_frame( f-1, buffer, map_p );
		//return;
		t = _frames[_frames.size()-1].keytime;
		f = _frames.size()-1;
	}	

	if (_frames.size() > 1)
	{
		f--;
	}	
	_last_apply_frame = f;
	if ( lastframe )
		*lastframe = _last_apply_frame;
	int nxtFrame = _frames.size() > 1 ? f+1 : f;

	float* fp1 = _frames[f].posture;
	float* fp2 = _frames[nxtFrame].posture;
	// convert t to [0,1] according to the keytimes:
	t = _frames.size() > 1 ? (t-_frames[f].keytime) / (_frames[f+1].keytime-_frames[f].keytime) : 0.f;

#if DEBUG_T
	if ( t<0.0 )	{
		LOG("SkMotion::apply ERR: mapped t=%.16f < 0.0 \n", t );
	}
#endif

	//sr_out<<"t: "<<t<<" frames: "<<f<<srspc<<(f+1)<<"\n";
	int num;
	int csize = _channels.size();

	
	float outValue[4], origValue[4], newValue[4]; // at most 4 
	// Apply to float* buffer
	float* v = NULL;
	int sum = 0;		
	for ( int i=0; i<csize; i++ ) {
	// Channel size
		num = _channels[i].size();
		// Find mapped buffer location
		int index = i; 
		if (buffer)
		{
			if (map_p)
			{
				index = map_p->get( i );
				v = buffer + index;
			}
			else
			{
				v = buffer + sum;
			}
			for (int x=0;x<num;x++)
				origValue[x] = v[x];				
		}
		else if (_channels[i].joint)
		{
			_channels[i].get(origValue);						
		}
		else 
		{
			index = -1;
		}

		_channels[i].interp ( newValue, fp1, fp2, t );
		if( index >= 0 ) {
			if (_channels[i].type == SkChannel::Quat) // rotation channel
			{				
				SrQuat orig(origValue[0], origValue[1], origValue[2], origValue[3]);
				SrQuat add(newValue[0], newValue[1], newValue[2], newValue[3]);
				SrQuat final = isAdditive ? orig*add : add; 	
				if (retarget)
					final = retarget->applyRetargetJointRotation(_channels.mappedName(i),final);
				for (int x = 0; x < 4; x++)
					outValue[x] = final.getData(x);
			}
			else if (num <= 3) // positional channel
			{				
				for (int x = 0; x < num; x++)
				{
					outValue[x] = isAdditive? origValue[x]+newValue[x] : newValue[x];
					if (retarget)
						outValue[x] = retarget->applyRetargetJointTranslation(_channels.mappedName(i),outValue[x]);
				}
			}
			else
			{
				// more that 3 channels but not a quaternion? Don't add data, just replace it...
				for (int x = 0; x < num; x++)
				{
					outValue[x] = newValue[x];
					if (retarget)
						outValue[x] = retarget->applyRetargetJointTranslation(_channels.mappedName(i),outValue[x]);
				}
			}
			if (buffer)
			{
				for (int x = 0; x < num; x++)
					v[x] = outValue[x];
			}
			else
			{
				_channels[i].set(outValue);
			}
		}	
		// Increment frame data pointers
		fp1+=num; fp2+=num; sum+=num;
	}			
		
#if 0
	else
	{
		// Apply to channel joints
		float values[4]; // 4 is the max num of values per channel
		for ( int i=0; i<csize; i++ ) {
			if ( _channels[i].joint ) {
				_channels[i].interp ( values, fp1, fp2, t );
				float origValues[4];
				_channels[i].get(origValues);
				if (_channels[i].type == SkChannel::Quat)
				{
					SrQuat orig(origValues[0], origValues[1], origValues[2], origValues[3]);
					SrQuat add(values[0], values[1], values[2], values[3]);
					SrQuat final = orig * add;
					float finalVal[4];
					for (int x = 0; x < 4; x++)
						finalVal[x] = final.getData(x);
					_channels[i].set(finalVal);
				}
				else if (num <= 3)
				{
					float vec[3];
					num = _channels[i].interp ( vec, fp1, fp2, t );
					for (int x = 0; x < num; x++)
						origValues[x] += vec[x];
					_channels[i].set(origValues);
				}
				else
				{
					// more that 3 channels but not a quaternion? Don't add data, just replace it...
					num = _channels[i].set ( values );
				}				
				
			} else {
				num = _channels[i].size();
			}

			fp1+=num; fp2+=num;
		}
	}
#endif 
}

// static 
const char* SkMotion::interp_type_name ( InterpType type ) {
	switch ( type ) {
		case CubicSpline :
			return "CubicSpline";
		default :
			return "Linear";
    }
}

// static 
SkMotion::InterpType SkMotion::interp_type_name ( const char* type ) {
   if ( sr_compare(type,"CubicSpline")==0 )
	   return CubicSpline;
   return Linear;
 }

void SkMotion::operator = ( const SkMotion& m )
 {
   disconnect();
   init();
   SkMotion& m2 = const_cast<SkMotion&>(m);
   const std::string& fromName = m2.getName();
   setName(fromName);

   int i;
   _postsize = m._postsize;
   _skeleton = m._skeleton;
   _floatbuffer = m._floatbuffer;
   _channels = m._channels;

   int fsize = m._frames.size();
   _frames.resize( fsize );
   _frames.clear();
   for ( i=0; i<fsize; i++ )
    { insert_frame ( i, m._frames[i].keytime );
      sr_buffer_copy ( _frames[i].posture, sizeof(float), _postsize, m._frames[i].posture, _postsize );
    }
 }

void SkMotion::move_keytimes ( float startkt )
 {
    if ( _frames.size()==0 ) return;
   
   float diff = _frames[0].keytime-startkt;
   if ( diff==0 ) return;

   for (size_t i=0; i<_frames.size(); i++ )
    { _frames[i].keytime -= diff;
    }
 }

static float _correct ( float a1, float a2 )
 {
   if ( SR_DIST(a1,a2)<=srpi ) return a2;

   if ( a2>a1 )
    return _correct ( a1, a2-sr2pi );
   else
    return _correct ( a1, a2+sr2pi );
 }

void SkMotion::correct_euler_angles ()
 {
   int frsize = _frames.size();
   
   if ( frsize<=1 ) return;

   SkChannel::Type type;
   int i, j, p=0;
   int chsize = _channels.size();

   for ( i=0; i<chsize; i++ )
    { type = _channels.type(i);
      if ( type<SkChannel::XRot || type>SkChannel::ZRot )
       { p += SkChannel::size ( type );
       }
      else
       { for ( j=1; j<frsize; j++ )
          _frames[j].posture[p] = _correct ( _frames[j-1].posture[p], _frames[j].posture[p] );
         p++;
       }
    }
 }

void SkMotion::change_channel_values ( int f1, int f2, int channel, float mfactor, const float* offset )
 {
   int fsize = _frames.size();
   if ( fsize==0 ) return;
   f2 = SR_BOUND(f2,0,(fsize-1));
   f1 = SR_BOUND(f1,0,f2);
   SkChannel& ch = _channels[channel];
   int fp = _channels.float_position ( channel );
   int f;
   for ( f=f1; f<=f2; f++ )
    { ch.change_values ( _frames[f].posture+fp, mfactor, offset );
    }
 }

void SkMotion::registerAnimation()
{
	if (_frames.size() == 0)
		return;

	std::string jname("base");
	// get the offset from the first frame
	SkChannelArray& channels = this->channels();
	int xPos= channels.search(jname, SkChannel::XPos);
	int yPos = channels.search(jname, SkChannel::YPos);
	int zPos = channels.search(jname, SkChannel::ZPos);
	int qPos = channels.search(jname, SkChannel::Quat);
	
	if (xPos == -1 || yPos == -1 || zPos == -1)
	{
		LOG("No base position found in motion %s, cannot register.", this->getName().c_str());
		return;
	}

	if (qPos == -1)
	{
		LOG("No base orientation found in motion %s, cannot register.", this->getName().c_str());
		return;
	}

	_frameOffset.clear();
	_frameOrientation.clear();
	for (size_t f = 0; f < _frames.size(); f++)
	{
		SkMotion::Frame& frame = _frames[f];
		_frameOffset.push_back(SrVec(frame.posture[xPos], frame.posture[yPos], frame.posture[zPos]));
		_frameOrientation.push_back(SrQuat(frame.posture[qPos], frame.posture[qPos + 1], frame.posture[qPos + 2], frame.posture[qPos + 3]));

		// remove the position and orientation from the motion
		frame.posture[xPos] = 0;
		frame.posture[yPos] = 0;
		frame.posture[zPos] = 0;
		frame.posture[qPos + 0] = 1;
		frame.posture[qPos + 1] = 0;
		frame.posture[qPos + 2] = 0;
		frame.posture[qPos + 3] = 0;
	}

	_isRegistered = true;
}

	void SkMotion::unregisterAnimation()
	{
		if (_frames.size() == 0)
		return;

	std::string jname("base");
	// get the offset from the first frame
	SkChannelArray& channels = this->channels();
	int xPos= channels.search(jname, SkChannel::XPos);
	int yPos = channels.search(jname, SkChannel::YPos);
	int zPos = channels.search(jname, SkChannel::ZPos);
	int qPos = channels.search(jname, SkChannel::Quat);
	
	if (xPos == -1 || yPos == -1 || zPos == -1)
	{
		LOG("No base position found in motion %s, cannot register.", this->getName().c_str());
		return;
	}

	if (qPos == -1)
	{
		LOG("No base orientation found in motion %s, cannot register.", this->getName().c_str());
		return;
	}

	for (size_t f = 0; f < _frames.size(); f++)
	{
		SkMotion::Frame& frame = _frames[f];

		SrVec& position = _frameOffset[f];
		SrQuat& orientation = _frameOrientation[f];

		// return the position and orientation to the motion
		frame.posture[xPos] = position.x;
		frame.posture[yPos] = position.y;
		frame.posture[zPos] = position.z;
		frame.posture[qPos + 0] = orientation.w;
		frame.posture[qPos + 1] = orientation.x;
		frame.posture[qPos + 2] = orientation.y;
		frame.posture[qPos + 3] = orientation.z;
	}

	_isRegistered = false;
	}

	bool SkMotion::isRegistered()
	{
		return _isRegistered;
	}

	void SkMotion::setFrameOrientation(std::vector<SrQuat>& orientation)
	{
		_frameOrientation.resize(orientation.size());
		_frameOrientation = orientation;
	}
	std::vector<SrQuat>& SkMotion::getFrameOrientation()
	{
		return _frameOrientation;
	}

	void SkMotion::setFrameOffset(std::vector<SrVec>& offset)
	{
		_frameOffset.resize(offset.size());
		_frameOffset = offset;
	}

	std::vector<SrVec>& SkMotion::getFrameOffset()
	{
		return _frameOffset;
	}


/*
// static:
struct JT { SkJoint* j; char t[7]; float v[7]; };

static void equalize ( SrArray<JT>& used, SkMotion* m )
 {
   SkJoint* joint;
   SkChannel::Type type;

   int c=0;
   int i, j;
   for ( i=0; i<used.size(); i++ )
    { joint = used[i].j;
      if ( !joint ) continue;
      for ( j=0; j<7; j++ )
       { if ( !used[i].t[j] ) continue;
         type = (SkChannel::Type)j;
         if ( m->channel_joint(c)!=joint || m->channel_type(c)!=type ) // not there?
           m->add_channel ( c, joint->name(), type, used[i].v[j] ); // add it.
         c++;
       }
    }
 }
    *! Makes all motions in the array to have the same number of channels.
        Note: the motions are connected to sk and then disconnected. *
    static void equalize_channels ( SkSkeleton* sk, SrArray<SkMotion*> motions );

void SkMotion::equalize_channels ( SkSkeleton* sk, SrArray<SkMotion*> motions )
 {
   if ( !sk || motions.size()<=1 ) return;

   int i, j, type;
   SkJoint* joint;
   const SrArray<SkJoint*>& joints = sk->joints();
   SrArray<JT> used;

   // initialize "used dofs" array:
   used.size ( joints.size() );
   for ( i=0; i<used.size(); i++ )
    { used[i].j=0;
      for ( j=0; j<7; j++ ) { used[i].t[j]=0; used[i].v[j]=5; } // 5 value is to debug
    }
   
   // mark used dofs of all motions:
   for ( i=0; i<motions.size(); i++ )
    { motions[i]->connect(sk);

      for ( j=0; j<motions[i]->channels(); j++ )
       { joint = motions[i]->channel_joint(j);
         if ( !joint ) continue;
         type = (int)motions[i]->channel_type(j);
         used[ joint->index() ].j = joint;
         used[ joint->index() ].t[type] = 1;
         used[ joint->index() ].v[type] = motions[i]->posture(0)[j];
       }
    }

   for ( i=0; i<motions.size(); i++ )
    { equalize ( used, motions[i] );
      motions[i]->disconnect();
    }
 }

    Makes all motions in the array to have the same number of frames.
    static void equalize_frames ( SrArray<SkMotion*> motions );
void SkMotion::equalize_frames ( SrArray<SkMotion*> motions )
 {
// resampling
 }
*/

//============================ End of File ===========================

const bool ascendingTime(SmartBody::SBMotionEvent* a, SmartBody::SBMotionEvent* b)
{
	return (a->getTime() < b->getTime());
}

void SkMotion::addMotionEvent(SmartBody::SBMotionEvent* motionEvent)
{
	_motionEvents.push_back(motionEvent);
	// make sure that the motion events are ordered by time
	std::sort(_motionEvents.begin(), _motionEvents.end(), ascendingTime);
}

std::vector<SmartBody::SBMotionEvent*>& SkMotion::getMotionEvents()
{
	return _motionEvents;
}

SkMotion* SkMotion::buildSmoothMotionCycle( float timeInterval )
{
	SkChannelArray& mchan_arr = this->channels();
	SmartBody::SBMotion* originalMotion = dynamic_cast<SmartBody::SBMotion*>(this);
	SmartBody::SBMotion *smooth_p = new SmartBody::SBMotion();
	smooth_p->setMotionSkeletonName(originalMotion->getMotionSkeletonName());	
	srSynchPoints sp(synch_points);
	smooth_p->synch_points = sp;
	smooth_p->init( mchan_arr );
	int num_f = this->frames();	
	int intervalFrames = (int)(this->frames()*timeInterval/duration());
	std::vector<float> discontinuity;
	// copy the motion
	for (int i = 0; i < num_f; i++)
	{
		smooth_p->insert_frame( i, this->keytime( i ) );
		float *ref_p = this->posture( i );
		float *new_p = smooth_p->posture( i );
		// go through each channel, and flip the channel value when necessary
		for (int k=0;k<mchan_arr.size();k++)
		{
			SkChannel& chan = mchan_arr[k];
			const std::string& jointName = mchan_arr.mappedName(k);
			int index = mchan_arr.float_position(k);			
			{
				for (int n=0;n<chan.size();n++)
					new_p[index+n] = ref_p[index+n];
			}
		}
	}

	// smooth the first and last s frames
	float *first_p = this->posture( 0 );
	float *last_p  = this->posture( num_f - 1 );	
	for (int i = 0; i< num_f; i++)
	{
		if (i > intervalFrames && i < num_f - intervalFrames)
			continue;	
		float *new_p = smooth_p->posture( i );
		int curFrame = (i <= intervalFrames) ? i + intervalFrames : ( i + intervalFrames - num_f );
		float ratio = ((float)curFrame)/intervalFrames;
		float yf = curFrame < intervalFrames ? 0.5f*ratio*ratio : -0.5f*ratio*ratio + 2.f*ratio - 2.f;
		for (int k=0;k<mchan_arr.size();k++)
		{
			SkChannel& chan = mchan_arr[k];
			const std::string& jointName = mchan_arr.mappedName(k);
			bool isPos = chan.type <= SkChannel::ZPos; 
			if (jointName == "base") // we ignore the base when smoothing
				continue;
			int index = mchan_arr.float_position(k);		
			if (chan.type == SkChannel::Quat) // not sure if this is the best way of doing this
			{
				SrQuat q1 = SrQuat( first_p[ index ], first_p[ index + 1 ], first_p[ index + 2 ], first_p[ index + 3 ] );
				SrQuat q2 = SrQuat( last_p[ index ], last_p[ index + 1 ], last_p[ index + 2 ], last_p[ index + 3 ] );
				//SrVec diff = curFrame < intervalFrames ? (q2.inverse()*q1).axisAngle()*yf : (q1.inverse()*q2).axisAngle()*(yf);
				SrQuat targetQ = curFrame < intervalFrames ? q1 : q2;
				SrQuat curQ = SrQuat( new_p[ index ], new_p[ index + 1 ], new_p[ index + 2 ], new_p[ index + 3 ] );
				SrQuat finalQ = slerp(curQ,targetQ,fabs(yf));			
				new_p[ index + 0 ] = (float)finalQ.w;
				new_p[ index + 1 ] = (float)finalQ.x;
				new_p[ index + 2 ] = (float)finalQ.y;
				new_p[ index + 3 ] = (float)finalQ.z;
			}
			else
			{
				for (int j=0;j<chan.size();j++)
				{
					float diff = first_p[index + j] - last_p[index + j]; // difference between posture			
					new_p[index+j] += yf*diff;
				}	
			}					
		}
	}
	return smooth_p;
}

void SkMotion::convertBoneOrientation( std::string &pjointName, SkSkeleton* interSk, SkSkeleton* tempSrcSk, std::vector<std::string>& endJoints )
{	
	if (std::find(endJoints.begin(),endJoints.end(),pjointName) != endJoints.end())
		return;

	SkJoint* pjoint = interSk->search_joint(pjointName.c_str());
	SkJoint* srcjoint = tempSrcSk->search_joint(pjointName.c_str());
	if (!srcjoint || !pjoint) return;

	std::vector<SrVec> srcDirList, origSrcDirList;
	std::vector<SrVec> dstDirList;
	SrVec pos = pjoint->gmat().get_translation();	

#if 0
	for (int i=0; i< srcjoint->num_children(); i++)
	{
		SkJoint* child = srcjoint->child(i);
		if (std::find(endJoints.begin(),endJoints.end(),child->name()) != endJoints.end())
			continue;
		if (!interSk->search_joint(child->name().c_str()))
			continue;

		SrVec srcdir = tempSrcSk->boneGlobalDirection(pjoint->name(),child->name());
		SrVec dstdir = interSk->boneGlobalDirection(pjoint->name(),child->name());	
		//jointQueues.push(child->name());
		srcDirList.push_back(srcdir);			
		dstDirList.push_back(dstdir);
		//dir += gdir;
	}
#else
	std::queue<std::string> childJointNameQueue;
	for (int i=0; i< srcjoint->num_children(); i++)
		childJointNameQueue.push(srcjoint->child(i)->getMappedJointName());
	while (!childJointNameQueue.empty())
	{
		std::string childName = childJointNameQueue.front(); childJointNameQueue.pop();
		SkJoint* child = tempSrcSk->search_joint(childName.c_str());
		//if (std::find(endJoints.begin(),endJoints.end(),childName) != endJoints.end())
		//	continue;
		SkJoint* interSkChild = interSk->search_joint(childName.c_str());
		if (interSkChild)
		{
			SrVec srcdir = tempSrcSk->boneGlobalDirection(pjoint->getMappedJointName(),childName);
			SrVec dstdir = interSk->boneGlobalDirection(pjoint->getMappedJointName(),childName);	
			//LOG("pjoint = %s, childName = %s",pjoint->name().c_str(), childName.c_str());

			//LOG("src dir = %f %f %f, dst dir = %f %f %f",srcdir[0],srcdir[1],srcdir[2], dstdir[0],dstdir[1],dstdir[2]);


			//jointQueues.push(child->name());
			srcDirList.push_back(srcdir);			
			dstDirList.push_back(dstdir);			
			break;
		}
		for (int k=0;k<child->num_children();k++)
		{
			childJointNameQueue.push(child->child(k)->getMappedJointName());
		}
	}
#endif	

	SrQuat jointRotation;
	//if (srcDirList.size() == 1 || srcDirList.size() == 2)
	{

		SrVec srcDir, dstDir;
		//for (unsigned int k=0;k<srcDirList.size();k++)
		for (unsigned int k=0;k<srcDirList.size();k++)
		{
			if (k>=1) continue;
			srcDir += srcDirList[k];
			dstDir += dstDirList[k];
		}
		srcDir.normalize(); dstDir.normalize();

		double dot_v = dot(srcDir, dstDir);
		if(dot_v >= 0.9999995000000f) 
		{				
			dot_v = 1.0f;
		}
		else if(dot_v < -1.0f) dot_v = -1.0f;
		double angle = acos(dot_v);			
		
		SrVec rotAxis = cross(dstDir, srcDir);
		rotAxis.normalize();
		SrVec rotAxisAngle = rotAxis*(float)angle; // global rotation
		SrQuat alignQuat = SrQuat(rotAxisAngle);
		SrMat alignMat;
		SrMat prerotMat;				
		SrMat pmatInv;

		//LOG("joint = %s, src dir = %f %f %f, dst dir = %f %f %f, rot axist = %f %f %f, angle = %f",pjointName.c_str(), srcDir[0],srcDir[1],srcDir[2], dstDir[0],dstDir[1],dstDir[2], rotAxis[0], rotAxis[1], rotAxis[2], sr_todeg(angle));


		SrMat gmatRot = pjoint->gmat().get_rotation();				
		if (pjoint->parent())
			pmatInv = pjoint->parent()->gmat().get_rotation().inverse();
		pjoint->quat()->prerot().get_mat(prerotMat);
		alignQuat.get_mat(alignMat);
		//SrMat gmatInv = prerotMat.inverse()*pjoint->parent()->gmat().get_rotation();//pjoint->gmat().get_rotation();//(prerotMat*pjoint->parent()->gmat()).inverse();//pjoint->gmatZero().get_rotation().inverse();//(prerotMat*pjoint->parent()->gmat()).inverse();
		//rotAxisAngle = rotAxisAngle*gmatInv;
		//SrMat quatMat = prerotMat.inverse()*gmatRot*alignMat*pmatInv;//alignMat*gmatRot.inverse();//prerotMat.inverse()*gmatRot*alignMat*pmat;
		SrMat quatMat = gmatRot*alignMat*gmatRot.inverse();
		//SrQuat quat = SrQuat(prerotMat.inverse())*SrQuat(gmatRot)*alignQuat*SrQuat(pmatInv);
		//SrVec alignRotAxisAngle = rotAxisAngle*gmatRot.inverse();								
		//SrVec alignRotAxisAngle = rotAxisAngle*pmatInv;		
		SrQuat quat = pjoint->quat()->rawValue()*SrQuat(quatMat);
		//SrQuat quat = pjoint->quat()->rawValue()*SrQuat(alignRotAxisAngle);//*pjoint->quat()->rawValue();
		//jointRotationMap[pjointName] = quat;
		pjoint->quat()->value(quat);
		pjoint->set_lmat_changed();
		pjoint->update_gmat();	
	}
}


SkMotion* SkMotion::buildRetargetMotionV2( SkSkeleton* sourceSk, SkSkeleton* targetSk, std::vector<std::string>& endJoints, std::vector<std::string>& relativeJoints, std::map<std::string, SrVec>& offsetJoints )
{
	SkChannelArray& mchan_arr = this->channels();
	SkSkeleton* interSk = new SkSkeleton(targetSk); // copy for an intermediate skeleton
	SkSkeleton* tempSrcSk = new SkSkeleton(sourceSk);

	std::vector<std::string> stopJoints;	
	stopJoints.push_back("skullbase");
	//stopJoints.push_back("l_wrist");
	//stopJoints.push_back("r_wrist");
	// update the global matrices
	tempSrcSk->clearJointValues();
	interSk->clearJointValues();
	tempSrcSk->invalidate_global_matrices();
	tempSrcSk->update_global_matrices();
	interSk->invalidate_global_matrices();
	interSk->update_global_matrices();

#if 0 // don't apply root pre-rotation ( still experimental )
	SrVec srcFaceDir = tempSrcSk->getFacingDirection();
	SrVec tgtFaceDir = interSk->getFacingDirection();
	SrQuat offsetRot = SrQuat(srcFaceDir,tgtFaceDir);
	sr_out << "srcFaceDir = " << srcFaceDir;
	sr_out << "tgtFaceDir = " << tgtFaceDir;
	sr_out << "offset rot = " << offsetRot << srnl;
	tempSrcSk->root()->quat()->prerot(offsetRot);
#endif
	
	SkMotion *retarget_p = new SmartBody::SBMotion();	
	//srSynchPoints sp(synch_points);
	retarget_p->synch_points.copy_points(synch_points);// = sp;
	retarget_p->init( mchan_arr ); // use the target channels instead
	int num_f = this->frames();

// 	for (unsigned int i=0;i<relativeJoints.size();i++)
// 	{
// 		LOG("relativeJoint %d = %s",i,relativeJoints[i].c_str());
// 	}

	std::map<std::string, SrQuat> jointRotationMap;
	std::queue<std::string> jointQueues;
	std::string rootName = "base";
	//jointQueues.push(interSk->root()->name());	
	jointQueues.push(rootName);
	while (!jointQueues.empty())
	{
		std::string pjointName = jointQueues.front();		
		jointQueues.pop();				
		if (std::find(stopJoints.begin(),stopJoints.end(),pjointName) != stopJoints.end())
			continue;

		SkJoint* srcJoint = tempSrcSk->search_joint(pjointName.c_str());
		SkJoint* targetJoint = interSk->search_joint(pjointName.c_str());
		bool isRelativeJoint = false;
		
		// just copy over joint quat if that is a root or relative joint
		//if (pjointName == interSk->root()->name() || std::find(relativeJoints.begin(),relativeJoints.end(), pjointName) != relativeJoints.end())
		if (pjointName == rootName || std::find(relativeJoints.begin(),relativeJoints.end(), pjointName) != relativeJoints.end())
			isRelativeJoint = true;
		if ( isRelativeJoint && srcJoint && targetJoint) 
		{
			// don't change the t-pose for these joints
			for (int i=0;i<targetJoint->num_children();i++)
			{					
				jointQueues.push(targetJoint->child(i)->jointName());
			}
		}
		else
		{
			//LOG("pjoint name = %s",pjointName.c_str());
			convertBoneOrientation(pjointName, interSk, tempSrcSk, endJoints);
			interSk->invalidate_global_matrices();
			interSk->update_global_matrices();
			SkJoint* pjoint = interSk->search_joint(pjointName.c_str());	
			if (!pjoint)
				continue;
			//SrQuat newPreRot = pjoint->quat()->prerot()*pjoint->quat()->rawValue();
			//pjoint->quat()->prerot(newPreRot); // reset pre-rotation to
			//pjoint->quat()->value(SrQuat()); // cleanup quat value
			for (int i=0; i< pjoint->num_children(); i++)
			{
				//LOG("target joint = %s, child = %s",pjoint->name().c_str(), pjoint->child(i)->name().c_str());
				SkJoint* child = pjoint->child(i);
				jointQueues.push(child->jointName());
			}
		}				
	}
	float heightRatio = (interSk->getBaseHeight("base")/tempSrcSk->getBaseHeight("base"));//*0.99f;
	//LOG("height ratio = %f", heightRatio);
	for (int i = 0; i < num_f; i++)
	{
		retarget_p->insert_frame(i, this->keytime(i));		
		float *ref_p = this->posture( i );
		float *new_p = retarget_p->posture( i );
		//tempSrcSk.update_global_matrices(); // update global values for the temp source skeleton
		for (int k=0;k<mchan_arr.size();k++)
		{
			SkChannel& chan = mchan_arr[k];			
			const std::string& jointName = mchan_arr.mappedName(k);				
			// map pre-rotation
			int index = mchan_arr.float_position(k);
			if (chan.type == SkChannel::Quat)
			{
				// flip rotation for y,z axis
				SrQuat q_orig = SrQuat( ref_p[ index ], ref_p[ index + 1 ], ref_p[ index + 2 ], ref_p[ index + 3 ] );
				SrQuat final_q = q_orig;
				//SrVec qa = q_orig.axisAngle();
				//SrVec qa_rot = qa*srcToTargetRot;	
				//SrQuat final_q = jointRot;//*SrQuat(qa_rot);	
				SkJoint* srcJoint = tempSrcSk->search_joint(jointName.c_str());
				SkJoint* targetJoint = interSk->search_joint(jointName.c_str());			
				if (srcJoint && targetJoint)
				{					
					SrQuat gsrc = SrQuat(srcJoint->gmat());
					SrQuat gdst = SrQuat(targetJoint->gmat());
					SrQuat protSrc = srcJoint->quat()->rawValue();
					SrQuat protDst = targetJoint->quat()->rawValue();//targetJoint->quat()->prerot();//targetJoint->quat()->rawValue();//targetJoint->quat()->prerot();
					//sr_out << "gsrc = " << gsrc << " gdst = " << gdst << srnl;
					final_q = protDst*gdst.inverse()*gsrc*protSrc.inverse()*q_orig*gsrc.inverse()*gdst;
					//final_q = protDst;//gdst.inverse()*gsrc*protSrc.inverse()*q_orig*gsrc.inverse()*gdst*protDst;
					final_q.normalize();
				}

				new_p[ index + 0 ] = (float)final_q.w;
				new_p[ index + 1 ] = (float)final_q.x;
				new_p[ index + 2 ] = (float)final_q.y;
				new_p[ index + 3 ] = (float)final_q.z;
			}
			else if (chan.type == SkChannel::XPos || chan.type == SkChannel::YPos || chan.type == SkChannel::ZPos)
			{
				// just copy over the translation for now
				float chanValue = ref_p[ index ];				
				new_p[ index ] = chanValue*heightRatio;
				//LOG("jointName = %s, new_p = %f",jointName.c_str(), new_p[index]);
				// 				if (jointPosMap.find(jointName) != jointPosMap.end())
				// 				{
				// 					SrVec pos = jointPosMap[jointName];
				// 					new_p[index] = pos[chan.type - SkChannel::XPos];
				// 				}
			}
		}		
	}	
	delete tempSrcSk;
	delete interSk;
	return retarget_p;
}

#if 0
SkMotion* SkMotion::buildRetargetMotion( SkSkeleton* sourceSk, SkSkeleton* targetSk, std::vector<std::string>& endJoints, 
										 std::vector<std::string>& relativeJoints, std::map<std::string, SrVec>& offsetJoints )
{
	SkChannelArray& mchan_arr = this->channels();
	SkSkeleton* interSk = new SkSkeleton(targetSk); // copy for an intermediate skeleton
	SkSkeleton* tempSrcSk = new SkSkeleton(sourceSk);
	
	// update the global matrices
	tempSrcSk->invalidate_global_matrices();
	tempSrcSk->update_global_matrices();
	interSk->invalidate_global_matrices();
	interSk->update_global_matrices();	

	SkMotion *retarget_p = new SmartBody::SBMotion();
	srSynchPoints sp(synch_points);
	retarget_p->synch_points = sp;
	retarget_p->init( mchan_arr ); // use the target channels instead
	int num_f = this->frames();

	this->connect(tempSrcSk);
	for (int i = 0; i < num_f; i++)
	{
		std::map<std::string, SrQuat> jointRotationMap;
		std::map<std::string, SrMat> jointPreRot;
		std::map<std::string, SrVec> jointPosMap;
		std::queue<std::string> jointQueues;
		std::string rootName = "base";
		jointQueues.push(rootName);	

		float *ref_p = this->posture( i );
		// clean up intermediate skeleton
		for (unsigned int j=0;j<interSk->joints().size();j++)
		{
			SkJoint* joint = interSk->joints()[j];			
			SrQuat offsetQuat;
			if (offsetJoints.find(joint->name()) != offsetJoints.end())
			{
				SrVec offsetRot = offsetJoints[joint->name()];
				offsetQuat = SrQuat(offsetRot*srDEGRAD);					
			}
			joint->quat()->value(offsetQuat);
		}
		this->apply_frame(i);		

		while (!jointQueues.empty())
		{
			std::string pjointName = jointQueues.front();
			jointQueues.pop();		
			// do not process if it is the end joints
// 			bool addChildren = true;
// 			if (std::find(endJoints.begin(),endJoints.end(),pjointName) != endJoints.end())
// 				addChildren = false;
			//if (std::find(endJoints.begin(),endJoints.end(),pjointName) != endJoints.end())
			//	continue;
			
			SkJoint* srcJoint = tempSrcSk->search_joint(pjointName.c_str());
			SkJoint* targetJoint = interSk->search_joint(pjointName.c_str());
			bool isRelativeJoint = false;
			// just copy over joint quat if that is a root or relative joint
			//if (pjointName == interSk->root()->name() || std::find(relativeJoints.begin(),relativeJoints.end(), pjointName) != relativeJoints.end())
			if (pjointName == rootName || std::find(relativeJoints.begin(),relativeJoints.end(), pjointName) != relativeJoints.end())
				isRelativeJoint = true;
			if ( isRelativeJoint && srcJoint && targetJoint) // directly copy over the root joint rotation
			{
				int chanID = mchan_arr.search(pjointName, SkChannel::Quat);
				int index = mchan_arr.float_position(chanID);
				SrMat rotMap = srcJoint->gmatZero()*targetJoint->gmatZero().inverse();
				rotMap = rotMap.get_rotation();
				SrQuat refQuat = SrQuat(ref_p[index],ref_p[index+1],ref_p[index+2],ref_p[index+3]);
				SrVec refAA = refQuat.axisAngle()*rotMap;
				jointRotationMap[pjointName] = SrQuat(refAA);
				jointPreRot[pjointName] = rotMap;
				for (int i=0;i<targetJoint->num_children();i++)
				{
					jointQueues.push(targetJoint->child(i)->name());
				}

				int posChanID = mchan_arr.search(pjointName, SkChannel::XPos);
				if (posChanID != -1)
				{
					int posIndex = mchan_arr.float_position(posChanID);
					SrVec pos;
					for (int k=0;k<3;k++)
					{
						pos[k] = ref_p[posIndex+k];
					}
					pos = pos*rotMap;					
					jointPosMap[pjointName] = pos;
				}
			}
			else
			{
				convertBoneOrientation(pjointName, interSk, tempSrcSk, jointRotationMap, endJoints);								
				SkJoint* pjoint = interSk->search_joint(pjointName.c_str());
				for (int i=0; i< pjoint->num_children(); i++)
				{
					SkJoint* child = pjoint->child(i);
					jointQueues.push(child->name());
				}
			}			
		}


		retarget_p->insert_frame(i, this->keytime(i));				
		float *new_p = retarget_p->posture( i );		
		//tempSrcSk.update_global_matrices(); // update global values for the temp source skeleton
		for (int k=0;k<mchan_arr.size();k++)
		{
			SkChannel& chan = mchan_arr[k];
			const std::string& jointName = mchan_arr.name(k);
			SrQuat jointRot;	
			SrMat prerot;
			if (jointRotationMap.find(jointName) != jointRotationMap.end())
				jointRot = jointRotationMap[jointName];

			int index = mchan_arr.float_position(k);
			if (chan.type == SkChannel::Quat)
			{				
				SrQuat final_q = jointRot;
				new_p[ index + 0 ] = (float)final_q.w;
				new_p[ index + 1 ] = (float)final_q.x;
				new_p[ index + 2 ] = (float)final_q.y;
				new_p[ index + 3 ] = (float)final_q.z;
			}	
			else if (chan.type == SkChannel::XPos || chan.type == SkChannel::YPos || chan.type == SkChannel::ZPos)
			{
				// just copy over the translation for now
				float chanValue = ref_p[ index ];				
				new_p[ index ] = chanValue;
// 				if (jointPosMap.find(jointName) != jointPosMap.end())
// 				{
// 					SrVec pos = jointPosMap[jointName];
// 					new_p[index] = pos[chan.type - SkChannel::XPos];
// 				}
			}
		}	
	}
	this->disconnect();
	return retarget_p;
}


SkMotion* SkMotion::buildRetargetMotion3( SkSkeleton* sourceSk, SkSkeleton* targetSk )
{		
	SkChannelArray& mchan_arr = this->channels();
	SkSkeleton* interSk = new SkSkeleton(targetSk); // copy for an intermediate skeleton
	SkSkeleton* tempSrcSk = new SkSkeleton(sourceSk);
	
	std::vector<std::string> stopJoints;
	stopJoints.push_back("l_wrist");
	stopJoints.push_back("r_wrist");

	// update the global matrices
	tempSrcSk->invalidate_global_matrices();
	tempSrcSk->update_global_matrices();
	interSk->invalidate_global_matrices();
	interSk->update_global_matrices();

	SkMotion *retarget_p = new SmartBody::SBMotion();
	srSynchPoints sp(synch_points);
	retarget_p->synch_points = sp;
	retarget_p->init( mchan_arr ); // use the target channels instead
	int num_f = this->frames();


	std::map<std::string, SrQuat> jointRotationMap;
	std::queue<std::string> jointQueues;
	std::string rootName = "base";
	//jointQueues.push(interSk->root()->name());	
	jointQueues.push(rootName);
	while (!jointQueues.empty())
	{
		std::string pjointName = jointQueues.front();
		jointQueues.pop();		
		if (std::find(stopJoints.begin(),stopJoints.end(),pjointName) != stopJoints.end())
			continue;
		
		SkJoint* pjoint = interSk->search_joint(pjointName.c_str());
		SkJoint* srcjoint = tempSrcSk->search_joint(pjointName.c_str());
		std::vector<SrVec> srcDirList, origSrcDirList;
		std::vector<SrVec> dstDirList;
		SrVec pos = pjoint->gmat().get_translation();		
		if (srcjoint)
		{
			SrVec srcPos = srcjoint->gmat().get_translation();
			LOG("joint = %s, src pos = %f %f %f, dst pos = %f %f %f",pjointName.c_str(),srcPos[0],srcPos[1],srcPos[2],pos[0],pos[1],pos[2]);
		}
		for (int i=0; i< pjoint->num_children(); i++)
		{
			SkJoint* child = pjoint->child(i);
			SrVec srcdir = tempSrcSk->boneGlobalDirection(pjoint->name(),child->name());
			SrVec dstdir = interSk->boneGlobalDirection(pjoint->name(),child->name());
			SrVec srcDir1 = sourceSk->boneGlobalDirection(pjoint->name(),child->name());
			jointQueues.push(child->name());
			srcDirList.push_back(srcdir);			
			dstDirList.push_back(dstdir);
			//dir += gdir;
		}
		SrQuat jointRotation;
		//if (srcDirList.size() == 1 || srcDirList.size() == 2)
		{
			
			//SrVec srcDir = srcDirList[0];
			//SrVec srcDir1 = origSrcDirList[0];
			//SrVec dstDir = dstDirList[0];
			SrVec srcDir, dstDir;
			for (unsigned int k=0;k<srcDirList.size();k++)
			{
				srcDir += srcDirList[k];
				dstDir += dstDirList[k];
			}
			srcDir.normalize(); dstDir.normalize();

			//LOG("joint = %s, src dir = %f %f %f, dst dir = %f %f %f",pjointName.c_str(), srcDir[0],srcDir[1],srcDir[2], dstDir[0],dstDir[1],dstDir[2]);
			double dot_v = dot(srcDir, dstDir);
			if(dot_v >= 0.9999995000000f) 
			{				
				dot_v = 1.0f;
			}
			else if(dot_v < -1.0f) dot_v = -1.0f;
			double angle = acos(dot_v);			
			SrVec rotAxis = cross(dstDir, srcDir);
			rotAxis.normalize();
			SrVec rotAxisAngle = rotAxis*(float)angle; // global rotation
			SrQuat alignQuat = SrQuat(rotAxisAngle);
			SrMat alignMat;
			SrMat prerotMat;
			SrMat gmatRot = pjoint->gmat().get_rotation();
			SrMat pmatInv;
			if (pjoint->parent())
				pmatInv = pjoint->parent()->gmat().get_rotation().inverse();
			pjoint->quat()->prerot().get_mat(prerotMat);
			alignQuat.get_mat(alignMat);
			//SrMat gmatInv = prerotMat.inverse()*pjoint->parent()->gmat().get_rotation();//pjoint->gmat().get_rotation();//(prerotMat*pjoint->parent()->gmat()).inverse();//pjoint->gmatZero().get_rotation().inverse();//(prerotMat*pjoint->parent()->gmat()).inverse();
			//rotAxisAngle = rotAxisAngle*gmatInv;
			SrMat quatMat = prerotMat.inverse()*gmatRot*alignMat*pmatInv;//alignMat*gmatRot.inverse();//prerotMat.inverse()*gmatRot*alignMat*pmat;
			//SrQuat quat = SrQuat(prerotMat.inverse())*SrQuat(gmatRot)*alignQuat*SrQuat(pmatInv);
			SrVec alignRotAxisAngle = rotAxisAngle*gmatRot.inverse();
			//SrQuat quat = SrQuat(quatMat);
			SrQuat quat = SrQuat(alignRotAxisAngle);
			jointRotationMap[pjointName] = quat;
			pjoint->quat()->value(quat);
			pjoint->set_lmat_changed();
			//pjoint->update_gmat();
			interSk->invalidate_global_matrices();
			interSk->update_global_matrices();
			//LOG("skeleton align rotation, joint = %s, rotation = %f %f %f",pjointName.c_str(),rotAxisAngle[0],rotAxisAngle[1],rotAxisAngle[2]);			
			SrVec newDstDir = SrVec();
			for (int k=0;k<pjoint->num_children();k++)
			{
				SkJoint* child = pjoint->child(k);
				newDstDir += interSk->boneGlobalDirection(pjoint->name(),child->name());				
			}
			newDstDir.normalize();
			//sr_out << "align global rotation = " << alignQuat << ", local rotation = " << quat << ",  new dstDir = " << newDstDir << srnl << srnl;	
		}			
	}

	
	//_leftRightRot = SrQuat(lEyeJoint->gmatZero()*rEyeJoint->gmatZero().inverse());
	std::map<std::string, SrQuat> rotationMap;
	for (int k=0;k<mchan_arr.size();k++)
	{
		SkChannel& chan = mchan_arr[k];
		const std::string& jointName = mchan_arr.name(k);
		int index = mchan_arr.float_position(k);
		if (chan.type == SkChannel::Quat)
		{
			// flip rotation for y,z axis			
			SkJoint* srcJoint = sourceSk->search_joint(jointName.c_str());
			SkJoint* targetJoint = targetSk->search_joint(jointName.c_str());
			if (srcJoint && targetJoint)
			{
				SrQuat srcToTargetRot = SrQuat(srcJoint->gmatZero()*targetJoint->gmatZero().inverse());
				rotationMap[jointName] = srcToTargetRot;
			}
		}
	}	

	for (int i = 0; i < num_f; i++)
	{
		retarget_p->insert_frame(i, this->keytime(i));		
		float *ref_p = this->posture( i );
		float *new_p = retarget_p->posture( i );
		//tempSrcSk.update_global_matrices(); // update global values for the temp source skeleton
		for (int k=0;k<mchan_arr.size();k++)
		{
			SkChannel& chan = mchan_arr[k];
			const std::string& jointName = mchan_arr.name(k);
			// map pre-rotation
			SrQuat srcToTargetRot;			
			if (rotationMap.find(jointName) != rotationMap.end())
				srcToTargetRot = rotationMap[jointName];

			SrQuat jointRot;
			if (jointRotationMap.find(jointName) != jointRotationMap.end())
				jointRot = jointRotationMap[jointName];

			int index = mchan_arr.float_position(k);
			if (chan.type == SkChannel::Quat)
			{
				// flip rotation for y,z axis
				SrQuat q_orig = SrQuat( ref_p[ index ], ref_p[ index + 1 ], ref_p[ index + 2 ], ref_p[ index + 3 ] );
				SrVec qa = q_orig.axisAngle();
				SrVec qa_rot = qa*srcToTargetRot;	
				SrQuat final_q = jointRot;//*SrQuat(qa_rot);	
				new_p[ index + 0 ] = (float)final_q.w;
				new_p[ index + 1 ] = (float)final_q.x;
				new_p[ index + 2 ] = (float)final_q.y;
				new_p[ index + 3 ] = (float)final_q.z;
			}
		}		
	}	

	return retarget_p;
}


SkMotion* SkMotion::buildRetargetMotion2( SkSkeleton* sourceSk, SkSkeleton* targetSk )
{	
	SkChannelArray& mchan_arr = this->channels();
	SkSkeleton interSk; interSk.copy(targetSk); // copy for an intermediate skeleton
	SkSkeleton tempSrcSk; tempSrcSk.copy(sourceSk);
	SkMotion *retarget_p = new SmartBody::SBMotion();
	srSynchPoints sp(synch_points);
	retarget_p->synch_points = sp;
	retarget_p->init( mchan_arr ); // use the target channels instead
	int num_f = this->frames();
	this->connect(&tempSrcSk);

	//_leftRightRot = SrQuat(lEyeJoint->gmatZero()*rEyeJoint->gmatZero().inverse());
	std::map<std::string, SrQuat> rotationMap;
	for (int k=0;k<mchan_arr.size();k++)
	{
		SkChannel& chan = mchan_arr[k];
		const std::string& jointName = mchan_arr.name(k);
		int index = mchan_arr.float_position(k);
		if (chan.type == SkChannel::Quat)
		{
			// flip rotation for y,z axis			
			SkJoint* srcJoint = sourceSk->search_joint(jointName.c_str());
			SkJoint* targetJoint = targetSk->search_joint(jointName.c_str());
			if (srcJoint && targetJoint)
			{
				SrQuat srcToTargetRot = SrQuat(srcJoint->gmatZero()*targetJoint->gmatZero().inverse());
				rotationMap[jointName] = srcToTargetRot;
			}
		}
	}	

	for (int i = 0; i < num_f; i++)
	{
		retarget_p->insert_frame(i, this->keytime(i));		
		float *ref_p = this->posture( i );
		float *new_p = retarget_p->posture( i );
		//tempSrcSk.update_global_matrices(); // update global values for the temp source skeleton
		for (int k=0;k<mchan_arr.size();k++)
		{
			SkChannel& chan = mchan_arr[k];
			const std::string& jointName = mchan_arr.name(k);
			// map pre-rotation
			SrQuat srcToTargetRot;			
			if (rotationMap.find(jointName) != rotationMap.end())
				srcToTargetRot = rotationMap[jointName];

			int index = mchan_arr.float_position(k);
			if (chan.type == SkChannel::Quat)
			{
				// flip rotation for y,z axis
				SrQuat q_orig = SrQuat( ref_p[ index ], ref_p[ index + 1 ], ref_p[ index + 2 ], ref_p[ index + 3 ] );
				SrVec qa = q_orig.axisAngle();
				SrVec qa_rot = qa*srcToTargetRot;	
				SrQuat final_q = SrQuat(qa_rot);	
				new_p[ index + 0 ] = (float)final_q.w;
				new_p[ index + 1 ] = (float)final_q.x;
				new_p[ index + 2 ] = (float)final_q.y;
				new_p[ index + 3 ] = (float)final_q.z;
			}
		}		
	}
	this->disconnect();
	return retarget_p;
}
#endif

SkMotion* SkMotion::copyMotion()
{
	SkChannelArray& mchan_arr = this->channels();
	SkMotion* cpMotion = new SmartBody::SBMotion();
	srSynchPoints sp(synch_points);
	cpMotion->synch_points = sp;
	cpMotion->init( mchan_arr );
	int num_f = this->frames();
	for (int i = 0; i < num_f; i++)
	{
		cpMotion->insert_frame( i, this->keytime( i ) );
		float *ref_p = this->posture( i );
		float *new_p = cpMotion->posture( i );
		memcpy(new_p,ref_p,sizeof(float)*posture_size());
	}
	return cpMotion;	
}


/*
	This function can be made generic later on
*/
SkMotion* SkMotion::buildPrestrokeHoldMotion(float holdTime, SkMotion* idleMotion)
{
	int numHoldFrames = int(holdTime * this->frames() / this->duration());
	int strokeStartFrameId = int(synch_points.get_time(srSynchPoints::STROKE_START) * this->frames() / this->duration());

	SkChannelArray& mchan_arr = this->channels();
	SkMotion* newMotion = new SmartBody::SBMotion();
	newMotion->synch_points.set_time(	synch_points.get_time(srSynchPoints::START),
										synch_points.get_time(srSynchPoints::READY),
										synch_points.get_time(srSynchPoints::STROKE_START) + holdTime,
										synch_points.get_time(srSynchPoints::STROKE) + holdTime,
										synch_points.get_time(srSynchPoints::STROKE_STOP) + holdTime,
										synch_points.get_time(srSynchPoints::RELAX) + holdTime,
										synch_points.get_time(srSynchPoints::STOP) + holdTime
										);

	newMotion->init( mchan_arr );
	int num_f = this->frames();
	for (int i = 0; i < (num_f + numHoldFrames); ++i)
	{
		newMotion->insert_frame(i, float(i) * this->duration() / float(this->frames()));

		int refPosId = i;
		if (i >= strokeStartFrameId && i <= (strokeStartFrameId + numHoldFrames))
			refPosId = strokeStartFrameId;
		else if (i < strokeStartFrameId)
			refPosId = i;
		else
			refPosId = i - numHoldFrames;
		float *ref_p = this->posture(refPosId);
		float *new_p = newMotion->posture(i);
		memcpy(new_p,ref_p,sizeof(float)*posture_size());
	}
	return newMotion;	
}

/*
	This function can be made generic later on
*/
SkMotion* SkMotion::buildPoststrokeHoldMotion(float holdTime, std::vector<std::string>& joints, float scale, float freq, SkMotion* idleMotion)
{
	bool insertIdleMotion = (idleMotion != NULL) ? true : false;

	int numHoldFrames = int(holdTime * this->frames() / this->duration());
	int strokeEndFrameId = int(synch_points.get_time(srSynchPoints::STROKE_STOP) * this->frames() / this->duration());

	SkChannelArray& mchan_arr = this->channels();
	SkMotion* newMotion = new SmartBody::SBMotion();
	newMotion->synch_points.set_time(	synch_points.get_time(srSynchPoints::START),
										synch_points.get_time(srSynchPoints::READY),
										synch_points.get_time(srSynchPoints::STROKE_START),
										synch_points.get_time(srSynchPoints::STROKE),
										synch_points.get_time(srSynchPoints::STROKE_STOP),
										synch_points.get_time(srSynchPoints::RELAX) + holdTime,
										synch_points.get_time(srSynchPoints::STOP) + holdTime
										);

	newMotion->init( mchan_arr );
	int num_f = this->frames();
	for (int i = 0; i < (num_f + numHoldFrames); ++i)
	{
		newMotion->insert_frame(i, float(i) * this->duration() / float(this->frames() - 1));

		int refPosId = i;
		bool inHoldingPeriod = false;
		if (i >= strokeEndFrameId && i <= (strokeEndFrameId + numHoldFrames))
		{
			if (insertIdleMotion)	// insert idle motion frames
			{
				int numIdleFrames = idleMotion->frames();
				refPosId = (i - strokeEndFrameId) % numIdleFrames;
			}
			else
				refPosId = strokeEndFrameId;
			inHoldingPeriod = true;
		}
		else if (i < strokeEndFrameId)
			refPosId = i;
		else
			refPosId = i - numHoldFrames;

		float* ref_p;
		if (inHoldingPeriod && insertIdleMotion)
			ref_p = idleMotion->posture(refPosId);
		else
			ref_p = this->posture(refPosId);

		float* new_p = newMotion->posture(i);
		memcpy(new_p,ref_p,sizeof(float)*posture_size());
	}
	std::vector<int> toSmoothIds;
	int wide = 5;
	for (int i = strokeEndFrameId - wide; i <= strokeEndFrameId + wide; i++)
		toSmoothIds.push_back(i);


	// handle the base joints
	if (strokeEndFrameId == 0)
	{
		LOG("SkMotion::buildPoststrokeHoldMotion ERR: please check if the stroke end time is set correctly!");
		return newMotion;
	}

	if (insertIdleMotion)
	{
		float *ref_p = newMotion->posture(strokeEndFrameId - 1);
		for (int i = strokeEndFrameId; i <= strokeEndFrameId + numHoldFrames; ++i)
		{
			for (int k=0;k<mchan_arr.size();k++)
			{
				SkChannel& chan = mchan_arr[k];
				const std::string& jointName = mchan_arr.name(k);
				int index = mchan_arr.float_position(k);
				bool isPos = chan.type == SkChannel::XPos; 
				if (jointName == "base" && isPos)
				{
					newMotion->posture(i)[index] = ref_p[index];
					newMotion->posture(i)[index + 1] = ref_p[index + 1];
					newMotion->posture(i)[index + 2] = ref_p[index + 2];
					break;
				}
			}
		}
		newMotion->smoothAtFrame(strokeEndFrameId, 1, 5);
		newMotion->smoothAtFrame(strokeEndFrameId + numHoldFrames, 1, 5);
	}
	else if (joints.size() > 0)	// joint specified, add perlin noise
	{
		newMotion->addPerlinNoise(joints, strokeEndFrameId, strokeEndFrameId + numHoldFrames, scale, freq);
		//newMotion->smoothAtFrame(strokeEndFrameId, 3, 5);
		//newMotion->smoothAtFrame(strokeEndFrameId + numHoldFrames, 3, 5);
	}

	
	return newMotion;
}


/*
	- smooth section [frameId - interval, frameId + interval]
	- while smoothing each frame, use the mask
	- mask looks like	[0.25, 0.5, 0.25]				maskType = 0 (linear), maskSize = 3
						[0.11, 0.22, 0.33, 0.22, 0.11]	maskType = 0 (linear), maskSize = 5
						etc.
*/
void SkMotion::smoothAtFrame(int frameId, int interval, int maskSize, int maskType)
{
	if (maskSize == 0 || (maskSize % 2) == 0)
	{
		LOG("SkMotion::smoothAtFrame Warning: mask size has to be odd number.");
		return;
	}

	std::vector<int> frameIds;
	for (int i = frameId - interval; i <= frameId + interval; ++i)
		frameIds.push_back(i);

	std::vector<float> mask;
	mask.resize(maskSize);
	if (maskType == 0)	// linear
	{
		int halfSize = maskSize / 2;
		for (int i = 0; i < halfSize; ++i)
		{
			float v = float(i + 1) / float(halfSize * halfSize + halfSize * 2 + 1);
			mask[i] = v;
			mask[maskSize - i - 1] = v;
		}
		mask[halfSize] = 1.0f / float(halfSize + 1);
	}
	smoothByMask(frameIds, mask);
}

void SkMotion::addPerlinNoise(std::vector<std::string>& affectedJoints, int startFrame, int endFrame, float amp, float freq)
{
	if (startFrame == -1)	startFrame = 0;
	if (endFrame == -1)	endFrame = this->frames() - 1;

	SkChannelArray& mchan_arr = this->channels();
	
	int interval = endFrame - startFrame + 1;
	Perlin perlinNoise;
	if (freq < 0)
		freq = 1.0f / float(interval);

	std::vector<std::vector<float> > vecRandomN;
	vecRandomN.resize(affectedJoints.size() * 3);
	// generate random number
	for (size_t j = 0; j < affectedJoints.size(); ++j)
		for (int d = 0; d < 3; ++d)
		{
			std::vector<float> randomN;
			randomN.resize(interval);
			perlinNoise.init();
			for (int f = startFrame; f <= endFrame; ++f)
			{
				float x = (f - startFrame) * freq;
				float v = perlinNoise.noise1(x);
				randomN[f - startFrame] = v * amp;
			}
			vecRandomN[j * 3 + d] = randomN;
		}

	for (int i = startFrame; i <= endFrame; ++i)	
	{
		// set to motion
		float* ref_p = this->posture(i);
		for (size_t j = 0; j < affectedJoints.size(); ++j)
		{
			int chanId = mchan_arr.linear_search(affectedJoints[j], SkChannel::Quat);
			if (chanId < 0)
				continue;
			int index = mchan_arr.float_position(chanId);
			ref_p[index + 1] += vecRandomN[j * 3 + 0][i - startFrame];
			ref_p[index + 2] += vecRandomN[j * 3 + 1][i - startFrame];
			ref_p[index + 3] += vecRandomN[j * 3 + 2][i - startFrame];
		}
	}
}


void SkMotion::smoothByMask(std::vector<int>& frameIds, std::vector<float>& mask)
{
	if (mask.size() == 0 || (mask.size() % 2) == 0)
	{
		LOG("SkMotion::smoothByMask Warning: mask size has to be odd number or non-zero.");
		return;
	}

	int halfWide = mask.size() / 2;

	std::vector<float*> newFrameData;
	for (size_t i = 0; i < frameIds.size(); ++i)
	{
		float* new_p = (float*)malloc(sizeof(float)*posture_size());
		memset(new_p, 0, posture_size());
		newFrameData.push_back(new_p);
	}
	
	SkChannelArray& mchan_arr = this->channels();

	// smooth the frameIds
	for (size_t i = 0; i < frameIds.size(); ++i)
	{
		int frameId = frameIds[i];
		float *new_p = newFrameData[i];

		for (int j = frameId - halfWide; j <= frameId + halfWide; ++j)
		{
			float w = mask[j - frameId + halfWide];
			// not sure this part should be clamp or loop, put it into clamp right now
			if (j < 0) 
				j = 0;					// clamp
				//j += this->frames();	// loop
			if (j >= this->frames()) 
				j = this->frames() - 1;	// clamp
				//j -= this->frames();	// loop
			
			float* ref_p = this->posture(j);
			for (int k=0;k<mchan_arr.size();k++)
			{
				SkChannel& chan = mchan_arr[k];
				const std::string& jointName = mchan_arr.name(k);
				bool isPos = chan.type <= SkChannel::ZPos; 
				bool isBase = jointName == "base";
				int index = mchan_arr.float_position(k);		
				if (chan.type == SkChannel::Quat) // not sure if this is the best way of doing this
				{
					SrQuat curQ = SrQuat( new_p[ index ], new_p[ index + 1 ], new_p[ index + 2 ], new_p[ index + 3 ] );
					if (curQ.w == 0 && curQ.x == 0 && curQ.y == 0 && curQ.z == 0)
						curQ.w = 1.0f;
					SrQuat refQ = SrQuat( ref_p[ index ], ref_p[ index + 1 ], ref_p[ index + 2 ], ref_p[ index + 3 ] );
					SrQuat unitQ;
					SrQuat finalQ = slerp(unitQ, refQ, w) * curQ;
					if (isBase)
					{
						new_p[ index + 0 ] = (float)refQ.w;
						new_p[ index + 1 ] = (float)refQ.x;
						new_p[ index + 2 ] = (float)refQ.y;
						new_p[ index + 3 ] = (float)refQ.z;
					}
					else
					{
						new_p[ index + 0 ] = (float)finalQ.w;
						new_p[ index + 1 ] = (float)finalQ.x;
						new_p[ index + 2 ] = (float)finalQ.y;
						new_p[ index + 3 ] = (float)finalQ.z;
					}
				}
				else
				{
					for (int c = 0; c < chan.size(); c++)
					{
						if (isBase)
							new_p[index+c] = ref_p[index + c];
						else
							new_p[index+c] += w * ref_p[index + c];
					}	
				}
			}
		}
	}

	for (size_t i = 0; i < frameIds.size(); ++i)
		memcpy(this->posture(frameIds[i]), newFrameData[i], posture_size());

	// clear memory
	for (size_t i = 0; i < newFrameData.size(); ++i)
		delete [] newFrameData[i];
	newFrameData.clear();
}


SkMotion* SkMotion::buildMirrorMotion( SkSkeleton* skeleton )
{
	std::map<std::string,bool> jointNameMap;
	for (unsigned int i=0; i<skeleton->joints().size();i++)
	{
		SkJoint* joint = skeleton->joints()[i];
		jointNameMap[joint->getMappedJointName()] = true;
	}
	return buildMirrorMotionJoints(skeleton,jointNameMap);
}

SkMotion* SkMotion::buildMirrorMotionJoints(SkSkeleton* skeleton, const std::map<std::string,bool>& jointNameMap)
{	
	SkChannelArray& mchan_arr = this->channels();
	SmartBody::SBMotion* originalMotion = dynamic_cast<SmartBody::SBMotion*>(this);
	SmartBody::SBMotion *mirror_p = new SmartBody::SBMotion();
	mirror_p->setMotionSkeletonName(originalMotion->getMotionSkeletonName());	
	srSynchPoints sp(synch_points);
	mirror_p->synch_points = sp;
	mirror_p->init( mchan_arr );
	int num_f = this->frames();
	for (int i = 0; i < num_f; i++)
	{
		mirror_p->insert_frame( i, this->keytime( i ) );
		float *ref_p = this->posture( i );
		float *new_p = mirror_p->posture( i );
		// go through each channel, and flip the channel value when necessary
		for (int k=0;k<mchan_arr.size();k++)
		{
			SkChannel& chan = mchan_arr[k];
			const std::string& jointName = mchan_arr.mappedName(k);
			bool mirrorJoint = true;
			// not used joints
			if (jointNameMap.find(jointName) == jointNameMap.end())
				mirrorJoint = false;
			else
				mirrorJoint = jointNameMap.find(jointName)->second;

			int index = mchan_arr.float_position(k);
			if (chan.type == SkChannel::XPos)
			{
				if (mirrorJoint)
					new_p[index] = -ref_p[index]; // flip x-translation
				else
					new_p[index] = ref_p[index];
			}
			else if (chan.type == SkChannel::Quat)
			{
				// flip rotation for y,z axis
				SrQuat q_orig = SrQuat( ref_p[ index ], ref_p[ index + 1 ], ref_p[ index + 2 ], ref_p[ index + 3 ] );
				SrVec qa = q_orig.axisAngle();
				SkJoint* joint = skeleton->search_joint(jointName.c_str());
				if (!joint)
				{
					//LOG("Joint %s not found")
					// joint not found, simply write to channels
					for (int n=0;n<chan.size();n++)
						new_p[index+n] = ref_p[index+n];
				}
				else
				{
					SrQuat final_q = q_orig;	
					if (mirrorJoint)
					{
						SrQuat q_zero = SrQuat(joint->gmatZero());
						SrQuat q_lzero = SrQuat(joint->gmatZero());
						// map left to right joint
						if (boost::algorithm::starts_with(jointName,"l_") || boost::algorithm::starts_with(jointName,"r_"))
						{
							std::string jointNameComp = jointName;
							jointNameComp[0] = (jointNameComp[0]=='r') ? 'l' : 'r';  // get the mirror joint name	
							SkJoint* jointComp = skeleton->search_joint(jointNameComp.c_str());
							if (jointComp)
								q_lzero = SrQuat(jointComp->gmatZero());

						}

						SrVec qa_rot = qa*q_zero;					
						SrQuat qa_temp = SrQuat(qa_rot);
						euler_t ref_e = euler_t(quat_t(qa_temp.w,qa_temp.x,qa_temp.y,qa_temp.z));														
						quat_t new_q = euler_t( ref_e.x(), -ref_e.y(), -ref_e.z() );
						vector3_t new_qv = new_q.axisangle();
						//SrVec new_qa = SrVec(qa_rot.x,-qa_rot.y,-qa_rot.z)*q_lzero.inverse();//SrVec((float)new_qv.x(),(float)new_qv.y(),(float)new_qv.z())*q_zero.inverse();
						SrVec new_qa = SrVec((float)new_qv.x(),(float)new_qv.y(),(float)new_qv.z())*q_lzero.inverse();				
						final_q = SrQuat(new_qa);	
					}																
					
#if 0 // print debug info 
					if (joint->name() == "r_shoulder" || joint->name() == "l_shoulder")
					{
						euler_t ref_eorig = euler_t(quat_t(q_orig.w,q_orig.x,q_orig.y,q_orig.z));	
						euler_t final_euler = euler_t(quat_t(final_q.w,final_q.x,final_q.y,final_q.z));
						LOG("joint %s : ",joint->name().c_str());
						sr_out << "qa before rot = " << qa << srnl;
						sr_out << "qa after rot = " << qa_rot << srnl;
						LOG("euler before rot = %f %f %f",ref_eorig.x(),ref_eorig.y(),ref_eorig.z());
						LOG("euler after rot = %f %f %f",ref_e.x(),ref_e.y(),ref_e.z());
						LOG("euler final = %f %f %f",final_euler.x(),final_euler.y(),final_euler.z());
					}
#endif
					new_p[ index + 0 ] = (float)final_q.w;
					new_p[ index + 1 ] = (float)final_q.x;
					new_p[ index + 2 ] = (float)final_q.y;
					new_p[ index + 3 ] = (float)final_q.z;
				}
				
			}
			else
			{
				for (int n=0;n<chan.size();n++)
					new_p[index+n] = ref_p[index+n];
			}
		}
		
		for (int k=0;k<mchan_arr.size();k++)
		{
			SkChannel& chan = mchan_arr[k];
			const std::string& jointName = mchan_arr.mappedName(k);			
			if (jointNameMap.find(jointName) == jointNameMap.end()) // skip joint value swapping
				continue;
			int index = mchan_arr.float_position(k);
			if (boost::algorithm::starts_with(jointName,"l_"))
			{
				std::string jointNameRight = jointName;
				jointNameRight[0] = 'r';  // get the mirror joint name
				int rjointIndex = mchan_arr.search( jointNameRight.c_str(), chan.type );
				if (rjointIndex < 0)
					continue;
				int rindex = mchan_arr.float_position(rjointIndex);				
				// swap left and right channels
				for (int n=0;n<chan.size();n++)
				{
					std::swap(new_p[rindex+n],new_p[index+n]);
				}
			}
		}
		
	}
	return mirror_p;
}


