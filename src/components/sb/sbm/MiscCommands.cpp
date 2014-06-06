#include "MiscCommands.h"


#include <sbm/mcontrol_callbacks.h>
#include <sbm/local_speech.h>
#include <sbm/text_speech.h>
#include <sbm/sbm_speech_audiofile.hpp>
#include <sb/SBPhysicsManager.h>
#ifdef EMBER_SB_BONEBUS
	#include <sb/SBBoneBusManager.h>
#endif
#include <sb/SBAssetManager.h>
#include <sb/sbm_pawn.hpp>
#include <sb/sbm_character.hpp>
#include <sb/SBSceneListener.h>
#include <sb/SBSpeechManager.h>
#include <sb/SBScene.h>
#include <sb/SBMotion.h>
#include <sb/SBSimulationManager.h>
#include <sb/SBCommandManager.h>
#include <sb/SBBmlProcessor.h>
#include <sb/SBReach.h>
#include <sb/SBTypes.h>
#include <bml/bml_processor.hpp>
#include <controllers/me_ct_scheduler2.h>
#include <controllers/me_ct_blend.hpp>
#include <controllers/me_ct_gaze.h>
#include <controllers/me_ct_eyelid.h>
#include <controllers/me_controller_tree_root.hpp>
#include <controllers/me_ct_channel_writer.hpp>
#include <controllers/MeCtReachEngine.h>
#include <controllers/me_ct_example_body_reach.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/version.hpp>

// android does not use GPU shader for now
#if !defined(__ANDROID__) && !defined(__FLASHPLAYER__) && !defined(SB_IPHONE)
#ifdef EMBER_SB_GPU
#include <sbm/GPU/SbmDeformableMeshGPU.h>
#endif
#endif


int set_attribute( SbmPawn* pawn, std::string& attribute, srArgBuffer& args)
{
	

	if( attribute=="world_offset" || attribute=="world-offset" ) {
		//  Command: set pawn <character id> world_offset ...
		//  Sets the parameters of the world_offset joint
		return set_world_offset_cmd( pawn, args );
	} 
	else if (attribute == "mass")
	{
		if (args.calc_num_tokens() == 0)
		{
			SkSkeleton* skeleton = pawn->_skeleton;
			std::vector<SkJoint*>& joints = skeleton->get_joint_array();
			for (size_t j = 0; j < joints.size(); j++)
			{
				LOG("%s : %f", joints[j]->jointName().c_str(), joints[j]->mass());

			}
			return CMD_SUCCESS;
		}
		std::string jointName = args.read_token();
		if (jointName.length() == 0)
		{
			LOG("ERROR: SbmCharacter::set_cmd_func(..): Need joint name. Use: set char mass <joint> <amount>");
			return CMD_FAILURE;
		}
		const SkJoint* joint = pawn->get_joint(jointName.c_str());
		if (!joint)
		{
			LOG("ERROR: SbmCharacter::set_cmd_func(..): No joint found with name '%s'.", jointName.c_str());
			return CMD_FAILURE;
		}
		float mass = args.read_float();
		if (mass < 0)
		{
			LOG("ERROR: SbmCharacter::set_cmd_func(..): Mass must be > 0.");
			return CMD_FAILURE;
		}
		// is there a function that returns an SkJoint* and not a const SkJoint*?
		// That would make this next line of code unnecessary.
		SkJoint* editableJoint = const_cast<SkJoint*>(joint);
		editableJoint->mass(mass);
		//LOG("Set joint '%s' on character '%s' to mass '%f'.", jointName.c_str(), pawn->name, mass);
		return CMD_SUCCESS;
	} 	
	else 
	{
		LOG("ERROR: SbmPawn::set_cmd_func(..): Unknown attribute \"%s\".", attribute.c_str() );
		return CMD_FAILURE;
	}
}


#ifdef EMBER_SB_WSP
#if USE_WSP
WSP::WSP_ERROR remote_pawn_position_update( std::string id, std::string attribute_name, wsp_vector & vector_3d, void * data, const std::string & data_provider )
{
	SmartBody::SBPawn* pawn = SmartBody::SBScene::getScene()->getPawn(id);
	if ( pawn != NULL )
	{
		float x, y, z, h, p, r;
		pawn->get_world_offset( x, y, z, h, p, r );

		pawn->set_world_offset( (float)vector_3d.x, (float)vector_3d.y, (float)vector_3d.z, h, p, r );
	}
	else
	{
		std::stringstream strstr;
		strstr << "ERROR: SbmPawn::remote_pawn_position_update: SbmPawn '" << id << "' is NULL, cannot set_world_offset";
		LOG(strstr.str().c_str());
		return WSP::not_found_error( "SbmPawn is NULL" );
	}

	return WSP::no_error();
}

WSP::WSP_ERROR remote_pawn_rotation_update( std::string id, std::string attribute_name, wsp_vector & vector_4d, void * data, const std::string & data_provider )
{
	SbmPawn * pawn_p =  SmartBody::SBScene::getScene()->getPawn( id );

	if ( pawn_p != NULL )
	{
		float x, y, z, h, p, r;
		pawn_p->get_world_offset( x, y, z, h, p, r );

		gwiz::euler_t e = gwiz::quat_t( vector_4d.q, vector_4d.x, vector_4d.y, vector_4d.z );
		pawn_p->set_world_offset( x, y, z, (float)e.h(), (float)e.p(), (float)e.r() );
	}
	else
	{
		std::stringstream strstr;
		strstr << "ERROR: SbmPawn::remote_pawn_rotation_update: SbmPawn '" << id << "' is NULL, cannotsbm set_world_offset";
		LOG(strstr.str().c_str());
		return  WSP::not_found_error( "SbmPawn is NULL" );
	}

	return  WSP::no_error();
}

void handle_wsp_error( std::string id, std::string attribute_name, int error, std::string reason, void* data )
{

	LOG( "error getting id: %s attribute_name: %s. error_code: %d reason: %s\n", id.c_str(), attribute_name.c_str(), error, reason.c_str() );
}


#endif
#endif

int pawn_set_cmd_funcx( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr)
{
	std::string pawn_id = args.read_token();
	if( pawn_id.length()==0 ) {
		LOG("ERROR: SbmPawn::set_cmd_func(..): Missing pawn id.");
		return CMD_FAILURE;
	}

	

	SbmPawn* pawn =  SmartBody::SBScene::getScene()->getPawn( pawn_id );
	if( pawn==NULL ) {
		LOG("ERROR: SbmPawn::set_cmd_func(..): Unknown pawn id \"%s\".", pawn_id.c_str());
		return CMD_FAILURE;
	}

	std::string attribute = args.read_token();
	if( attribute.length()==0 ) {
		LOG("ERROR: SbmPawn::set_cmd_func(..): Missing attribute \"%s\" to set.", attribute.c_str());
		return CMD_FAILURE;
	}

	return  set_attribute( pawn, attribute, args);
}

