/*
 *  me_controller.cpp - part of Motion Engine and SmartBody-lib
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

#include <iostream>
#include <sstream>

#include <controllers/me_controller.h>
#include <sr/sr_quat.h>
#include <sr/sr_euler.h>
#include <controllers/me_prune_policy.hpp>
#include <controllers/me_default_prune_policy.hpp>
#include <vhcl_log.h>
#include <controllers/me_controller_tree_root.hpp>
#include <sb/sbm_pawn.hpp>
#include <sb/SBMotion.h>
#include <sb/SBScene.h>

#include "sbm/lin_win.h"
using namespace std;

#define WARN_ON_INVALID_BUFFER_INDEX (1)
#define TRACE_REMAP_CHANNELS         (0)


//============================= MeController ============================

int MeController::instance_count = 0;


MeController::MeController () 
:	SBObject(),
	_active( false ),
	_indt( 0.0f ),
	_outdt( 0.0f ),
	_emphasist( -1.0f ),
	_lastEval( -1 ),  // effectively unset.. should really be a less likely value like NaN
	_prune_policy( new MeDefaultPrunePolicy() ),
	_context( NULL ),
 	_record_output( NULL ), // for recording poses and motions of immediate local results
	_startTime(-1),
	_stopTime(-1),
	_initialized(false),
	_enable(true)
	//_handle("")
{
	setName("");
	_instance_id = instance_count;
	instance_count ++;
	_invocation_count = -1;

	_prune_policy->ref();

	_frames = new std::list<FRAME>;
	_record_mode = RECORD_NULL;
	_record_max_frames = 0;
	_record_frame_count = 0;
	_record_full_prefix = "";
	_buffer_changes_toggle = false;
	_buffer_changes_toggle_reset = true;
	_record_duration = 0.0;

	SBObject::createBoolAttribute("enable", true, true, "Basic", 220, false, false, false, "whether to evaluate this controller");
	SBObject::createStringAttribute("handle", "", true, "Basic", 220, false, false, false, "handle for this controller");
}

MeController::~MeController () {
	//assert( _context==NULL );  // Controller should not be deleted if still referenced by context
	stop_record();

	if(_frames)	delete _frames;

	if( _prune_policy ) {
		_prune_policy->unref();
		_prune_policy = NULL;
	}

}

void MeController::clone_parameters( MeController* other )
{
	setName(other->getName());
	_indt = other->_indt;
	_outdt = other->_outdt;
	_emphasist = other->_emphasist;
}

void MeController::emphasist ( float t )
{ 
    if ( t<0 ) {
        t=-1.0f;
    }
    else
    {
        double d = controller_duration();
        if ( d>=0 && double(t)>d )
            t=float(d);
    }
    _emphasist=t;
}

void MeController::inoutdt ( float indt, float outdt )
 {
   float d = (float)controller_duration();

   if ( d>=0 && indt+outdt>d )
    { float factor = d/(indt+outdt);
      indt *= factor;
      outdt *= factor;
    }

   _indt = indt;
   _outdt = outdt;
 }

void MeController::remove_all_children() {
	size_t n = count_children();
	while( n != 0 ) {
		remove_child( child( --n ) );
	}
}

void MeController::init (SbmPawn* pawn) {
	_active = false;
	controller_init ();

	if( _context )
		_context->child_channels_updated( this );

	updateDefaultVariables(pawn);	
}

MePrunePolicy* MeController::prune_policy () {
	return _prune_policy;
}

void MeController::prune_policy( MePrunePolicy* prune_policy ) {
	if( _prune_policy != prune_policy ) {
		if( _prune_policy != NULL ) {
			_prune_policy->unref();
			_prune_policy = NULL;
		}
		_prune_policy = prune_policy;
		if( _prune_policy != NULL ) {
			_prune_policy->ref();
		}
	}
}


void MeController::start (double time) {

	// add the controller resource as 'start'
	/*ControllerResource* cres = new ControllerResource();
	cres->setControllerName(this->getName());
	cres->setType("start");
	cres->setTime(time);
	SBResourceManager::getResourceManager()->addControllerResource(cres);
	*/
	
	_active = true;
	controller_start ();
	_startTime = time;
}

