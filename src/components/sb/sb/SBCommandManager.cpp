#include "SBCommandManager.h"
#include "SBScene.h"
#include "SBAttribute.h"
#include "SBAssetManager.h"
#include "SBSimulationManager.h"
#include <sstream>
#include "components/sb/sbm/mcontrol_callbacks.h"
#include "components/sb/sbm/MiscCommands.h"
#include "components/sb/sbm/sbm_test_cmds.hpp"
#include "components/sb/sbm/text_speech.h" // ?
#include "components/sb/sbm/local_speech.h" // ?
#include "components/sb/bml/bml_processor.hpp"
#include "components/sb/bml/bml.hpp"


SequenceManager::SequenceManager()
{
}

SequenceManager::~SequenceManager()
{
	clear();
}

void SequenceManager::clear()
{
	for (size_t x = 0; x < _sequences.size(); x++)
	{
		srCmdSeq* seq = _sequences[x].second;
		seq->reset();
		while(char* cmd = seq->pull() )	{
			delete [] cmd;
		}
		delete seq;
	}

	_sequenceSet.clear();
	_sequences.clear();
}

bool SequenceManager::addSequence(const std::string& seqName, srCmdSeq* seq)
{
	if (_sequenceSet.find(seqName) != _sequenceSet.end())
		return false;

	_sequenceSet.insert(seqName);
	_sequences.push_back(std::pair<std::string, srCmdSeq*>(seqName, seq));
	return true;
}

bool SequenceManager::removeSequence(const std::string& seqName, bool deleteSequence)
{
	std::set<std::string>::iterator iter = _sequenceSet.find(seqName);
	if (iter == _sequenceSet.end())
		return false;

	_sequenceSet.erase(iter);

	for (std::vector<std::pair<std::string, srCmdSeq*> >::iterator iter = _sequences.begin();
		iter != _sequences.end();
		iter++)
	{
		if ((*iter).first == seqName)
		{
			/*
			if (deleteSequence)
				delete (*iter).second;
			_sequences.erase(iter);
			*/
			// need to mark the sequences as invalid for later removal
			(*iter).second->setValid(false);

			return true;
		}
	}

	LOG("Could not find sequence in active sequence queue. Please check code - this should not happen.");
	return false;
}

srCmdSeq* SequenceManager::getSequence(const std::string& name)
{
	for (std::vector<std::pair<std::string, srCmdSeq*> >::iterator iter = _sequences.begin();
		iter != _sequences.end();
		iter++)
	{
		if ((*iter).first == name)
		{
			return (*iter).second;
		}
	}

	return NULL;
}

srCmdSeq* SequenceManager::getSequence(int num, std::string& name)
{
	if (num < (int) _sequences.size())
	{
		name = _sequences[num].first;
		return _sequences[num].second;
	}
	else
	{
		return NULL;
	}
}

int SequenceManager::getNumSequences()
{
	return _sequences.size();
}

void SequenceManager::cleanupMarkedSequences()
{
	bool hasInvalidSequences = true;
	while (hasInvalidSequences)
	{
		hasInvalidSequences = false;
		for (std::vector<std::pair<std::string, srCmdSeq*> >::iterator iter = _sequences.begin();
			iter != _sequences.end();
			iter++)
		{
			if (!(*iter).second->isValid())
			{
				srCmdSeq* seq = (*iter).second;
				delete seq;
				_sequences.erase(iter);
				hasInvalidSequences = true;
				break;
			}
		}
	}
}