int set_voice_cmd_func( SbmCharacter* character, srArgBuffer& args)
{
	
	//  Command: set character voice <speech_impl> <character id> voice <implementation-id> <voice code>
	//  Where <implementation-id> is "remote" or "audiofile"
	//  Sets character's voice code
	const char* impl_id = args.read_token();

	if( strlen( impl_id )==0 ) {
		character->set_speech_impl( NULL );
		std::string s( "" );
		character->set_voice_code( s );

		// Give feedback if unsetting
		LOG("Unset %s's voice.", character->getName().c_str());
	} else if( _stricmp( impl_id, "remote" )==0 ) {
		const char* voice_id = args.read_token();
		if( strlen( voice_id )==0 ) {
			LOG("ERROR: Expected remote voice id.");
			return CMD_FAILURE;
		}
		character->set_speech_impl(SmartBody::SBScene::getScene()->getSpeechManager()->speech_rvoice() );
		std::string s( voice_id );
		character->set_voice_code( s );
	} else if( _stricmp( impl_id, "local" )==0 ) {
		const char* voice_id = args.read_token();
		if( strlen( voice_id )==0 ) {
			LOG("ERROR: Expected local voice id.");
			return CMD_FAILURE;
		}
		LOG("set local voice");
		character->set_speech_impl( SmartBody::SBScene::getScene()->getSpeechManager()->speech_localvoice() );
		FestivalSpeechRelayLocal* relay = SmartBody::SBScene::getScene()->getSpeechManager()->festivalRelay();
		relay->setVoice(voice_id);
		std::string s( voice_id );
		character->set_voice_code( s );
	} else if( _stricmp( impl_id, "audiofile" )==0 ) {
		const char* voice_path = args.read_token();
		if( strlen( voice_path )==0 ) {
			LOG("ERROR: Expected audiofile voice path.");
			return CMD_FAILURE;
		}
		character->set_speech_impl( SmartBody::SBScene::getScene()->getSpeechManager()->speech_audiofile() );
		std::string voice_path_str= "";
		voice_path_str+=voice_path;
		character->set_voice_code( voice_path_str );
	} else if( _stricmp( impl_id, "text" )==0 ) {
		const char* voice_path = args.read_token();
		if( strlen( voice_path )==0 ) {
			LOG("ERROR: Expected id.");
			return CMD_FAILURE;
		}
		character->set_speech_impl(SmartBody::SBScene::getScene()->getSpeechManager()->speech_text() );
		std::string voice_path_str= "";
		voice_path_str+=voice_path;
		character->set_voice_code( voice_path_str );
	} else {
		LOG("ERROR: Unknown speech implementation \"%s\".", impl_id);
		return CMD_FAILURE;
	}
	return CMD_SUCCESS;
}

int set_voicebackup_cmd_func( SbmCharacter* character, srArgBuffer& args)
{
	
	//  Command: set character voice <speech_impl> <character id> voice <implementation-id> <voice code>
	//  Where <implementation-id> is "remote" or "audiofile"
	//  Sets character's voice code
	const char* impl_id = args.read_token();

	if( strlen( impl_id )==0 ) {
		character->set_speech_impl_backup( NULL );
		std::string s("");
		character->set_voice_code_backup( s );

		// Give feedback if unsetting
		LOG("Unset %s's voice.", character->getName().c_str());
	} else if( _stricmp( impl_id, "remote" )==0 ) {
		const char* voice_id = args.read_token();
		if( strlen( voice_id )==0 ) {
			LOG("ERROR: Expected remote voice id.");
			return CMD_FAILURE;
		}
		character->set_speech_impl_backup(SmartBody::SBScene::getScene()->getSpeechManager()->speech_rvoice() );
		std::string s( voice_id );
		character->set_voice_code_backup( s );
	} else if( _stricmp( impl_id, "local" )==0 ) {
		const char* voice_id = args.read_token();
		if( strlen( voice_id )==0 ) {
			LOG("ERROR: Expected local voice id.");
			return CMD_FAILURE;
		}
		LOG("set local voice");
		character->set_speech_impl_backup(SmartBody::SBScene::getScene()->getSpeechManager()->speech_localvoice() );
		FestivalSpeechRelayLocal* relay =SmartBody::SBScene::getScene()->getSpeechManager()->festivalRelay();
		relay->setVoice(voice_id);
		std::string s( voice_id );
		character->set_voice_code_backup( s );
	} else if( _stricmp( impl_id, "audiofile" )==0 ) {
		const char* voice_path = args.read_token();
		if( strlen( voice_path )==0 ) {
			LOG("ERROR: Expected audiofile voice path.");
			return CMD_FAILURE;
		}
		character->set_speech_impl_backup(SmartBody::SBScene::getScene()->getSpeechManager()->speech_audiofile() );
		std::string voice_path_str= "";
		voice_path_str+=voice_path;
		character->set_voice_code_backup( voice_path_str );
	} else if( _stricmp( impl_id, "text" )==0 ) {
		const char* voice_path = args.read_token();
		if( strlen( voice_path )==0 ) {
			LOG("ERROR: Expected id.");
			return CMD_FAILURE;
		}
		character->set_speech_impl_backup(SmartBody::SBScene::getScene()->getSpeechManager()->speech_text() );
		std::string voice_path_str= "";
		voice_path_str+=voice_path;
		character->set_voice_code_backup( voice_path_str );
	} else {
		LOG("ERROR: Unknown speech implementation \"%s\".", impl_id);
		return CMD_FAILURE;
	}
	return CMD_SUCCESS;
}