double MeController::start_time ()
{
	return _startTime;
}

void MeController::stop (double time) {

	// add the controller resource as 'stop'
	/*ControllerResource* cres = new ControllerResource();
	cres->setControllerName(this->getName());
	cres->setType("stop");
	cres->setTime(time);
	SBResourceManager::getResourceManager()->addControllerResource(cres);
	*/
	
	_active = false;
	stop_record();
	controller_stop ();
	_stopTime = time;

// LOG( ">>> MeController::stop <<<\n" );
}

double MeController::stop_time ()
{
	return _stopTime;
}


void MeController::remap() {
#if TRACE_REMAP_CHANNELS
	std::cout << "========= Entering MeController::remap() for " << controller_type() << " \"" << name() << "\" ==" << std::endl;
#endif

	SkChannelArray& localChnls = controller_channels();
	int size = localChnls.size();
	_toContextCh.size( size );

	SkChannelArray& contextChnls = _context->channels();

	for( int i=0; i<size; ++i ) {
		const std::string& name = localChnls.mappedName( i );
		SkChannel::Type type = localChnls.type( i );
		_toContextCh[i] = contextChnls.search( name.c_str(), type );

		int parent_index = _toContextCh[i];
		if( parent_index >= 0 ) {
#if WARN_ON_INVALID_BUFFER_INDEX
			// WARN for invalid parent context buffer indices
			if( _context->toBufferIndex( parent_index ) < 0 ) {
				const char* parent_ch_type = SkChannel::type_name( _context->channels().type( parent_index ) );
				const char* parent_ch_name = _context->channels().mappedName( parent_index ).c_str();
				std::stringstream strstr;
				strstr << "WARNING: MeController::remap(): "<<controller_type()<<" \""<<this->getName()<<"\": "
					<<_context->context_type()<<" channel "<<parent_index<<", \""<<parent_ch_name<<"\" ("<<parent_ch_type<<") lacks valid buffer index!!";
				LOG(strstr.str().c_str());
			}
#endif

#if TRACE_REMAP_CHANNELS
			// Get a reference to the channel to inspect via debugger
			const char*     local_ch_name = name.get_string();
			SkChannel::Type parent_ch_type = _context->channels().type( parent_index );
			const char*     parent_ch_name = (const char*)(_context->channels().name( parent_index ));

			// Print it out...
			std::cout << "L#"<< i << "\t-> P#"<<parent_index<<"\t\t"<<local_ch_name<<" ("<<SkChannel::type_name(type)<<")"<< std::endl;

			if( strcmp( local_ch_name, parent_ch_name ) || (type != parent_ch_type ) ) {
				strstr << "ERROR: MeController::remap(): " << controller_type() << " \"" << _name << "\" :"
					<< " Local \"" << local_ch_name << "\" " << SkChannel::type_name(type) << " != "
					<< " Parent \"" << parent_ch_name << "\" " << SkChannel::type_name(parent_ch_type) << std::endl;
				LOG(strstr.str());
			}

#endif
		}
	}

	controller_map_updated();

#if TRACE_REMAP_CHANNELS
	std::cout << "========= Exiting MeController::remap() for " << controller_type() << " \"" << name() << "\" ==" << std::endl;
#endif
}

int MeController::getContextChannel(int index)
{
	return _toContextCh[index];
}