namespace SmartBody {



SBCommandManager::SBCommandManager()
{
	registerCallbacks();
}

SBCommandManager::~SBCommandManager()
{
}

bool SBCommandManager::hasCommand(const std::string& command)
{
	int ret = cmd_map.is_command(command.c_str());
	if (ret == 1)
		return true;
	else
		return false;
}



int SBCommandManager::insert_set_cmd( const char *key, srCmdMap<SBCommandManager>::sr_cmd_callback_fp fp )
{
	return( set_cmd_map.insert( key, fp ) );
}

int SBCommandManager::insert_print_cmd( const char *key, srCmdMap<SBCommandManager>::sr_cmd_callback_fp fp )
{
	return( print_cmd_map.insert( key, fp ) );
}

int SBCommandManager::insert_test_cmd( const char *key, srCmdMap<SBCommandManager>::sr_cmd_callback_fp fp )
{
	return( test_cmd_map.insert( key, fp ) );
}
int SBCommandManager::insert( const char *key, srCmdMap<SBCommandManager>::sr_cmd_callback_fp fp, char* description )
{
//if (cmd_map.is_command(key))
return( cmd_map.insert( key, fp ) );
//else
//	return CMD_SUCCESS;
}

void SBCommandManager::registerCallbacks()
{
	//insert( "sb",			sb_main_func ); EMBER_SB_PYTHON
	insert( "sbm",			sbm_main_func );

	insert( "echo",			mcu_echo_func );
	
	insert( "seq",			mcu_sequence_func );
	insert( "seq-chain",	mcu_sequence_chain_func );
	//insert( "send",			sbm_vhmsg_send_func ); EMBER_SB_VHMSG

	//  cmd prefixes "set" and "print"
	insert( "set",          mcu_set_func );
	insert( "print",        mcu_print_func );
	insert( "test",			mcu_test_func );

	//insert( "terrain",		mcu_terrain_func ); EMBER_SB_HEIGHTFIELD
	insert( "time",			mcu_time_func );
	insert( "tip",			mcu_time_ival_prof_func );

	insert( "panim",		mcu_panim_cmd_func );
	insert( "motionplayer", mcu_motion_player_func);

	insert( "pawn",			pawn_cmd_func );
	insert( "char",			character_cmd_func );

	insert( "ctrl",			mcu_controller_func );
	//insert( "wsp",			mcu_wsp_cmd_func ); EMBER_SB_WSP
	insert( "create_remote_pawn", create_remote_pawn_func );

	insert( "vrAgentBML",   BML::Processor::vrAgentBML_cmd_func );
	insert( "bp",		    BML::Processor::bp_cmd_func );
	insert( "vrSpeak",		BML::Processor::vrSpeak_func );
	insert( "vrExpress",  mcu_vrExpress_func );

	//insert( "receiver",		mcu_joint_datareceiver_func ); EMBER_SB_KINECT
	insert( "net_reset",           mcu_net_reset );
	insert( "net_check",           mcu_net_check );
	insert( "RemoteSpeechCmd"  ,   mcuFestivalRemoteSpeechCmd_func );
	insert( "RemoteSpeechReply",   remoteSpeechResult_func );
	insert( "RemoteSpeechTimeOut", remoteSpeechTimeOut_func);  // internally routed message
//	insert( "locomotion",          locomotion_cmd_func );
//	insert( "loco",                locomotion_cmd_func ); // shorthand
	insert( "check",			   mcu_check_func);		// check matching between .skm and .sk
	//insert( "pythonscript",		   mcu_pythonscript_func); EMBER_SB_PYTHON
	//insert( "python",			   mcu_python_func); EMBER_SB_PYTHON
	//insert( "p",				   mcu_python_func); EMBER_SB_PYTHON
	insert( "bml",				   test_bml_func );
	//insert( "steer",			   mcu_steer_func);	EMBER_SB_STEER
	insert( "RemoteSpeechReplyRecieved", remoteSpeechReady_func);  // TODO: move to test commands
	insert( "syncpoint",		   syncpoint_func);
	//insert( "vhmsgconnect",		   mcu_vhmsg_connect_func); EMBER_SB_VHMSG
	//insert( "vhmsgdisconnect",	   mcu_vhmsg_disconnect_func); EMBER_SB_VHMSG
	//insert( "vhmsglog",			   vhmsglog_func); EMBER_SB_VHMSG

#ifdef USE_GOOGLE_PROFILER
	insert( "startprofile",			   startprofile_func );
	insert( "stopprofile",			   stopprofile_func );
	insert( "startheapprofile",			   startheapprofile_func );
	insert( "stopheapprofile",			   stopheapprofile_func );
#endif
	insert_set_cmd( "bp",             BML::Processor::set_func );
	insert_set_cmd( "pawn",           pawn_set_cmd_funcx );
	insert_set_cmd( "character",      character_set_cmd_func );
	insert_set_cmd( "char",           character_set_cmd_func );
	
	insert_print_cmd( "bp",           BML::Processor::print_func );
	
	insert_test_cmd( "bml",  test_bml_func );
	insert_test_cmd( "fml",  test_fml_func );
	insert_test_cmd( "rhet", remote_speech_test);
	insert_test_cmd( "bone_pos", test_bone_pos_func );
	

	insert( "net",	mcu_net_func );

	insert( "PlaySound", mcu_play_sound_func );
	insert( "StopSound", mcu_stop_sound_func );

	insert( "uscriptexec", mcu_uscriptexec_func );

	insert( "CommAPI", mcu_commapi_func );

	insert( "vrKillComponent", mcu_vrKillComponent_func );
	insert( "vrAllCall", mcu_vrAllCall_func );
	insert( "vrPerception", mcu_vrPerception_func );
	insert( "vrBCFeedback", mcu_vrBCFeedback_func );
	insert( "vrSpeech", mcu_vrSpeech_func );
	insert( "sbmdebugger", mcu_sbmdebugger_func );

	insert( "text_speech", text_speech::text_speech_func ); // [BMLR]

	/*
	// deprecated callbacks
	insert( "help",					deprecatedMessage);
	insert( "reset",				deprecatedMessage );
	insert( "path",					deprecatedMessage );
	insert( "physics",				deprecatedMessage );	
	insert( "mirror",				deprecatedMessage);
	insert( "syncpolicy",          deprecatedMessage );
	insert( "adjustmotion",		   deprecatedMessage);
	insert( "mediapath",		   deprecatedMessage);
	insert( "triggerevent",		   deprecatedMessage );
	insert( "addevent",			   deprecatedMessage );
	insert( "removeevent",		   deprecatedMessage );
	insert( "enableevents",	       deprecatedMessage );
	insert( "disableevents",	   deprecatedMessage );
	insert( "registerevent",       deprecatedMessage );
	insert( "unregisterevent",     deprecatedMessage );
	insert( "setmap",			   deprecatedMessage );
	insert( "motionmap",		   deprecatedMessage );
	insert( "motionmapdir",		   deprecatedMessage );
	insert( "skeletonmap",		   deprecatedMessage );
	insert( "characters",		   deprecatedMessage );
	insert( "pawns",			   deprecatedMessage );
	insert( "resetanimation",	   deprecatedMessage);
	insert( "animation",		   deprecatedMessage);
	insert( "skscale",			   deprecatedMessage);
	insert( "skmscale",			   deprecatedMessage);
	insert( "xmlcachedir",		   deprecatedMessage);
	insert( "xmlcache",			   deprecatedMessage);
	*/
}

int SBCommandManager::execute( const char *key, srArgBuffer& args )
{ 
	std::stringstream strstr;
	strstr << key << " " << args.peek_string();
			

	int ret = ( cmd_map.execute( key, args, this ) );

	return ret; 
}

int SBCommandManager::execute( const char *key, char* strArgs )
{
	std::stringstream strstr;
	strstr << key << " " << strArgs;

    srArgBuffer args( strArgs );
			
	int ret = ( cmd_map.execute( key, args, this ) );

	return ret; 
}

int SBCommandManager::execute( char *cmd )
{ 
	//LOG("execute cmd = %s\n",cmd);
	// check to see if this is a sequence command
	// if so, save the command id
	std::string checkCmd = cmd;
	size_t startpos = checkCmd.find_first_not_of(" \t");
	if( std::string::npos != startpos )
		checkCmd = checkCmd.substr( startpos );

	int ret = ( cmd_map.execute( cmd, this ) ); 

	return ret;
}

int SBCommandManager::execute_seq( srCmdSeq *seq )
{
	std::ostringstream seq_id;
	SmartBody::IntAttribute* intAttr = dynamic_cast<SmartBody::IntAttribute*>(SmartBody::SBScene::getScene()->getAttribute("queuedCommandsIndex"));
	seq_id << "execute_seq-" << (intAttr->getValue());
	intAttr->setValue(intAttr->getValue() + 1);

	return execute_seq( seq, seq_id.str().c_str() );
}

int SBCommandManager::execute_seq( srCmdSeq *seq, const char* seq_name )
{
	
//	printf( "mcuCBHandle::execute_seq: id: '%s'\n", seq_id );
//	seq_p->print();

	if ( !activeSequences.addSequence( seq_name, seq ) ) {
		LOG("ERROR: mcuCBHandle::execute_seq(..): Failed to insert srCmdSeq \"%s\"into active_seq_map.", seq_name );
		return CMD_FAILURE;
	}	

	return CMD_SUCCESS;
}

int SBCommandManager::execute_seq_chain( const std::vector<std::string>& seq_names, const char* error_prefix )
{
	std::vector<std::string>::const_iterator it  = seq_names.begin();
	std::vector<std::string>::const_iterator end = seq_names.end();

	if( it == end ) {
		// No sequences -> NOOP
		return CMD_SUCCESS;
	}

	const std::string& first_seq_name = *it;  // convenience reference
	std::string fullPath;
	FILE* first_file_p = SmartBody::SBScene::getScene()->getAssetManager()->open_sequence_file( first_seq_name.c_str(), fullPath );
	if( first_file_p == NULL ) {
		if( error_prefix )
			LOG("%s Cannot find sequence \"%s\". Aborting seq-chain.", error_prefix, first_seq_name.c_str());
		return CMD_FAILURE;
	}

	srCmdSeq* seq_p = new srCmdSeq();
	int parse_result = seq_p->read_file( first_file_p );
	fclose( first_file_p );
	if( parse_result != CMD_SUCCESS ) {
		if( error_prefix )
			LOG("%s Unable to parse sequence\"%s\".", error_prefix, first_seq_name.c_str());

		delete seq_p;
		seq_p = NULL;

		return CMD_FAILURE;
	}

	// Test remaining sequence names, error early if invalid
	std::vector<std::string>::const_iterator second = ++it;
	for( ; it != end; ++it ) {
		const std::string& next_seq = *it;  // convenience reference

		std::string fullPath;
		FILE* file = SmartBody::SBScene::getScene()->getAssetManager()->open_sequence_file( next_seq.c_str(), fullPath );
		if( file == NULL ) {
			if( error_prefix )
				LOG("%s Cannot find sequence \"%s\". Aborting seq-chain.", error_prefix, next_seq.c_str() );
			return CMD_FAILURE;
		} else {
			fclose( file );
		}
	}

	if( second != end ) {  // has more than one seq_name
		// Append new seq-chian command of remaining seq_names at end of seq_p
		float time = seq_p->duration();

		// Start from second
		it = second;

		// build command
		std::ostringstream oss;
		oss << "seq-chain";
		for( ; it != end; ++it )
			oss << ' ' << (*it);

		// insert command or error with cleanup
		int result = seq_p->insert( time, oss.str().c_str() );
		if( result != CMD_SUCCESS ) {
			if( error_prefix )
				LOG("%s Failed to insert seq-chain command at time %f", error_prefix, time);

			delete seq_p;
			seq_p = NULL;

			return CMD_FAILURE;
		}
	}

	execute_seq( seq_p, first_seq_name.c_str() );

	return CMD_SUCCESS;
}


//  Schedule command in some seconds
int SBCommandManager::execute_later( const char* command, float seconds )
{
	srCmdSeq *temp_seq = new srCmdSeq();
	temp_seq->insert( (float)SmartBody::SBScene::getScene()->getSimulationManager()->getTime() +seconds, command );

	std::ostringstream seqName;
	SmartBody::IntAttribute* intAttr = dynamic_cast<SmartBody::IntAttribute*>(SmartBody::SBScene::getScene()->getAttribute("queuedCommandsIndex"));
	seqName << "execute_later-" << (intAttr->getValue());
	intAttr->setValue(intAttr->getValue() + 1);

	return execute_seq( temp_seq, seqName.str().c_str() );;
}

//  Queue command for next frame
int SBCommandManager::execute_later( const char* command )
{ 
	return( execute_later( command, 0 ) ); 
}


srCmdSeq* SBCommandManager::lookup_seq( const char* name )
{
	int err = CMD_FAILURE;
	
	// Remove previous activation of sequence.
	// Anm: Why?  Need clear distrinction (and definition) between pending and active.
	abortSequence( name );

	srCmdSeq* seq = pendingSequences.getSequence( name );
	if (seq)
	{
		pendingSequences.removeSequence( name, false );
	}
	else
	{
		// Sequence not found.  Load new instance from file.
		std::string fullPath;
		FILE* file = SmartBody::SBScene::getScene()->getAssetManager()->open_sequence_file( name, fullPath );
		if( file ) {
			seq = new srCmdSeq();
			err = seq->read_file( file );
			fclose( file );

			if( err != CMD_SUCCESS ) {
				LOG("ERROR: mcuCBHandle::lookup_seq(..): '%s' PARSE FAILED\n", name ); 

				delete seq;
				seq = NULL;
			}
		} else {
			LOG("ERROR: mcuCBHandle::lookup_seq(..): '%s' NOT FOUND\n", name ); 
		}
	}
	
	return( seq );
}

int SBCommandManager::abortSequence( const char* name ) {
	srCmdSeq* seq = activeSequences.getSequence(name);
	if( !seq )
		return CMD_FAILURE;
	
	bool success = activeSequences.removeSequence( name, true );
	

	srCmdSeq* pending = pendingSequences.getSequence( name );
	if( pending )
		success = pendingSequences.removeSequence( name, true );

	return CMD_SUCCESS; 
}

int SBCommandManager::deleteSequence( const char* name ) {
	int result = abortSequence( name );

	srCmdSeq* seq = pendingSequences.getSequence( name );
	if( seq  )
	{
		pendingSequences.removeSequence( name, true );
		result = CMD_SUCCESS;
	}

	return result;
}

SequenceManager* SBCommandManager::getPendingSequences()
{
	return &pendingSequences;
}

SequenceManager* SBCommandManager::getActiveSequences()
{
	return &activeSequences;
}

int SBCommandManager::mcu_set_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr ) {
    char* arg = args.read_token();
    int result = SmartBody::SBScene::getScene()->getCommandManager()->set_cmd_map.execute( arg, args, SmartBody::SBScene::getScene()->getCommandManager() );
	if( result == CMD_NOT_FOUND ) {
		// TODO: Differentiate between not finding this var and subargs
		LOG("SBM ERR: Unknown Variable, Cannot set: '%s'\n> ", arg );  // Clarify this as a set command error
		return CMD_SUCCESS; // Avoid multiple error messages
	} else {
		return result;
	}
}