int pawn_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr)
{
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	

	std::string pawn_name = args.read_token();
	if( pawn_name.length()==0 )
	{
		LOG("ERROR: Expected pawn name.");
		return CMD_FAILURE;
	}

	std::string pawn_cmd = args.read_token();
	if( pawn_cmd.length()==0 )
	{
		LOG("ERROR: Expected pawn command.");
		return CMD_FAILURE;
	}

	if (pawn_cmd == "init")
	{
		// pawn <name> init [loc <x> <y> <z>] [geom <shape name>] [color <color hex>] [size <size>]
		SbmPawn* pawn_p =  SmartBody::SBScene::getScene()->getPawn(pawn_name);
		if( pawn_p != NULL ) {
			LOG("ERROR: Pawn \"%s\" already exists.", pawn_name.c_str());
			return CMD_FAILURE;
		}
		// Options
		float loc[3] = { 0, 0, 0 };

		bool has_geom = false;
		std::string geom_str = "box";
		std::string file_str = "";
		std::string size_str = "";
		std::string color_str = "red";
		std::string type_str = "";
		SrVec size = SrVec(1.f,1.f,1.f);
		bool setRec = false;
		SrVec rec;
		std::string defaultColor = "red";
		while( args.calc_num_tokens() > 0 ) {
			std::string option = args.read_token();
			// TODO: Make the following option case insensitive
			if( option == "loc" ) {
				args.read_float_vect( loc, 3 );
			} else if( option=="geom" ) {
				geom_str = args.read_token();
				has_geom = true;
			} else if (option == "file")
			{
				file_str = args.read_token();
				has_geom = true;	
			} else if( option=="type" ) {
				type_str = args.read_token();
				has_geom = true;
			} else if( option=="size" ) {
				size_str = args.read_token();
				has_geom = true;
			} else if( option=="color" ) {
				color_str = args.read_token();
				has_geom = true;
			} else if( option=="rec" ) {
				setRec = true;
				size.x = rec.x = args.read_float();
				size.y = rec.y = args.read_float();
				size.z = rec.z = args.read_float();
				has_geom = true;
			} else {
				std::stringstream strstr;
				strstr << "WARNING: Unrecognized pawn init option \"" << option << "\"." << std::endl;
				LOG(strstr.str().c_str());
			}
		}		

		pawn_p = scene->createPawn(pawn_name.c_str() );
		pawn_p->setClassType("pawn");
		SkSkeleton* skeleton = new SmartBody::SBSkeleton();
		skeleton->ref();
		std::string skel_name = pawn_name+"-skel";
		skeleton->setName( skel_name);
		// Init channels
		skeleton->make_active_channels();

		int err = pawn_p->init( skeleton );

		if( err != CMD_SUCCESS ) {
			std::stringstream strstr;		
			strstr << "ERROR: Unable to initialize SbmPawn \"" << pawn_name << "\".";
			LOG(strstr.str().c_str());
			delete pawn_p;
			skeleton->unref();
			return err;
		}

		// setting up geometry and physics 
		if( has_geom && !geom_str.empty() ) {
			//LOG("WARNING: SbmPawn geometry not implemented.  Ignoring options.");			
			if (!size_str.empty())
			{
				float uniformSize = (float)atof(size_str.c_str());
				for (int i=0;i<3;i++)
					size[i] = uniformSize;
			}			
			//pawn_p->initGeomObj(geom_str.c_str(),size,color_str.c_str(),file_str.c_str());
			SmartBody::SBPhysicsManager* phyManager = SmartBody::SBScene::getScene()->getPhysicsManager();
			phyManager->createPhysicsPawn(pawn_p->getName(),geom_str,size);
		}
	#ifdef EMBER_SB_STEER
		if (pawn_p->getGeomObject())
		{
			if (geom_str == "box")
			{
				pawn_p->steeringSpaceObjSize = rec;
				if (!setRec)
				{
					float size = (float)atof(size_str.c_str());
					pawn_p->steeringSpaceObjSize = SrVec(size, size, size);
				}
				if (type_str == "steering")
					pawn_p->initSteeringSpaceObject();
			}
		}
	#endif
		// 		else // default null geom object
		// 		{
		// 			SBGeomObject* colObj = new SBGeomNullObject();
		// 			pawn_p->colObj_p = colObj;
		// 		}


		SmartBody::SBPawn* sbpawn = dynamic_cast<SmartBody::SBPawn*>(pawn_p);

		if (sbpawn->getPhysicsObject())
		{
			sbpawn->setWorldOffset(sbpawn->getPhysicsObject()->getGlobalTransform().gmat());
		}
		// [BMLR] Send notification to the renderer that a pawn was created.
		// NOTE: This is sent both for characters AND pawns
#ifdef EMBER_SB_BONEBUS
#ifndef SB_NO_BONEBUS
		SmartBody::SBScene::getScene()->getBoneBusManager()->getBoneBus().SendCreatePawn( pawn_name.c_str(), loc[ 0 ], loc[ 1 ], loc[ 2 ] );
#endif
#endif
		float x,y,z,h,p,r;
		sbpawn->get_world_offset(x,y,z,h,p,r);
		//printf("h = %f, p = %f, r = %f\n",h,p,r);	
		sbpawn->set_world_offset(loc[0],loc[1],loc[2],h,p,r);	
		sbpawn->wo_cache_update();

		std::vector<SmartBody::SBSceneListener*>& listeners = scene->getSceneListeners();
		for (size_t i = 0; i < listeners.size(); i++)
		{
			listeners[i]->OnCharacterCreate( pawn_name, pawn_p->getClassType() );
		}
		
		return CMD_SUCCESS;
	}

	bool all_pawns = false;
	SbmPawn* pawn_p = NULL;
	if( pawn_name== "*" )
	{
		std::vector<std::string> pawns;
		
		for (std::vector<std::string>::iterator citer = pawns.begin();
			citer != pawns.end();
			citer++)
		{
			srArgBuffer copy_args( args.peek_string() );
			pawn_p =  SmartBody::SBScene::getScene()->getPawn( *citer );
			int err = pawn_parse_pawn_command( pawn_p, pawn_cmd, copy_args);
			if( err != CMD_SUCCESS )
				return( err );
		}
		return CMD_SUCCESS;
	} 
	else
	{
		pawn_p = SmartBody::SBScene::getScene()->getPawn( pawn_name.c_str() );
		if( pawn_p ) 
		{
			int ret = pawn_parse_pawn_command( pawn_p, pawn_cmd, args);
			return( ret );
		}
		else
		{
			LOG("No pawn named '%s' exists.", pawn_name.c_str());
			return CMD_FAILURE;
		}
	}
}

int character_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr)
{
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();

	std::string char_name = args.read_token();
	if( char_name.length()==0 ) {
		LOG( "HELP: char <> <command>" );
		LOG( "  param" );
		LOG( "  smoothbindmesh" );
		LOG( "  smoothbindweight" );
		LOG( "  mesh");
		LOG( "  ctrl" );
		LOG( "  inspect" );
		LOG( "  channels" );
		LOG( "  controllers" );
		LOG( "  prune" );
		LOG( "  viseme curveon|curveoff" );
		LOG( "  viseme timedelay <timedelay>" );
		LOG( "  viseme magnitude <amount>" );
		LOG( "  viseme <viseme name> <weight> <ramp in>" );
		LOG( "  viseme <viseme name> trap <weight> <dur> [<ramp-in> [<ramp-out>]]" );
		LOG( "  viseme <viseme name> curve <number of keys> <curve information>" );
		LOG( "  viseme curve" );
		LOG( "  viseme plateau on|off" );
		LOG( "  clampvisemes on|off" );
		LOG( "  minvisemetime <amount>" );
		LOG( "  bone" );
		LOG( "  bonep" );
		LOG( "  remove" );
		LOG( "  viewer" );
		LOG( "  gazefade in|out [<interval>]" );
		LOG( "  gazefade print" );
		LOG( "  reholster" );
		LOG( "  blink" );
		LOG( "  eyelid pitch <enable>" );
		LOG( "  eyelid range <min-angle> <max-angle> [<lower-min> <lower-max>]" );
		LOG( "  eyelid close <closed-angle>" );
		LOG( "  eyelid tight <upper-norm> [<lower-norm>]" );
		LOG( "  softeyes" );
		LOG( "  sk <file> <scale>");
		LOG( "  minibrain <on|off>");
		return( CMD_SUCCESS );
	}

	std::string char_cmd = args.read_token();
	if( char_cmd.length()==0 ) {
		LOG( "SbmCharacter::character_cmd_func: ERR: Expected character command." );
		return CMD_FAILURE;
	}

	bool all_characters = false;
	SmartBody::SBCharacter* character = NULL;
	if( char_name == "*" ) {

		all_characters = true;
		const std::vector<std::string>& characters =  scene->getCharacterNames();

		for (std::vector<std::string>::const_iterator citer = characters.begin();
			citer != characters.end();
			citer++)
		{
			srArgBuffer copy_args( args.peek_string() );
			character = scene->getCharacter( *citer );
			int err = character_parse_character_command( character, char_cmd, copy_args, true );
			if( err != CMD_SUCCESS )
				return( err );
		}
		return( CMD_SUCCESS );
	} 

	character = scene->getCharacter( char_name );
	if( character ) {

		int err = character_parse_character_command( character, char_cmd, args, false );
		if( err != CMD_NOT_FOUND )	{
			return( err );
		}
	}

		if( char_cmd == "param" )
		{

			char* param_name = args.read_token();
			GeneralParam * new_param = new GeneralParam;
			new_param->size = args.read_int();

			if( new_param->size == 0 )
			{
				LOG("SbmCharacter::parse_character_command: param_registeration ERR: parameter size not defined!\n");
				delete new_param;
				return( CMD_FAILURE );
			}
			for(int i = 0 ; i < (int)new_param->char_names.size(); i++)
			{
				if(char_name == new_param->char_names[i])
				{
					LOG("SbmCharacter::parse_character_command: param_registeration ERR: parameter redefinition!\n");
					delete new_param;
					return( CMD_FAILURE );	
				}
			}
			new_param->char_names.push_back( char_name );
			GeneralParamMap::iterator it; 
			if( (it = scene->getGeneralParameters().find(param_name)) != scene->getGeneralParameters().end())
			{
				it->second->char_names.push_back( char_name );
				delete new_param;
			}
			else
			{
				scene->getGeneralParameters().insert(make_pair(std::string(param_name),new_param));
			}
			return( CMD_SUCCESS );
		}

		LOG( "SbmCharacter::character_cmd_func ERR: char '%s' or cmd '%s' NOT FOUND", char_name.c_str(), char_cmd.c_str() );
		return( CMD_FAILURE );
}