void MeController::dumpChannelMap()
{
	SkChannelArray& localChnls = controller_channels();
	int size = localChnls.size();
	_toContextCh.size( size );

	SkChannelArray& contextChnls = _context->channels();
	for( int i=0; i<size; ++i ) {
		const std::string& name = localChnls.name( i );
		SkChannel::Type type = localChnls.type( i );
		//_toContextCh[i] = contextChnls.search( name, type );

		int parent_index = _toContextCh[i];
		if( parent_index >= 0 ) {
#if WARN_ON_INVALID_BUFFER_INDEX
			// WARN for invalid parent context buffer indices
			if( _context->toBufferIndex( parent_index ) < 0 ) {
				const char* parent_ch_type = SkChannel::type_name( _context->channels().type( parent_index ) );
				const char* parent_ch_name = _context->channels().name( parent_index ).c_str();
				std::stringstream strstr;
				strstr << "WARNING: MeController::remap(): "<<controller_type()<<" \""<<this->getName()<<"\": "
					<<_context->context_type()<<" channel "<<parent_index<<", \""<<parent_ch_name<<"\" ("<<parent_ch_type<<") lacks valid buffer index!!";
				LOG(strstr.str().c_str());
			}
#endif

			// Get a reference to the channel to inspect via debugger
			const char*     local_ch_name = name.c_str();
			SkChannel::Type parent_ch_type = _context->channels().type( parent_index );
			const char*     parent_ch_name = _context->channels().name( parent_index ).c_str();

			// Print it out...
			LOG("L# %d \t-> P#%d\t\t%s (%s)", i, parent_index, local_ch_name, SkChannel::type_name(type));

			if( strcmp( local_ch_name, parent_ch_name ) || (type != parent_ch_type ) ) {
				std::stringstream strstr;
				strstr << "ERROR: MeController::remap(): " << controller_type() << " \"" << getName() << "\" :"
					<< " Local \"" << local_ch_name << "\" " << SkChannel::type_name(type) << " != "
					<< " Parent \"" << parent_ch_name << "\" " << SkChannel::type_name(parent_ch_type) << std::endl;
				LOG(strstr.str().c_str());
			}

		}
	}
}

void MeController::evaluate ( double time, MeFrameData& frame ) {

	MeEvaluationLogger* logger = _context->get_evaluation_logger();
	if( logger )
		logger->controller_pre_evaluate( time, *_context, *this, frame );

	// Reevaluate controller. Even for the same evaluation time as _lastEval, results may be influenced by differing buffer values
	if (isEnabled())
		_active = controller_evaluate ( time, frame );

	if (this->is_record_buffer_changes())
		this->cal_buffer_changes( frame );
#if 0 // disable recording in base controller. Should go through MtCtMotionRecorder interface
	if( _record_mode ) 
		cont_record( time, frame );
#endif

	if( logger )
		logger->controller_post_evaluate( time, *_context, *this, frame );
}

/*
void MeController::record_pose( const char *full_prefix ) { 
	_record_mode = RECORD_POSE; 
	_record_full_prefix = std::string( full_prefix ); 
}
*/

void MeController::record_motion( int max_num_of_frames ) { 

	stop_record();
	_record_mode = RECORD_MOTION; 
	_record_max_frames = max_num_of_frames;
	LOG("MeController::record_motion START");
}

void MeController::record_bvh( int max_num_of_frames, double dt )	{

	stop_record();
	_record_mode = RECORD_BVH_MOTION; 
	_record_max_frames = max_num_of_frames;
	_record_dt = dt;
	LOG("MeController::record_bvh START");
}

void MeController::load_bvh_joint_hmap( void )	{
	
/*
	if( _context ) {
		SkChannelArray& ctChannels = controller_channels(); // virtual ct reference
		int nchan = ctChannels.size();
		
		for( int i=0; i<nchan; ++i ) {
		
			SkJointName jname = ctChannels.name( i );
			const char* strname = jname.getstring();
			
//			bool b = _record_joint_hmap.insertstat( strname, 1 );
			bool b = _record_joint_hmap.insert( jname, 1 );
			if( b ) {
				LOG( "MeController::load_bvh_joint_hmap SUCCESS: '%s'\n", strname );
			}
			else	{
				LOG( "MeController::load_bvh_joint_hmap FAILURE: '%s'\n", strname );
			}
		}
	}
*/
}

void MeController::print_tabs( int depth )	{

	for( int i=0; i<depth; i++ ) { *_record_output << "\t"; }
}