/*
	Executes a command to print to the console some debug data.
	See insert_print_cmd and its call in main
*/

int SBCommandManager::mcu_print_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr ) {
    char* arg = args.read_token();
    int result = SmartBody::SBScene::getScene()->getCommandManager()->print_cmd_map.execute( arg, args, SmartBody::SBScene::getScene()->getCommandManager() );
	if( result == CMD_NOT_FOUND ) {
		// TODO: Differentiate between not finding this var and subargs
		LOG("SBM ERR: Print command NOT FOUND: '%s'\n> ", arg );  // Clarify this as a print command error
		return CMD_SUCCESS; // Avoid multiple error messages
	} else {
		return result;
	}
}

/*
	Executes a test sub-command.
	See insert_test_cmd and its call in main
*/

int SBCommandManager::mcu_test_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr ) {
    char* arg = args.read_token();
    int result = SmartBody::SBScene::getScene()->getCommandManager()->test_cmd_map.execute( arg, args, SmartBody::SBScene::getScene()->getCommandManager() );
	if( result == CMD_NOT_FOUND ) {
		LOG("SBM ERR: Test command NOT FOUND: '%s'\n> ", arg );  // Clarify this as a test command error
		return CMD_SUCCESS; // Avoid multiple error messages
	} else {
		return result;
	}
}

}