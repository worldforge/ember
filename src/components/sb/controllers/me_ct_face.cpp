/*
 *  me_ct_face.cpp - part of SmartBody-lib
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

//#include "sbm_pawn.hpp"
#include "sbm/gwiz_math.h"
#include "controllers/me_ct_face.h"
#include <vhcl_log.h>
#include <sstream>
#include <sbm/action_unit.hpp>
#include <sb/sbm_pawn.hpp>
#include <sb/sbm_character.hpp>
#include <sb/SBScene.h>
#include <sbm/sbm_deformable_mesh.h>
//////////////////////////////////////////////////////////////////////////////////

std::string MeCtFace::type_name = "Face";

MeCtFace::MeCtFace( void )	{

   _duration = -1.0;
   _base_pose_p = NULL;
   _skeleton_ref_p = NULL;
   _useVisemeClamping = true;

   _customized_motion = NULL;
   _motionStartTime = -1;
}

MeCtFace::~MeCtFace( void )	{
	
	clear();
}

void MeCtFace::clear( void )	{
	
   _duration = -1.0;
   _skeleton_ref_p = NULL;

	if( _base_pose_p )	{
		_base_pose_p = NULL;
	}
	_key_pose_map.reset();

	_visemeChannelMap.clear();
	_baseChannelToBufferIndex.clear();
}

void MeCtFace::customizeMotion(const std::string& motionName, double startTime)
{
	_motionStartTime = startTime;
	_customized_motion = (SkMotion*)SmartBody::SBScene::getScene()->getMotion(motionName);

	SkChannelArray& mChannels = _customized_motion->channels();
	int size = mChannels.size();
	_mChan_to_buff.size( size );
	if( _context ) 
	{
		SkChannelArray& cChannels = _context->channels();
		for (int i = 0; i < size; ++i) 
		{
			int chanIndex = cChannels.search(mChannels.mappedName(i), mChannels.type(i));
			_mChan_to_buff[i] = _context->toBufferIndex(chanIndex);
		}
	}
	else 
	{
		_mChan_to_buff.setall( -1 );
	}	
}

void MeCtFace::init (SbmPawn* pawn) {
	
	clear();
	MeController::init(NULL);

	SbmCharacter* sbmCharacter = dynamic_cast<SbmCharacter*>(pawn);
	_character = dynamic_cast<SmartBody::SBCharacter*>(pawn);
	SmartBody::SBFaceDefinition* faceDefinition = sbmCharacter->getFaceDefinition();
	if (!faceDefinition || !faceDefinition->getFaceNeutral())
		return;

	int numAUs = faceDefinition->getNumAUs();
	for (int a = 0; a < numAUs; a++)
	{
		int auNum = faceDefinition->getAUNum(a);
		std::stringstream id;
		id << auNum;
		ActionUnit* au = faceDefinition->getAU(auNum);
		if( au->is_left() )
		{
			add_key(au->getLeftName().c_str(), au->left);
		}
		if( au->is_right() )
		{
			add_key(au->getRightName().c_str(), au->right);
		}
		if( au->is_bilateral() )
		{
			add_key(au->getBilateralName().c_str(), au->left);
		}
	}

	// add the visemes
	int numVisemes = faceDefinition->getNumVisemes();
	for (int v = 0; v < numVisemes; v++)
	{
		std::string visemeName = faceDefinition->getVisemeName(v);
		SkMotion* motion = faceDefinition->getVisemeMotion(visemeName);
		if( motion )
			add_key( visemeName.c_str(), motion );
	}
	
	SkMotion* faceNeutral = faceDefinition->getFaceNeutral();
	if ( faceNeutral )	{
		_base_pose_p = faceNeutral;
		_base_pose_p->move_keytimes( 0.0 ); // make sure motion starts at 0
		_channels.setJointMapName(_base_pose_p->channels().getJointMapName());

		SkChannelArray& mchan_arr = _base_pose_p->channels();
		int size = mchan_arr.size();
		_include_chan_flag.size( size );
		for( int i = 0; i < size; i++ )	{
			_include_chan_flag[ i ] = 1;
			_channels.add( mchan_arr.name( i ), mchan_arr.type( i ) );
		}
		
		finish_adding();

#define DEFAULT_REMOVE_EYEBALLS 1
#if DEFAULT_REMOVE_EYEBALLS
		remove_joint( "eyeball_left" );
		remove_joint( "eyeball_right" );
#endif

	}
}

void MeCtFace::remove_joint( const char *joint_name ) {
	int found = 0;
	
	if( joint_name )	{
		if( _base_pose_p )	{
			SkChannelArray& mchan_arr = _base_pose_p->channels();
			int size = mchan_arr.size();
			for( int i = 0; i < size; i++ )	{
				if( mchan_arr.mappedName( i ) == joint_name ) {
//					LOG( "MeCtFace::remove_joint: exclude[ %d ]: '%s:%s'\n", 
//						i, mchan_arr.name( i ).get_string(), SkChannel::type_name( mchan_arr.type( i ) ) );
					_include_chan_flag[ i ] = 0;
					found = 1;
				}
			}
			if( !found )	{
				LOG( "MeCtFace::remove_joint Warning: joint '%s' NOT FOUND\n", joint_name );
			}
		}
		else	{
			LOG( "MeCtFace::remove_joint ERR: _base_pose_p is NULL\n" );
		}
	}
	else	{
		LOG( "MeCtFace::remove_joint ERR: joint_name is NULL\n" );
	}
}

void MeCtFace::remove_channel( const char *joint_name, SkChannel::Type ch_type ) {
	int found = 0;
	
	if( joint_name )	{
		if( _base_pose_p )	{
			SkChannelArray& mchan_arr = _base_pose_p->channels();
			int size = mchan_arr.size();
			for( int i = 0; i < size; i++ )	{
				if( ( mchan_arr.name( i ) == joint_name )&&( mchan_arr.type( i ) == ch_type ) ) {
//					LOG( "MeCtFace::remove_channel: exclude[ %d ]: '%s:%s'\n", 
//						i, mchan_arr.name( i ).get_string(), SkChannel::type_name( mchan_arr.type( i ) ) );
					_include_chan_flag[ i ] = 0;
					found = 1;
				}
			}
			if( !found )	{
				LOG( "MeCtFace::remove_channel ERR: channel '%s:%s' NOT FOUND\n", joint_name, SkChannel::type_name( ch_type ) );
			}
		}
		else	{
			LOG( "MeCtFace::remove_channel ERR: _base_pose_p is NULL\n" );
		}
	}
	else	{
		LOG( "MeCtFace::remove_channel ERR: joint_name is NULL\n" );
	}
}

void MeCtFace::add_key( const char *weight_key, SkMotion* key_pose_p ) {
	
	if (key_pose_p)
	{
		key_pose_p->move_keytimes( 0.0 );
	}
	_key_pose_map.insert( weight_key, key_pose_p );
	
	_channels.add( weight_key, SkChannel::XPos );
}

void MeCtFace::finish_adding( void )	{
	if( _context ) {
		_context->child_channels_updated( this );
	}

	_visemeChannelMap.clear();
	_key_pose_map.reset();

	if (!_base_pose_p)
		return;

	// get a map from the base channel index to the base buffer index
	SkChannelArray& baseChannelArray = _base_pose_p->channels();
	int baseSize = baseChannelArray.size();
	int curIndex = 0;
	for (int b = 0; b < baseSize; b++)
	{
		_baseChannelToBufferIndex.push_back(curIndex);
		int baseChannelSize = baseChannelArray[b].size();
		curIndex += baseChannelSize;
	}

	// now map the key channels to the base channels
	char* keyName;
	while (SkMotion* nextKey = _key_pose_map.next(&keyName))
	{
		std::vector<int> keyIndices;
		SkChannelArray& channelArray = nextKey->channels();
		int size = channelArray.size();
		for (int c = 0; c < size; c++)
		{
			SkChannel& channel = nextKey->channels()[c];
			std::string jointName = nextKey->channels().mappedName(c);
			int baseChannelIndex = _base_pose_p->channels().search(jointName, channel.type);
			if (baseChannelIndex >= 0)
			{
				keyIndices.push_back(baseChannelIndex); // mapping from the key pose channel index to the base pose channel index
			}
			else
			{
				keyIndices.push_back(-1); // key contains channel that does not exist in base pose
			}

		}
		
		_visemeChannelMap.insert(std::pair<std::string, std::vector<int> > (std::string(nextKey->getName()), keyIndices));
	}
}

///////////////////////////////////////////////////////////////////////////////////

void MeCtFace::context_updated( void ) {

#if 0
	if( _context ) {
		_skeleton_ref_p = _context->channels().skeleton(); // WHY HERE?
		if( _skeleton_ref_p == NULL )	{
			LOG( "MeCtFace::context_updated ERR: _skeleton_ref_p is NULL\n" );
		}
	}
	else	{
		LOG( "MeCtFace::context_updated ERR: context is NULL\n" );
	}
#endif
}

void MeCtFace::controller_map_updated( void ) {

	if( _base_pose_p ) {
		// Map motion channel index to context float buffer index
		SkChannelArray& base_channels = _base_pose_p->channels();
		
		const int num_base_chans = base_channels.size();
		int num_key_chans = _key_pose_map.get_num_entries();

		_bChan_to_buff.resize( num_base_chans );
		_kChan_to_buff.resize( num_key_chans );

		if( _context ) {

			SkChannelArray& cChannels = _context->channels();
			for( int i=0; i<num_base_chans; ++i ) {
				int chan_index = cChannels.search( base_channels.mappedName( i ), base_channels.type( i ) );
				// NOTE: check for -1 : report not found? NOT NECESSARILY
				_bChan_to_buff[ i ] = _context->toBufferIndex( chan_index );
			}
			
			// The following assumes that there will be no new additions to the hash-map
			int c = 0;
			char* key_pose_name;
			_key_pose_map.reset();
			while( _key_pose_map.next( & key_pose_name ) )	{
				int chan_index = cChannels.search( key_pose_name, SkChannel::XPos );
				// NOTE: check for -1 : report not found?
				_kChan_to_buff[ c++ ] = _context->toBufferIndex( chan_index );
			}
		} 
		else {
			for (size_t x = 0; x < _bChan_to_buff.size(); x++)
				_bChan_to_buff[x] = -1;
			for (size_t x = 0; x < _kChan_to_buff.size(); x++)
				_kChan_to_buff[x] = -1;
		}
	}
}

void MeCtFace::controller_start( void )	{

}

bool MeCtFace::updateMotion(double t, MeFrameData& frame)
{
	if (_customized_motion == NULL)
		return false;

	double localTime = t - _motionStartTime;
	if (localTime > _customized_motion->duration())
	{
		_customized_motion = NULL;
		return false;
	}

	int frameNum = int(localTime * _customized_motion->frames() / _customized_motion->duration());
	_customized_motion->apply(float(localTime),
		&(frame.buffer()[0]),  // pointer to buffer's float array
		&_mChan_to_buff,
		SkMotion::Linear, &frameNum, true, 0);

	return true;
}

bool MeCtFace::controller_evaluate( double t, MeFrameData& frame ) {
	bool continuing = true;
	continuing = t < _duration;
	if( t < 0.0 )	{
		return( continuing );
	}

	// check customized motion
	if (updateMotion(t, frame))
		return continuing;

	float *fbuffer = &( frame.buffer()[0] );
	SkChannelArray& base_channels = _base_pose_p->channels();
	if (_base_pose_p == NULL)
	{
		continuing = false;
		return continuing;
	}

#if 0
	if (_character->getBoolAttribute("blendshape"))
	{
		std::vector<std::string> visemeChannels;
#if 1
		visemeChannels.push_back("open");
		visemeChannels.push_back("W");
		visemeChannels.push_back("ShCh");
		visemeChannels.push_back("PBM");
		visemeChannels.push_back("FV");
		visemeChannels.push_back("wide");
		visemeChannels.push_back("tBack");
		visemeChannels.push_back("tRoof");
		visemeChannels.push_back("tTeeth");

		visemeChannels.push_back("au_1");
		visemeChannels.push_back("au_2");
		visemeChannels.push_back("au_4");
		visemeChannels.push_back("au_5");
		visemeChannels.push_back("au_6");
		visemeChannels.push_back("au_7");
		visemeChannels.push_back("au_9");
#endif

		DeformableMesh* mesh = _character->dMeshInstance_p->getDeformableMesh();
		if (mesh)
		{
			std::map<std::string, float>::iterator iter = mesh->visemeWeightMap.begin();
			for (; iter != mesh->visemeWeightMap.end(); ++iter)
			{
				iter->second = 0.0f;
				if (iter->first == "neutral")
					iter->second = 1.0f;
			}

			for (size_t i = 0; i < visemeChannels.size(); ++i)
			{
				std::string& visemeChannelName = visemeChannels[i];
				int chanid = frame.context()->channels().search(visemeChannels[i], SkChannel::XPos);
				if (chanid >= 0)
				{
					double v = frame.buffer()[frame.toBufferIndex(chanid)];
					if (v < 0)
						v = 0;
					if (v > 1)
						v = 1;

					mesh->visemeWeightMap[visemeChannels[i]] = float(v);
				}
				else
				{
					;//LOG("channel not found %s", visemeChannelName.c_str());
				}
			}
		}		
		return continuing;
	}
#endif

	int nchan = base_channels.size();
	float * base_pose_buff_p = NULL;
	if (_base_pose_p->frames() > 0)
		base_pose_buff_p = _base_pose_p->posture( 0 );

	int pose_var_index = 0;
	for( int i=0; i<nchan; i++ )	{
		
		int ch_size = base_channels[ i ].size();
		if( _include_chan_flag[ i ] )	{
			SkChannel::Type ch_type = base_channels[ i ].type;

			int base_ch_index = _bChan_to_buff[ i ];
			if( base_ch_index >= 0 )	{
				if( 
					( ch_type == SkChannel::XPos ) ||
					( ch_type == SkChannel::YPos ) ||
					( ch_type == SkChannel::ZPos )
				)	{
					fbuffer[ base_ch_index ] = base_pose_buff_p[ pose_var_index ];
				}
				else
				if( ch_type == SkChannel::Quat )	{
					fbuffer[ base_ch_index ] = base_pose_buff_p[ pose_var_index ];
					fbuffer[ base_ch_index + 1 ] = base_pose_buff_p[ pose_var_index + 1 ];
					fbuffer[ base_ch_index + 2 ] = base_pose_buff_p[ pose_var_index + 2 ];
					fbuffer[ base_ch_index + 3 ] = base_pose_buff_p[ pose_var_index + 3 ];
				}
			}
		}
		pose_var_index += ch_size;
	}


	int c = 0;
	SkMotion* key_pose_p;

	c = 0;
	_key_pose_map.reset();
	key_pose_p = _key_pose_map.next();
	while( key_pose_p )
	{
		int weight_index = _kChan_to_buff[ c++ ];
		if( weight_index >= 0 )	{
			float key_weight = fbuffer[ weight_index ];
			if( fabs( key_weight ) > 0.01 )	{
				float adjustedWeight = key_weight;
			
				//LOG( "Face: '%s': %f\n", key_pose_p->getName().c_str(), key_weight );
			
				if (key_pose_p->frames() == 0)
					continue;
				float* key_pose_buff_p = key_pose_p->posture( 0 );
				int numKeyChannels = key_pose_p->channels().size();
				SkChannelArray& keyPoseChannels = key_pose_p->channels();

				pose_var_index = 0;
				std::map<std::string, std::vector<int> >::iterator iter = _visemeChannelMap.find(std::string(key_pose_p->getName()));
				if (iter == _visemeChannelMap.end())
				{
					LOG("Viseme %s not found in channel-buffer map. Programmer needs to fix this! Might be lack of finish_adding function.", key_pose_p->getName().c_str());
					key_pose_p = _key_pose_map.next();
					continue;
				}
				std::vector<int>& keyToChannelMap = (*iter).second;

				for( int i=0; i<numKeyChannels; i++ )	{
					int ch_size = keyPoseChannels[ i ].size();
					int chIndex = keyToChannelMap[i];

					int baseIndex = keyToChannelMap[i];
//					if (i == -1) // this should only happen if there is a channel in the key pose that does not exist in the base pose
					if (baseIndex == -1) // this should only happen if there is a channel in the key pose that does not exist in the base pose
					{
						pose_var_index += ch_size;
						key_pose_p = _key_pose_map.next();
						continue;
					}
					int baseBufferIndex = _baseChannelToBufferIndex[baseIndex];

					if(_include_chan_flag[ chIndex ] )	{
						SkChannel::Type ch_type = keyPoseChannels[ i ].type;
						
						int base_ch_index = _bChan_to_buff[ chIndex ];
						if( base_ch_index >= 0 )	{
							if( 
								( ch_type == SkChannel::XPos ) ||
								( ch_type == SkChannel::YPos ) ||
								( ch_type == SkChannel::ZPos )
							)	{

								fbuffer[ base_ch_index ] += 
									( key_pose_buff_p[ pose_var_index ] - base_pose_buff_p[ baseBufferIndex ] ) * adjustedWeight;
							}
							else
							if( ch_type == SkChannel::Quat )	{

								gwiz::quat_t accum_q(
									fbuffer[ base_ch_index ],
									fbuffer[ base_ch_index + 1 ],
									fbuffer[ base_ch_index + 2 ],
									fbuffer[ base_ch_index + 3 ]
								);
								gwiz::quat_t base_q(
									base_pose_buff_p[ baseBufferIndex ], 
									base_pose_buff_p[ baseBufferIndex + 1 ], 
									base_pose_buff_p[ baseBufferIndex + 2 ], 
									base_pose_buff_p[ baseBufferIndex + 3 ] 
								);
								gwiz::quat_t key_q(
									key_pose_buff_p[ pose_var_index ], 
									key_pose_buff_p[ pose_var_index + 1 ], 
									key_pose_buff_p[ pose_var_index + 2 ], 
									key_pose_buff_p[ pose_var_index + 3 ] 
								);

								gwiz::quat_t result_q = ( ( key_q * -base_q ) * adjustedWeight ) * accum_q;

								fbuffer[ base_ch_index + 0 ] = (float)result_q.w();
								fbuffer[ base_ch_index + 1 ] = (float)result_q.x();
								fbuffer[ base_ch_index + 2 ] = (float)result_q.y();
								fbuffer[ base_ch_index + 3 ] = (float)result_q.z();
							}
						}
					}
					else	{
//						LOG( "exclude: %d\n", i );
					}
					pose_var_index += ch_size;
				}
			}
		}
		key_pose_p = _key_pose_map.next();
	}

	return continuing;
}

SkChannelArray& MeCtFace::controller_channels( void )	{
	return( _channels );
}

double MeCtFace::controller_duration( void ) {
	return( _duration );
}

const std::string& MeCtFace::controller_type( void ) const	{
	return( type_name );
}

void MeCtFace::print_state( int tabCount ) {

	LOG("MeCtFace" );

	const char* str = getName().c_str();
	if( str )
		LOG(" \"%s\"", str );

	LOG("\n" );
}

//////////////////////////////////////////////////////////////////////////////////

#if 0
void check_quat_morph( void )	{
	
	// test formula: 
	//	quat_t result_q = ( ( key_q * -base_q ) * key_weight ) * accum_q;

#if 0
	euler_t base( 30.0, 40.0, 10.0 );
	euler_t key( 35.0, 40.0, 10.0 );
#elif 0
	euler_t base( 30.0, 40.0, 10.0 );
	euler_t key( 30.0, 45.0, 10.0 );
#else
	euler_t base( 30.0, 40.0, 10.0 );
	euler_t key( 30.0, 40.0, 15.0 );
#endif

	euler_t dif = key * -base;
	dif.print();

	euler_t sum = dif * base;
	sum.print();

	euler_t sum1 = ( dif * 0.0 ) * base;
	sum1.print();

	euler_t sum2 = ( dif * 0.5 ) * base;
	sum2.print();

	euler_t sum3 = ( dif * 2.0 ) * base;
	sum3.print();
}
#endif