bool MeController::print_bvh_hierarchy( SkJoint* joint_p, int depth )	{
	int i;
	
	if( joint_p == NULL )	{
		LOG("MeController::print_bvh_hierarchy ERR: NULL joint_p");
		return( false );
	}
	
	print_tabs( depth );
	if( depth == 0 )	{
		*_record_output << "ROOT " << joint_p->jointName().c_str() << "\n";
	}
	else	{
		*_record_output << "JOINT " << joint_p->jointName().c_str() << "\n";
	}

	print_tabs( depth );
	*_record_output << "{\n";
	
	print_tabs( depth + 1 );
	*_record_output << "OFFSET ";

	// STUPID-POLYTRANS ignores ROOT OFFSET: added to CHANNEL motion
	if( depth == 0 )	{
		*_record_output << "0.0 0.0 0.0 \n";
	}
	else	{
		SrVec offset_v = joint_p->offset();

	// STUPID-POLYTRANS subtracts OFFSET instead of adds
#define STUPID_POLYTRANS_FLIP_OFFSET 0
#if STUPID_POLYTRANS_FLIP_OFFSET
		*_record_output << -offset_v.x << " ";
		*_record_output << -offset_v.y << " ";
		*_record_output << -offset_v.z << " ";
#else
		*_record_output << offset_v.x << " ";
		*_record_output << offset_v.y << " ";
		*_record_output << offset_v.z << " ";
#endif
		*_record_output << "\n";
	}
	
	// CHANNELS: 
	// Optimize: check 
	//   SkJointQuat::_active, and 
	//   SkJointPos:SkVecLimits::frozen()
	print_tabs( depth + 1 );
	*_record_output << "CHANNELS 6 Xposition Yposition Zposition Zrotation Xrotation Yrotation\n";
	
	int num_child = joint_p->num_children();
	if( num_child == 0 )	{
	
		print_tabs( depth + 1 );
		*_record_output << "End Site\n";

		print_tabs( depth + 1 );
		*_record_output << "{\n";
		
		// End Site OFFSET not used
		// This is the geometric vector of the final bone segment
		print_tabs( depth + 2 );
		*_record_output << "OFFSET 0.0 0.0 0.0\n";
		
		print_tabs( depth + 1 );
		*_record_output << "}\n";
	}
	else
	for( i = 0; i < num_child; i++ )	{
		SkJoint* child_p = joint_p->child( i );
		print_bvh_hierarchy( child_p, depth + 1 );
	}

	print_tabs( depth );
	*_record_output << "}\n";
	
	return( true );
}

bool MeController::print_bvh_motion( SkJoint* joint_p, int depth, FRAME& frame_data )	{ 
	// NOTE: depth only used to hack STUPID-POLYTRANS ROOT bug
	int i;

	if( joint_p == NULL )	{
		LOG("MeController::print_bvh_motion ERR: NULL joint_p");
		return( false );
	}
	
	SkJointPos* sk_jp_p = joint_p->pos();
	
//*_record_output << " " << joint_p->name() << " { ";
	
	// STUPID-POLYTRANS ignores ROOT OFFSET: add to CHANNEL motion
	std::ostringstream * frame_data_os = new std::ostringstream;
	if( depth == 0 )	{
		SrVec offset_v = joint_p->offset();
		*frame_data_os << " " << sk_jp_p->value( 0 ) + offset_v.x;
		*frame_data_os << " " << sk_jp_p->value( 1 ) + offset_v.y;
		*frame_data_os << " " << sk_jp_p->value( 2 ) + offset_v.z;
		frame_data += frame_data_os->str();
	}
	else	{
		*frame_data_os << " " << sk_jp_p->value( 0 );
		*frame_data_os << " " << sk_jp_p->value( 1 );
		*frame_data_os << " " << sk_jp_p->value( 2 );
		frame_data += frame_data_os->str();
	}
	delete frame_data_os;
	
//	SkJointQuat* sk_jq_p = joint_p->quat();
//	SrQuat sr_q = sk_jq_p->value();

	SrMat sr_m = joint_p->lmat();
	float ex, ey, ez;
	sr_euler_angles_yxz( sr_m, ex, ey, ez ); // see sk_joint_euler.h for order interpretation...
//	sr_euler_angles_zxy( sr_m, ex, ey, ez );

//*_record_output << " ," << joint_p->name();

//inline double RAD( double d ) { return( d * 0.017453292519943295 ); }
//inline double DEG( double r ) { return( r * 57.295779513082323 ); }
	std::ostringstream * frame_data_os1 = new std::ostringstream;
	*frame_data_os1 << " " << ez * 57.295779513082323;
	*frame_data_os1 << " " << ex * 57.295779513082323;
	*frame_data_os1 << " " << ey * 57.295779513082323;
	frame_data += frame_data_os1->str();
	delete frame_data_os1;

//*_record_output << " }";


	int num_child = joint_p->num_children();
	for( i = 0; i < num_child; i++ )	{
		SkJoint* child_p = joint_p->child( i );
		print_bvh_motion( child_p, depth + 1, frame_data );
	}

	return( true );
}