int create_remote_pawn_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr)
{
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();

	

	std::string pawn_and_attribute = args.read_token();
	int interval = args.read_int();

	if( pawn_and_attribute.length()==0 ) {
		LOG("ERROR: Expected pawn name.");
		return CMD_FAILURE;
	}

	SbmPawn* pawn_p = NULL;

	pawn_p =  SmartBody::SBScene::getScene()->getPawn( pawn_and_attribute );

	if( pawn_p != NULL ) {
		LOG("ERROR: Pawn \"%s\" already exists.", pawn_and_attribute.c_str() );
		return CMD_FAILURE;
	}

	pawn_p = scene->createPawn( pawn_and_attribute.c_str() );

	SkSkeleton* skeleton = new SmartBody::SBSkeleton();
	skeleton->ref();
	std::string skel_name = pawn_and_attribute+"-skel";
	skeleton->setName( skel_name.c_str() );
	// Init channels
	skeleton->make_active_channels();	

	int err = pawn_p->init( skeleton );

	std::vector<SmartBody::SBSceneListener*>& listeners = scene->getSceneListeners();
	for (size_t i = 0; i < listeners.size(); i++)
	{
		listeners[i]->OnCharacterCreate( pawn_and_attribute, "" );
	}
		
	if( err != CMD_SUCCESS ) {
		LOG("ERROR: Unable to initialize SbmPawn \"%s\".", pawn_and_attribute.c_str() );
		delete pawn_p;
		skeleton->unref();
		return err;
	}

	if( err != CMD_SUCCESS )	{
		LOG("ERROR: SbmPawn pawn_map.insert(..) \"%s\" FAILED", pawn_and_attribute.c_str() );
		delete pawn_p;
		skeleton->unref();
		return err;
	}

/*
#if USE_WSP
	mcu.theWSP->subscribe_vector_3d_interval( pawn_and_attribute, "position", interval, handle_wsp_error, remote_pawn_position_update, &mcu );
	mcu.theWSP->subscribe_vector_4d_interval( pawn_and_attribute, "rotation", interval, handle_wsp_error, remote_pawn_rotation_update, &mcu );
#endif
*/
	return( CMD_SUCCESS );
}


int character_set_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr)
{
	std::string character_id = args.read_token();
	if( character_id.length()==0 ) {
		LOG("ERROR: SbmCharacter::set_cmd_func(..): Missing character id.");
		return CMD_FAILURE;
	}

	SmartBody::SBCharacter* character = SmartBody::SBScene::getScene()->getCharacter( character_id );
	if( character==NULL ) {
		LOG("ERROR: SbmCharacter::set_cmd_func(..): Unknown character \"%s\" to set.", character_id.c_str());
		return CMD_FAILURE;
	}

	std::string attribute = args.read_token();
	if( attribute.length()==0 ) {
		LOG("ERROR: SbmCharacter::set_cmd_func(..): Missing attribute to set.");
		return CMD_FAILURE;
	}

	//  voice_code and voice-code are backward compatible patches
	if( attribute=="voice" || attribute=="voice_code" || attribute=="voice-code" ) {
		return set_voice_cmd_func( character, args );
	} else if( attribute == "voicebackup") {
		return set_voicebackup_cmd_func( character, args );
	} else {
		return set_attribute( character, attribute, args );
	}
}

// Print error on error..
bool parse_float_or_error( float& var, const char* str, const std::string& var_name )
{
	if( std::istringstream( str ) >> var )
		return true; // no error
	// else
	LOG("ERROR: Invalid value for %s: %s", var_name.c_str(), str);
	return false;
}


int set_world_offset_cmd( SbmPawn* pawn, srArgBuffer& args )
{
	float x, y, z, h, p, r;
	pawn->get_world_offset( x, y, z, h, p, r );

	bool has_error = false;
	std::string arg = args.read_token();
	if( arg.length() == 0 ) {
		LOG("ERROR: SbmPawn::set_world_offset: Missing offset parameters.");
		return CMD_FAILURE;
	}

	while( arg.length() > 0 ) {
		// TODO: handle "+x", "-x", etc...
		if( arg=="x" ) {
			has_error |= !parse_float_or_error( x, args.read_token(), arg );
		} else if( arg=="y" ) {
			has_error |= !parse_float_or_error( y, args.read_token(), arg );
		} else if( arg=="z" ) {
			has_error |= !parse_float_or_error( z, args.read_token(), arg );
		} else if( arg=="z" ) {
			has_error |= !parse_float_or_error( z, args.read_token(), arg );
		} else if( arg=="p" || arg=="pitch" ) {
			has_error |= !parse_float_or_error( p, args.read_token(), "pitch" );
		} else if( arg=="r" || arg=="roll" ) {
			has_error |= !parse_float_or_error( r, args.read_token(), "roll" );
		} else if( arg=="h" || arg=="heading" || arg=="yaw" ) {
			has_error |= !parse_float_or_error( h, args.read_token(), "yaw" );
		} else if( arg=="xyz" || arg=="pos" || arg=="position" ) {
			has_error |= !parse_float_or_error( x, args.read_token(), "x" );
			has_error |= !parse_float_or_error( y, args.read_token(), "y" );
			has_error |= !parse_float_or_error( z, args.read_token(), "z" );
		} else if( arg=="hpr" ) {
			has_error |= !parse_float_or_error( h, args.read_token(), "heading" );
			has_error |= !parse_float_or_error( p, args.read_token(), arg );
			has_error |= !parse_float_or_error( r, args.read_token(), arg );
		} else {
			LOG("ERROR: Unknown world_offset attribute \"%s\".", arg.c_str());
			has_error = true;
		}
		arg = args.read_token();
	}

	if( has_error )
		return CMD_FAILURE;

	pawn->set_world_offset( x, y, z, h, p, r );
	return CMD_SUCCESS;
}