void MeController::record_stop()	{
	stop_record();
	LOG("MeController::record_stop");
}

void MeController::record_clear()	{
	_frames->clear();
}

void MeController::record_write( const char *full_prefix ) {
	
	_record_full_prefix = std::string(full_prefix);	
	ostringstream record_id_oss;
	_invocation_count++;
	if( getName() != "" )	{
		setName( "noname" );
	}
	record_id_oss << _instance_id << "." << _invocation_count << "_R";
	string recordname = string( controller_type() ) + "_" + getName() + "_" + record_id_oss.str();	
	saveMotionRecord(recordname);
	_record_full_prefix = "";
}

void MeController::saveMotionRecord( const std::string &recordname )
{
	std::string filename;
	SrOutput* fileOutput = NULL;
	SrString stringOutput;
	if( _record_mode == RECORD_BVH_MOTION )	{
		filename = _record_full_prefix + recordname + ".bvh";
		fileOutput = new SrOutput( filename.c_str(), "w" );
		_record_output = new SrOutput(stringOutput);

		SkSkeleton* skeleton_p = NULL;
		if( _context->channels().size() > 0 )	{
			skeleton_p = _context->channels().skeleton();
		}
		if( skeleton_p == NULL )	{
			LOG("MeController::record_write NOTICE: SkSkeleton not available");
			_record_mode = RECORD_NULL;
		}

		*_record_output << "HIERARCHY\n";
		print_bvh_hierarchy( skeleton_p->root(), 0 );
		*_record_output << "MOTION\n";
		*_record_output 
			<< (const char *)"Frames: " 
			<< (int)_frames->size() 
			<< srnl;	

		*_record_output << "Frame Time: " << _record_dt << srnl;	
		//		load_bvh_joint_hmap();
		LOG("MeController::write_record BVH: %s", filename.c_str() );
	}
	else if( _record_mode == RECORD_MOTION )	{
			filename = _record_full_prefix + recordname + ".skm";
			fileOutput = new SrOutput( filename.c_str(), "w" );		
			_record_output = new SrOutput(stringOutput);

			*_record_output << "# SKM Motion Definition - M. Kallmann 2004\n";
			*_record_output << "# Maya exporter v0.6\n";
			*_record_output << "# Recorded output from MeController\n\n";
			*_record_output << "SkMotion\n\n";
			*_record_output << "name \"" << recordname.c_str() << "\"\n\n";

			SkChannelArray& channels = controller_channels();
			*_record_output << channels << srnl;
			*_record_output << "frames " << (int)_frames->size() << srnl;	

			LOG("MeController::write_record SKM: %s", filename.c_str() );
	}
	else	
	{
		LOG("MeController::init_record NOTICE: POSE not implemented");
			_record_mode = RECORD_NULL;
			_frames->clear();
			//filename = _record_full_prefix + recordname + ".skp";
	}

	std::list<FRAME>::iterator iter = _frames->begin();
	std::list<FRAME>::iterator end  = _frames->end();
	LOG("Output Motion, num frames = %d",_frames->size());
	int frameCount = 0;
	for(;iter!=end; iter++)
	{
		*_record_output<<(*iter).c_str()<<srnl;
		//LOG("finish writing frame %d",frameCount++);
	}
	if (_record_mode == RECORD_MOTION) // write out time sync point to avoid erroneous results
	{
		*_record_output << "ready time: " << _record_duration*0.01 << srnl;
		*_record_output << "strokeStart time: " << _record_duration*0.02 << srnl;
		*_record_output << "emphasis time: " << _record_duration*0.5 << srnl;
		*_record_output << "stroke time: " << _record_duration*0.99 << srnl;
		*_record_output << "relax time: " << _record_duration*1.0 << srnl;	
	}
	
	// load the motion
	SrInput recordInput = SrInput((const char*)(stringOutput));
	SmartBody::SBMotion* sbMotion = SmartBody::SBScene::getScene()->createMotion(recordname);
	if (sbMotion == NULL)
	{
		LOG("Recorded motion %s is already existing!", recordname.c_str());
		return;
	}

	sbMotion->setName(recordname);
	sbMotion->load(recordInput);
	sbMotion->filename(filename.c_str());
		
	// save motion to file as well
	*fileOutput << stringOutput;
	fileOutput->close();

	//record_clear();
	if( fileOutput )	{
		delete fileOutput;
		fileOutput = NULL;
	}
	
}

void MeController::cont_record( double time, MeFrameData& frame )	{
	
	if( time < 0.0001 ) time = 0.0;

	if( _record_mode == RECORD_BVH_MOTION )	{

		SkSkeleton* skeleton_p = NULL;
		if( _context->channels().size() > 0 )	{
			skeleton_p = _context->channels().skeleton();
		}
		if( skeleton_p == NULL )	{
			LOG("MeController::cont_record NOTICE: SkSkeleton not available");
			_record_mode = RECORD_NULL;
			return;
		}
		FRAME frame_data;
		frame_data.clear();
		// NOTE: depth only used to hack STUPID-POLYTRANS ROOT bug
		print_bvh_motion( skeleton_p->root(), 0, frame_data );		
		if((_record_frame_count>=_record_max_frames)&&(_record_max_frames>0))
			_frames->pop_front();
		_frames->push_back(frame_data);
	}
	else
	if( _record_mode == RECORD_MOTION )	{
		FRAME frame_data;
		frame_data.clear();
		ostringstream frame_data_os;
		frame_data_os << "kt " << time << " fr ";

		SkChannelArray& channels = controller_channels();
		int num_channels = channels.size();
		SrBuffer<float>& buff = frame.buffer();

		int i, j;
		for( i=0; i<num_channels; i++ )	{

			int index = frame.toBufferIndex( _toContextCh[ i ] );
			int channel_size = channels[ i ].size();

			// SKM format does not actually store a quat, it stores an 'axisangle'
			if( channels[ i ].type == SkChannel::Quat )	{
				SrQuat q ( buff[ index + 0 ], buff[ index + 1 ], buff[ index + 2 ], buff[ index + 3 ] );
				SrVec axis = q.axis();
				float ang = q.angle();
				axis.len ( ang );		
				frame_data_os << axis.x << " ";
				frame_data_os << axis.y << " ";
				frame_data_os << axis.z << " ";
			}
			else
			for( j=0; j<channel_size; j++ )	{
				frame_data_os << buff[ index + j ] << " ";
			}
		}
		frame_data += frame_data_os.str();
		if((_record_frame_count>=_record_max_frames)&&(_record_max_frames>0))
			_frames->pop_front();
		_frames->push_back(frame_data);
	}
	_record_duration = time;
	_record_frame_count++;
}

void MeController::stop_record( void )	{
	_record_mode = RECORD_NULL;
	_record_max_frames = 0;
	_record_frame_count = 0;
	if( _record_output )	{
		delete _record_output;
		_record_output = NULL;
	}
	_frames->clear();
//	cout << "MeController::stop_record" << endl;
}

void MeController::record_buffer_changes(bool val)
{
	_buffer_changes_toggle = val;
	_buffer_changes_toggle_reset = true;
}