int pawn_parse_pawn_command( SbmPawn* pawn, std::string cmd, srArgBuffer& args)
{
	

	if (cmd == "remove")
	{	
		SmartBody::SBScene::getScene()->removePawn(pawn->getName());
		return CMD_SUCCESS;
	}
	else if (cmd == "prune")
	{
		int result = pawn->prune_controller_tree();
		if( result != CMD_SUCCESS )
		{
			LOG("ERROR: Failed to prune pawn \"%s\"", pawn->getName().c_str());
			return CMD_FAILURE;
		}
		else 
		{
			return CMD_SUCCESS;
		}
	}
	else if (cmd == "setshape")
	{
		std::string geom_str = "box", color_str = "red", file_str = "", type_str = "";
		bool setRec = false;
		bool has_geom = false;
		SrVec size = SrVec(1.f,1.f,1.f);		
		while( args.calc_num_tokens() > 0 )
		{
			std::string option = args.read_token();
			// TODO: Make the following option case insensitive
			if( option=="geom" ) {
				geom_str = args.read_token();
				has_geom = true;
			} else if( option=="size" ) {
				//size_str = args.read_token();
				float uniformSize = args.read_float();
				for (int i=0;i<3;i++)
					size[i] = uniformSize;//args.read_float();
				has_geom = true;

			} else if (option=="file" ) {
				file_str = args.read_token();
				has_geom = true;			
			} else if( option=="color" ) {
				color_str = args.read_token();
				has_geom = true;
			} else if( option=="type" ) {
				type_str = args.read_token();
				has_geom = true;
			} 
		#ifdef EMBER_SB_STEER
			else if( option=="rec" ) {
				setRec = true;

				size[0] = pawn->steeringSpaceObjSize.x = args.read_float();
				size[1] = pawn->steeringSpaceObjSize.y = args.read_float();
				size[2] = pawn->steeringSpaceObjSize.z = args.read_float();

				has_geom = true;
			} 
		#endif
			else {
				std::stringstream strstr;
				strstr << "WARNING: Unrecognized pawn setshape option \"" << option << "\"." << endl;
				LOG(strstr.str().c_str());
			}
		}	

		if (has_geom)
		{				
			//initGeomObj(geom_str.c_str(),size,color_str.c_str(),file_str.c_str());
			SmartBody::SBPhysicsManager* phyManager = SmartBody::SBScene::getScene()->getPhysicsManager();
			phyManager->createPhysicsPawn(pawn->getName(),geom_str,size);
			// init steering space
		#ifdef EMBER_SB_STEER
			if (!setRec)
				pawn->steeringSpaceObjSize = size;//SrVec(size, size, size);
			if (type_str == "steering")
				pawn->initSteeringSpaceObject();
		#endif
			return CMD_SUCCESS;
		}
		else
		{
			LOG("Pawn %s, fail to setshape. Incorrect parameters.", pawn->getName().c_str());
			return CMD_FAILURE;
		} 
	}
	else if (cmd == "physics")
	{
		std::string option = args.read_token();

		bool turnOn = false;
		if (option == "on" || option == "ON")
			turnOn = true;			
		else if (option == "off" || option == "OFF")
			turnOn = false;			
		else
			return CMD_FAILURE;

		SmartBody::SBPawn* sbpawn = dynamic_cast<SmartBody::SBPawn*>(pawn);
		SmartBody::SBPhysicsObj* phyObj = sbpawn->getPhysicsObject();
		if (phyObj) phyObj->enablePhysicsSim(turnOn);

		//setPhysicsSim(turnOn);
		return CMD_SUCCESS;
	}
	else if (cmd == "collision")
	{	
		std::string option = args.read_token();
		bool turnOn = false;
		if (option == "on" || option == "ON")
			turnOn = true;			
		else if (option == "off" || option == "OFF")
			turnOn = false;			
		else
			return CMD_FAILURE;

		SmartBody::SBPawn* sbpawn = dynamic_cast<SmartBody::SBPawn*>(pawn);
		SmartBody::SBPhysicsObj* phyObj = sbpawn->getPhysicsObject();
		if (phyObj) phyObj->enableCollisionSim(turnOn);

		//setCollision(turnOn);			
		return CMD_SUCCESS;
	}
	else
	{
		return CMD_FAILURE;
	}
}