void MeController::cal_buffer_changes( MeFrameData& frame)
{

	if (_buffer_changes_toggle_reset)
	{
		SkChannelArray& channelsInUse = this->controller_channels();
		int bufferSize = channelsInUse.floats();
		if (_bufferRecord.size() != bufferSize)
		{
			_bufferRecord.resize(bufferSize);
		}
		for (int x = 0; x < bufferSize; x++)
		{
			_bufferRecord[x] = 0.0;
		}
	}

	SkChannelArray& channelsInUse = this->controller_channels();
	int bufferSize = channelsInUse.floats();
	if (_bufferRecord.size() != bufferSize)
	{
		_bufferRecord.resize(bufferSize);
	}
	int curIndex = 0;
	int numChannels = channelsInUse.size();
	for( int i = 0 ; i < numChannels; i++ )
	{
		SkChannel& channel = channelsInUse.get(i);
		int channelIndex = _toContextCh[ i ];
		int bufferIndex = frame.toBufferIndex(channelIndex);
		if (bufferIndex < 0)
			continue;

		int channelSize = channel.size();
		for (int j = 0; j < channelSize; j++)
		{
			float val = frame.buffer()[bufferIndex + j];
			_bufferRecord[curIndex + j] = val;
		}
		curIndex += channelSize;
    }

}


std::vector<float>& MeController::get_buffer_changes()
{
	return _bufferRecord;
}



void MeController::output ( SrOutput& o )
 {
   o << getName().c_str() << " inout " << _indt << srspc << _outdt << srnl;
   if ( _emphasist>=0 )
     o << "emphasist " << _emphasist << srnl;
 }

void MeController::input ( SrInput& i )
 {
   i.get_token();
   setName((const char*) i.last_token());
   i.get_token(); // inout
   i >> _indt;
   i >> _outdt;
   i.get_token();
   if ( i.last_token()=="emphasist" )
    { i >> _emphasist; }
   else
    { _emphasist=-1.0; i.unget_token(); }
 }

void MeController::print_state( int tab_count ) {
	using namespace std;

	const char* name = this->getName().c_str();

	cout << controller_type();
	if( name!=NULL && name[0]!='\0' )
		cout << " \"" << name << "\"";
	cout << " @0x" << this << endl;

	print_children( tab_count );
}

void MeController::print_children( int tab_count ) {
	using namespace std;

	int count = count_children();
	if( count>0 ) {
		std::string indent( ++tab_count, '\t' );
		for( int i=0; i<count; ++i ) {
			MeController* ct = child( i );
			if( ct ) {
				cout << endl << indent;
				ct->print_state( tab_count );
			} else {
				cout << endl << indent << "Child " << i << " is NULL" << endl;
			}
		}
	}
}

const std::string& MeController::handle() const
{
	return _handle;
}

void MeController::handle( std::string handle )
{
	_handle = handle;
}

bool MeController::isEnabled() const
{
	return _enable;
}

void MeController::setEnable( bool val )
{
	_enable = val;

}

void MeController::updateDefaultVariables(SbmPawn* pawn)
{
	// set the default parameters
	//if (_initialized)
	//	return;	
	if (pawn)
	{		
		for (unsigned int i=0;i<_defaultAttributes.size();i++)
		{
			SmartBody::SBAttribute* dattr = _defaultAttributes[i].first;
			SmartBody::SBAttribute* pawnDefaultAttr = pawn->getAttribute(dattr->getName());
			VariablePointer& varPtr = _defaultAttributes[i].second;
			varPtr.updateVariableFromAttribute(pawnDefaultAttr);
		}
		_initialized = true;
	}
}

void MeController::notify(SmartBody::SBSubject* subject)
{
	SmartBody::BoolAttribute* boolAttribute = dynamic_cast<SmartBody::BoolAttribute*>(subject);
	if (boolAttribute)
	{
		if (boolAttribute->getName() == "enable")
		{
			setEnable(boolAttribute->getValue());
		}
	}

	SmartBody::StringAttribute* stringAttribute = dynamic_cast<SmartBody::StringAttribute*>(subject);
	if (stringAttribute)
	{
		if (stringAttribute->getName() == "handle")
		{
			handle(stringAttribute->getValue());
		}
	}

}


//============================ End of File ============================