int character_parse_character_command( SbmCharacter* character, std::string cmd, srArgBuffer& args, bool all_characters )
{

	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	SmartBody::SBCharacter* sbChar = dynamic_cast<SmartBody::SBCharacter*>(character);
	
	 if ( cmd == "remove" ) {
				SmartBody::SBScene::getScene()->removeCharacter(character->getName());
				return CMD_SUCCESS;

			} 
			else if( cmd == "inspect" ) {
					if( character->getSkeleton() ) {
						SkJoint* joint_p = character->getSkeleton()->search_joint( SbmPawn::WORLD_OFFSET_JOINT_NAME );
						if( joint_p )	{
							character->inspect_skeleton( joint_p );
							//				inspect_skeleton_local_transform( joint_p );
							//				inspect_skeleton_world_transform( joint_p );
						}
					}
					return CMD_SUCCESS;
				}
				else
					if( cmd == "channels" ) {

						if( character->getSkeleton() )
						{
							if( character->ct_tree_p ) 
							{
								SkChannelArray channels = character->getSkeleton()->channels();
								int numChannels = channels.size();
								for (int c = 0; c < numChannels; c++)
								{
									std::stringstream strstr;
									strstr << c << " ";
									SkJoint* joint = channels.joint(c);
									if (joint)
									{
										strstr << joint->jointName() << " ";
									}
									SkChannel& channel = channels[c];
									int channelSize = channel.size();
									// get the channel index
									int channelIndex = character->ct_tree_p->toBufferIndex(c);
									strstr << channelIndex << " (" << channelSize << ") ";
									LOG( "%s", strstr.str().c_str() );
								}
							}
						}
						return CMD_SUCCESS;
					}
					else 
						if( cmd == "controllers" )
						{
							if( character->ct_tree_p )
							{
								int n = character->ct_tree_p->count_controllers();
								for (int c = 0; c < n; c++)
								{
									LOG( "%s", character->ct_tree_p->controller(c)->getName().c_str() );
								}
							}
							return CMD_SUCCESS;
						}
						else 
							if (cmd == "requests")
							{
								BML::Processor* bp = SmartBody::SBScene::getScene()->getBmlProcessor()->getBMLProcessor();
								for (std::map<std::string, BML::BmlRequestPtr >::iterator iter = bp->getBMLRequestMap().begin();
									iter != bp->getBMLRequestMap().end();
									iter++)
								{
									if (all_characters)
									{
										LOG("%s", (*iter).second->requestId.c_str());
									}
									else
									{			
										// make sure the requests is for this character
										std::string requestWithName = (*iter).second->requestId;
										std::string charName = character->getName();
										charName.append("|");
										int index = requestWithName.find(charName);
										if (index == 0)
										{
											LOG("%s", (*iter).second->requestId.c_str());
										}
									}
								}
								return CMD_SUCCESS;
							}
							if (cmd == "interrupt")
							{
								int numRequestsInterrupted = 0;
								BML::Processor* bp = SmartBody::SBScene::getScene()->getBmlProcessor()->getBMLProcessor();
								for (std::map<std::string, BML::BmlRequestPtr >::iterator iter = bp->getBMLRequestMap().begin();
									iter != bp->getBMLRequestMap().end();
									iter++)
								{
									std::string requestWithName = (*iter).second->requestId;
									if (all_characters)
									{
										int pipeLocation = requestWithName.find("|");
										std::string charName = requestWithName.substr(0, pipeLocation);
										std::string request = requestWithName.substr(pipeLocation + 1);
										std::stringstream strstr;
										strstr << "bp interrupt " << charName << " " << request << " .5"; 
										SmartBody::SBScene::getScene()->getCommandManager()->execute((char*) strstr.str().c_str());
										numRequestsInterrupted++;
									}
									else
									{			
										// make sure the requests is for this character

										std::string charName = character->getName();
										charName.append("|");
										int index = requestWithName.find(charName);
										if (index == 0)
										{
											std::string request = requestWithName.substr(charName.size());
											std::stringstream strstr;
											strstr << "bp interrupt " << character->getName() << " " << request << " .5"; 
											SmartBody::SBScene::getScene()->getCommandManager()->execute((char*) strstr.str().c_str());
											numRequestsInterrupted++;
										}
									}
									std::string name = character->getName();
									LOG("%d requests interrupted on character %s.", numRequestsInterrupted, name.c_str());
								}
								return CMD_SUCCESS;
							}
							else if( cmd == "prune" ) {
						return( character->prune_controller_tree( ) );
					}
					else if( cmd == "viseme" )
					{ 
							char* viseme = args.read_token();
							char* next = args.read_token();
							//		float* curveInfo = NULL;
							//		float weight = 0.0f;
							//		float rampin_duration = 0.0;
							//		int numKeys = 0;
							//		int numKeyParams = 0;

							if( _stricmp( viseme, "curveon" ) == 0 )
							{
								character->set_viseme_curve_mode(true);
								return CMD_SUCCESS;
							}
							else if ( _stricmp( viseme, "reset") == 0)
							{
								// reset all of the visemes and action units to zero
								// clear away all the controllers on the head schedule
								MeCtScheduler2* scheduler = character->head_sched_p;
								if (!scheduler)
								{
									LOG("No scheduler available");
									return CMD_SUCCESS;
								}
								std::vector<MeCtScheduler2::TrackPtr> tracksToRemove;
								MeCtScheduler2::VecOfTrack tracks = scheduler->tracks();
								for (size_t t = 0; t < tracks.size(); t++)
								{
									MeCtScheduler2::TrackPtr track = tracks[t];
									MeController* controller = track->animation_ct();
									MeCtChannelWriter* channelWriter = dynamic_cast<MeCtChannelWriter*>(controller);
									if (channelWriter)
									{
										tracksToRemove.push_back(track);
									}
								}
								scheduler->remove_tracks(tracksToRemove);
								LOG("Removed %d visemes/Action Units", tracksToRemove.size());
								return CMD_SUCCESS;
							}

							else if( _stricmp( viseme, "curveoff" ) == 0 )
							{
								character->set_viseme_curve_mode(false);
								return CMD_SUCCESS;
							}
							else if( _stricmp( viseme, "timedelay" ) == 0 )
							{
								float timeDelay = (float)atof( next );
								character->set_viseme_time_delay( timeDelay );
								return CMD_SUCCESS;
							}
							if( _stricmp( viseme, "sounddelay" ) == 0 )
							{
								float soundDelay = (float)atof( next );
								character->set_viseme_sound_delay( soundDelay );
								return CMD_SUCCESS;
							}
							if( _stricmp( viseme, "magnitude" ) == 0 )
							{
								float magnitude = (float)atof( next );
								character->set_viseme_magnitude( magnitude );
								return CMD_SUCCESS;
							}
							if( _stricmp( viseme, "plateau" ) == 0 )
							{
								if (!next)
								{
									LOG("Character %s viseme plateau setting is %s", character->getName().c_str(), character->isVisemePlateau()? "on" : "off");
									return CMD_SUCCESS;
								}
								if (_stricmp(next, "on") == 0)
								{
									character->setVisemePlateau(true);
									LOG("Character %s viseme plateau setting is now on.", character->getName().c_str());
								}
								else if (_stricmp(next, "off") == 0)
								{
									character->setVisemePlateau(false);
									LOG("Character %s viseme plateau setting is now off.", character->getName().c_str());
								}
								else
								{
									LOG("use: char %s viseme plateau <on|off>", character->getName().c_str());
								}
								return CMD_SUCCESS;
							}
							if (_stricmp( viseme, "diphone" ) == 0 )
							{
								if (!next)
								{
									LOG("Character %s diphone setting is %s", character->getName().c_str(), character->isDiphone()? "on" : "off");
									return CMD_SUCCESS;
								}
								if (_stricmp(next, "on") == 0)
								{
									character->setDiphone(true);
									LOG("Character %s diphone setting is now on.", character->getName().c_str());
								}
								else if (_stricmp(next, "off") == 0)
								{
									character->setDiphone(false);
									LOG("Character %s diphone setting is now off.", character->getName().c_str());
								}
								else
								{
									LOG("use: char %s diphone <on|off>", character->getName().c_str());
								}
								return CMD_SUCCESS;
							}
							if( strcmp( viseme, "minvisemetime" ) == 0 )
							{
								if (!next)
								{
									LOG("Character %s min viseme time is %f", character->getName().c_str(), character->getMinVisemeTime());
									return CMD_SUCCESS;
								}
								float minTime = (float)atof( next );
								character->setMinVisemeTime( minTime );
								return CMD_SUCCESS;
							}

							// keyword next to viseme
							if( strcmp( viseme, "clear" ) == 0 ) // removes all head controllers
							{
								if (character->head_sched_p)
								{
									std::vector<MeCtScheduler2::TrackPtr> tracks = character->head_sched_p->tracks();
									character->head_sched_p->remove_tracks(tracks);
								}
							}
							else if( strcmp( next, "curve" ) == 0 )
							{
								int numKeys = args.read_int();
								if( numKeys <= 0 )	
								{
									LOG( "Viseme data is missing" );
									return CMD_FAILURE;
								}
								int num_remaining = args.calc_num_tokens();
								int numKeyParams = num_remaining / numKeys;
								if( num_remaining != numKeys * numKeyParams )	{
									LOG( "Viseme data is malformed" );
									return CMD_FAILURE;
								}
								float* curveInfo = new float[ num_remaining ];
								args.read_float_vect( curveInfo, num_remaining );											

								//			schedule_viseme_blend_curve( viseme, mcu_p->time, 1.0f, curveInfo, numKeys, numKeyParams );
								character->schedule_viseme_curve( viseme, SmartBody::SBScene::getScene()->getSimulationManager()->getTime(), curveInfo, numKeys, numKeyParams, 0.0f, 0.0f );
								delete [] curveInfo;
							}
							else if( _stricmp( next, "trap" ) == 0 )
							{
								// trap <weight> <dur> [<ramp-in> <ramp-out>]
								float weight = args.read_float();
								float dur = args.read_float();
								float ramp_in = 0.1f;
								float ramp_out = 0.1f;
								if( args.calc_num_tokens() > 0 )
									ramp_in = args.read_float();
								if( args.calc_num_tokens() > 0 )
									ramp_out = args.read_float();
								character->schedule_viseme_trapezoid( viseme, SmartBody::SBScene::getScene()->getSimulationManager()->getTime(), weight, dur, ramp_in, ramp_out );
							}
							else
							{
								float weight = (float)atof(next);
								float rampin_duration = args.read_float();
								character->schedule_viseme_blend_ramp( viseme, SmartBody::SBScene::getScene()->getSimulationManager()->getTime(), weight, rampin_duration );
							}
							return CMD_SUCCESS;
						}
						else if (cmd == "visemeweight")
						{
							SmartBody::SBFaceDefinition* faceDefinition = character->getFaceDefinition();
							if (!faceDefinition)
							{
								LOG("Character %s does not have any visemes defined.", character->getName().c_str());
								return CMD_FAILURE;
							}
							int numRemaining = args.calc_num_tokens();
							if (numRemaining == 0)
							{
								// dump all of the existing viseme weights
								
								int numVisemes = faceDefinition->getNumVisemes();
								for (int v = 0; v < numVisemes; v++)
								{
									const std::string& visemeName = faceDefinition->getVisemeName(v);
									float weight = faceDefinition->getVisemeWeight(visemeName);
									LOG("%s %f", visemeName.c_str(), weight);

								}
								return CMD_SUCCESS;
							}
							if (numRemaining == 1)
							{
								std::string visemeName = args.read_token();
								if (!faceDefinition->hasViseme(visemeName))
								{
									LOG("Character %s does not have viseme %s defined.", character->getName().c_str(), visemeName.c_str());
									return CMD_FAILURE;
								}
								float weight = faceDefinition->getVisemeWeight(visemeName);
								LOG("%s %f", visemeName.c_str(), weight);
								return CMD_SUCCESS;
							}
							if (numRemaining == 2)
							{
								std::string visemeName = args.read_token();
								float weight = args.read_float();
								if (visemeName == "*")
								{
									// change all of the visemes
									int numVisemes = faceDefinition->getNumVisemes();
									for (int v = 0; v < numVisemes; v++)
									{
										std::string viseme = faceDefinition->getVisemeName(v);
										faceDefinition->setVisemeWeight(viseme, weight);
									}
									LOG("Set all visemes to weight %f", visemeName.c_str(), weight);
									return CMD_SUCCESS;
								}
								if (!faceDefinition->hasViseme(visemeName))
								{
									LOG("Character %s does not have viseme %s defined.", character->getName().c_str(), visemeName.c_str());
									return CMD_FAILURE;
								}
								faceDefinition->setVisemeWeight(visemeName, weight);
								LOG("%s %f", visemeName.c_str(), weight);
								return CMD_SUCCESS;
							}
							if (numRemaining > 2)
							{
								LOG("Usage:\nchar %s visemeweight\nchar %s visemeweight <visemename>\nchar %s visemeweight <visemename> <weight>", character->getName().c_str(), character->getName().c_str(), character->getName().c_str());
								return CMD_FAILURE;
							}

						}
					else 	if( cmd == "viewer" )
					{
							std::string viewType = args.read_token();
							if (viewType == "0" || viewType == "bones")
							{
								character->setStringAttribute("displayType", "bones");
							}
							else if (viewType == "1" || viewType == "visgeo")
							{
								character->setStringAttribute("displayType", "visgeo");
							}
							else if (viewType == "2" || viewType == "colgeo")
							{
								character->setStringAttribute("displayType", "colgeo");
							}
							else if (viewType == "3" || viewType == "axis")
							{
								character->setStringAttribute("displayType", "axis");
							}
							else if (viewType == "4" || viewType == "deformable")
							{
								character->setStringAttribute("displayType", "mesh");
							}
						#ifdef EMBER_SB_GPU
							else if (viewType == "5" || viewType == "deformableGPU")
							{
								character->setStringAttribute("displayType", "GPUmesh");
							}
						#endif
							else
							{
								LOG("Usage: char <name> viewer <bones|visgeo|colgeo|axis|deformable>");
							}
							return CMD_SUCCESS;
						} 
			else if( cmd == "gazefade" ) {

					std::string fade_cmd = args.read_token();
					bool fade_in;
					bool print_track = false;
					if( fade_cmd == "in" ) {
						fade_in = true;
					}
					else
						if( fade_cmd == "out" ) {
							fade_in = false;
						}
						else
							if( fade_cmd == "print" ) {
								print_track = true;
							}
							else	{
								return( CMD_NOT_FOUND );
							}
							float interval = args.read_float();
							if( print_track )	{
								LOG( "char '%s' gaze tracks:",character->getName().c_str() );
							}
							
							double curTime = SmartBody::SBScene::getScene()->getSimulationManager()->getTime();
							MeCtScheduler2::VecOfTrack track_vec = character->gaze_sched_p->tracks();
							int n = track_vec.size();
							for( int i = 0; i < n; i++ )	{
								MeCtScheduler2::TrackPtr t_p = track_vec[ i ];
								MeCtBlend* blend = dynamic_cast<MeCtBlend*>(t_p->blending_ct()); 
								MeController* ct_p = t_p->animation_ct();
								MeCtGaze* gaze_p = dynamic_cast<MeCtGaze*> (ct_p);
								if( gaze_p )	{	
									if (blend) {
										// don't fade gaze controllers that are scheduled 
										// but have not yet been started

										srLinearCurve& blend_curve = blend->get_curve();
										int n = blend_curve.get_num_keys();
										if( n > 0 )	{
											double h = blend_curve.get_head_param();
											double v = blend_curve.get_head_value();
											if (h > curTime) // controller hasn't started yet
											{
												continue;
											}
										}
									}
									if( print_track )	{
										LOG( " %s", gaze_p->getName().c_str() );
									}
									else
										if( fade_in )	{
											gaze_p->set_fade_in( interval );
										}
										else	{
											gaze_p->set_fade_out( interval );
										}
								}
							}
							return CMD_SUCCESS;
				} 	
				else 

						if( cmd == "blink" )
						{
							if( character->eyelid_reg_ct_p )	{
								character->eyelid_reg_ct_p->blink_now();
								return( CMD_SUCCESS );
							}
							return( CMD_FAILURE );
						}
						else
							if (cmd == "breathing")
							{
								if (character->breathing_p)
								{
									return mcu_character_breathing(character->getName().c_str(), args, SmartBody::SBScene::getScene()->getCommandManager());
								}
								return CMD_FAILURE;
							}
							else
								if( cmd == "eyelid" )
								{
									if( character->eyelid_reg_ct_p )	{

										std::string eyelid_cmd  = args.read_token();
										if( eyelid_cmd.length()==0 ) {

											LOG( "char <> eyelid <command>:" );
											LOG( " eyelid print" );
											LOG( " eyelid pitch 0|1" );
											LOG( " eyelid range <upper-min> <upper-max> [<lower-min> <lower-max>]" );
											LOG( " eyelid close <closed-angle>" );
											LOG( " eyelid tight <upper-weight> [<lower-weight>]" );
											LOG( " eyelid delay <upper-delay> [<upper-delay>]" );


											//				eyelid_reg_ct_p->test();
											return( CMD_SUCCESS );
										}

										int n = args.calc_num_tokens();
										if( eyelid_cmd == "pitch" )
										{
											if( n > 0 )	{
												bool enable = args.read_int() != 0;
												character->eyelid_reg_ct_p->set_eyeball_tracking( enable );
											}
											else	{
												LOG( "MeCtEyeLidRegulator: pitch tracking %s", 
													character->eyelid_reg_ct_p->get_eyeball_tracking() ?
													"ENABLED" : "DISABLED"
													);
											}
											return( CMD_SUCCESS );
										}
										else
											if( eyelid_cmd == "range" )
											{
												if( n < 2 )	{
													return( CMD_FAILURE );
												}
												float upper_min = args.read_float();
												float upper_max = args.read_float();
												character->eyelid_reg_ct_p->set_upper_range( upper_min, upper_max );
												if( n >= 4 )	{
													float lower_min = args.read_float();
													float lower_max = args.read_float();
													character->eyelid_reg_ct_p->set_lower_range( lower_min, lower_max );
												}
												return( CMD_SUCCESS );
											}
											else
												if( eyelid_cmd == "close" )
												{
													if( n < 1 ) {
														return( CMD_FAILURE );
													}
													float close_angle = args.read_float();
													character->eyelid_reg_ct_p->set_close_angle( close_angle );
													return( CMD_SUCCESS );
												}
												else
													if( eyelid_cmd == "tight" )
													{
														float upper_mag = args.read_float();
														character->eyelid_reg_ct_p->set_upper_tighten( upper_mag );
														if( n > 1 ) {
															float lower_mag = args.read_float();
															character->eyelid_reg_ct_p->set_lower_tighten( lower_mag );
														}
														return( CMD_SUCCESS );
													}
													if( eyelid_cmd == "delay" )
													{
														float upper_delay = args.read_float();
														character->eyelid_reg_ct_p->set_upper_delay( upper_delay );
														if( n > 1 ) {
															float lower_delay = args.read_float();
															character->eyelid_reg_ct_p->set_lower_delay( lower_delay );
														}
														return( CMD_SUCCESS );
													}
													if( eyelid_cmd == "print" )
													{
														character->eyelid_reg_ct_p->print();
														return( CMD_SUCCESS );
													}
													return( CMD_NOT_FOUND );
									}
									return( CMD_FAILURE );
								}
								else
									if( cmd == "softeyes" )
									{
										if(character->eyelid_ct == NULL )
										{
											LOG("ERROR: SbmCharacter::parse_character_command(..): character \"%s\" has no eyelid_ct.", character->getName().c_str() );
											return CMD_FAILURE;
										}

										if( args.calc_num_tokens() == 0 )
										{
											LOG( "softeyes params: %s", character->isSoftEyes() ? "ENABLED" : "DISABLED" );
											float lo, up;

											character->eyelid_ct->get_weight( lo, up );
											LOG( " eyelid weight: { %f, %f }", lo, up );

											character->eyelid_ct->get_upper_lid_range( lo, up );
											LOG( " eyelid upper trans: { %f, %f }", lo, up );

											character->eyelid_ct->get_lower_lid_range( lo, up );
											LOG( " eyelid lower trans: { %f, %f }", lo, up );

											character->eyelid_ct->get_eye_pitch_range( lo, up );
											LOG( " eyeball pitch: { %f, %f }", lo, up );

											LOG( "commmands:" );
											LOG( " char <> softeyes [on|off] " );
											LOG( " char <> softeyes weight <lower> <upper>" );
											LOG( " char <> softeyes upperlid|lowerlid|eyepitch <lower-lim> <upper-lim>" );
											return CMD_SUCCESS;
										}

										std::string softEyesCommand = args.read_token();
										if( softEyesCommand == "on")
										{
											character->setSoftEyes( true );
										}
										else 
											if( softEyesCommand == "off")
											{
												character->setSoftEyes( false );
											}
											else	{

												float lo = args.read_float();
												float up = args.read_float();

												if( softEyesCommand == "weight" )
												{
													character->eyelid_ct->set_weight( lo, up );
												}
												else
													if( softEyesCommand == "upperlid" )
													{
														character->eyelid_ct->set_upper_lid_range( lo, up );
													}
													else 
														if( softEyesCommand == "lowerlid" )
														{
															character->eyelid_ct->set_lower_lid_range( lo, up );
														}
														else 
															if( softEyesCommand == "eyepitch" )
															{
																character->eyelid_ct->set_eye_pitch_range( lo, up );
															}
															else
															{
																LOG( "SbmCharacter::parse_character_command ERR: command '%s' not recognized", softEyesCommand.c_str());
																return CMD_NOT_FOUND;
															}
															return CMD_SUCCESS;
											}
											return CMD_SUCCESS;
									}
									else if (cmd == "sk")
									{
										std::string file = args.read_token();
										std::string scaleStr = args.read_token();
										double scale = atof(scaleStr.c_str());
										return character->writeSkeletonHierarchy(file, scale);		
									}
									else if (cmd == "minibrain")
									{
										if (args.calc_num_tokens() == 0)
										{
											SmartBody::MiniBrain* miniBrain = character->getMiniBrain();
											if (miniBrain)
											{
												LOG("Character %s has an active minibrain.", character->getName().c_str());
											}
											else
											{
												LOG("Character %s has an inactive minibrain.", character->getName().c_str());
											}
											return CMD_SUCCESS;
										}

										std::string tok = args.read_token();
										if (tok == "on")
										{
											SmartBody::MiniBrain* miniBrain = character->getMiniBrain();
											if (miniBrain)
											{
												delete miniBrain;
											}
											miniBrain = new SmartBody::MiniBrain();
											character->setMiniBrain(miniBrain);
											LOG("Minibrain for character %s is now on", character->getName().c_str());
										}
										else if (tok == "off")
										{
											SmartBody::MiniBrain* miniBrain = character->getMiniBrain();
											if (miniBrain)
											{
												delete miniBrain;
											}
											
											character->setMiniBrain(NULL);
											LOG("Minibrain for character %s is now off", character->getName().c_str());
										}
										else
										{
											LOG("Usage: char %s minibrain <on|off>", character->getName().c_str());
										}
										return CMD_SUCCESS;
									}
// 									else if ( cmd == "collision")
// 									{
// 										string phyCmd = args.read_token();
// 										if (phyCmd == "on" || phyCmd == "ON")
// 										{
// 											//character->setJointPhyCollision(true);
// 											return CMD_SUCCESS;
// 										}
// 										else if (phyCmd == "off" || phyCmd == "OFF")
// 										{
// 											//character->setJointPhyCollision(false);
// 											return CMD_SUCCESS;
// 										}
// 										else
// 										{
// 											LOG( "SbmCharacter::parse_character_command ERR: incorrect parameter for collision = %s",phyCmd.c_str());
// 											return CMD_FAILURE;
// 										}
// 									}
// 									else if ( cmd == "collider" )
// 									{
// 										string colCmd = args.read_token();
// 										if (colCmd == "build") // build all joint colliders automatically
// 										{
// 											character->buildJointPhyObjs();
// 											return CMD_SUCCESS;
// 										}										
// 									}
									else if ( cmd == "handmotion")
									{
										std::string hand_cmd = args.read_token();	
										if (hand_cmd == "grabhand" || hand_cmd == "reachhand" || hand_cmd == "releasehand")
										{
											std::string motion_name = args.read_token();
											std::string tagName = args.read_token();
											SmartBody::SBMotion* motion = SmartBody::SBScene::getScene()->getAssetManager()->getMotion(motion_name);

											SmartBody::SBReach* reach = sbChar->getReach();
											//LOG("SbmCharacter::parse_character_command LOG: add motion name : %s ", motion_name.c_str());
											int reachType = MeCtReachEngine::getReachType(tagName);//
											if (reachType == -1)
												reachType = MeCtReachEngine::RIGHT_ARM;
											if (motion)
											{
												//addReachMotion(motion);
												TagMotion tagMotion = TagMotion(reachType,motion);
												if (hand_cmd == "grabhand")
													reach->setGrabHandMotion(tagName,motion);
												else if (hand_cmd == "reachhand")
													reach->setReachHandMotion(tagName,motion);
												else if (hand_cmd == "releasehand")
													reach->setReleaseHandMotion(tagName,motion);

												return CMD_SUCCESS;
											}
											else
											{
												LOG( "SbmCharacter::parse_character_command ERR: motion '%s' not found", motion_name.c_str());
												return CMD_FAILURE;
											}
										}
									}
									else if ( cmd == "reachmotion" )
									{
										std::string reach_cmd = args.read_token();		
										bool print_track = false;
										SmartBody::SBReach* reach = sbChar->getReach();
										if (reach_cmd == "add")
										{			
											string motion_name = args.read_token();		
											string tagName = args.read_token();
											int reachType = MeCtReachEngine::getReachType(tagName);//
											if (reachType == -1)
												reachType = MeCtReachEngine::RIGHT_ARM;
											SmartBody::SBMotion* motion = SmartBody::SBScene::getScene()->getAssetManager()->getMotion(motion_name);
											//LOG("SbmCharacter::parse_character_command LOG: add motion name : %s ", motion_name.c_str());
											if (motion && reach)
											{
												// assume the right hand motion and mirror the left hand motion
												reach->addMotion(tagName,motion);
												return CMD_SUCCESS;
											}
											else
											{
												LOG( "SbmCharacter::parse_character_command ERR: motion '%s' not found", motion_name.c_str());
												return CMD_NOT_FOUND;
											}
										}
										else if (reach_cmd == "list")
										{
#if 0
											int motion_num = character->reachMotionData->size();
											//SkMotion* motion = getReachMotion(motion_num);
											std::vector<std::string> reachMotionNames = reach->getMotionNames();
											for (unsigned int c = 0; c < reachMotionNames.size(); c++)
											{
												LOG( "%s", reachMotionNames[c].c_str() );
											}
#endif
											return CMD_SUCCESS;
										}
										else if (reach_cmd == "build")
										{			
											if (character->reachEngineMap->size() == 0)
											{
												LOG("character %s, reach engine is not initialized.", character->getName().c_str());
												return CMD_FAILURE;
											}				
											SmartBody::SBReach* reach = sbChar->getReach();
											ReachEngineMap::iterator mi;
											for ( mi  = character->reachEngineMap->begin();
												mi != character->reachEngineMap->end();
												mi++)
											{
												MeCtReachEngine* re = mi->second;
												if (re)
												{
													//re->updateMotionExamples(getReachMotionDataSet(),"KNN");
													re->updateMotionExamples(reach->getReachMotionDataSet(),"Inverse");
												}
											}
											return (CMD_SUCCESS);
										}			
										else if (reach_cmd == "play")
										{
#if 0
											int motion_num = args.read_int();
											SkMotion* motion = character->getReachMotion(motion_num);
											if (motion)
											{
												//motion->name()
												char cmd[256];
												sprintf(cmd,"bml char %s <body posture=\"%s\"/>", character->getName().c_str(),motion->getName().c_str());
												SmartBody::SBScene::getScene()->getCommandManager()->execute(cmd);
											}			
#endif
											return CMD_SUCCESS;
										}
										return CMD_FAILURE;
									}
									return( CMD_NOT_FOUND );
}

