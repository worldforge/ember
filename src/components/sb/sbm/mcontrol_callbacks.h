/*
 *  mcontrol_callbacks.h - part of SmartBody-lib
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
 */

#ifndef MCONTROL_CALLBACKS_H
#define MCONTROL_CALLBACKS_H

#include "components/sb/sb/SBTypes.h"
#include "sr_arg_buff.h"
#include "components/sb/sb/SBCommandManager.h"


int mcu_sequence_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int mcu_sequence_chain_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
#ifdef EMBER_SB_HEIGHTFIELD
int mcu_terrain_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
#endif
int mcu_time_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int mcu_time_ival_prof_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

// panimation commands
int mcu_panim_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int mcu_panim_test_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int mcu_motion_player_func(srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int mcu_panim_schedule_func(std::string motion1, std::string charName, bool loop, SmartBody::SBCommandManager* cmdMgr);
int mcu_panim_schedule_func(std::string motion1, std::string motion2, std::string characterName, float weight, bool loop, SmartBody::SBCommandManager* cmdMgr);

//int mcu_character_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );  // Old version... See SbmCharacter::character_cmd_func

int mcu_character_breathing(const char* char_name, srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr);

int mcu_controller_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

int mcu_net_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int mcu_net_reset( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int mcu_net_check( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

int mcu_play_sound_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int mcu_stop_sound_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

int mcu_uscriptexec_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

int mcu_commapi_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

int mcu_vrKillComponent_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
SBAPI int mcu_vrAllCall_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int mcu_vrQuery_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr);

//perception
int mcu_vrPerception_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

// listening feedback
int mcu_vrBCFeedback_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

//speech
int mcu_vrSpeech_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

int mcu_sbmdebugger_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

#ifdef EMBER_SB_WSP
int mcu_wsp_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
#endif

// examine the motion for specific skeleton
int mcu_check_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

#ifdef EMBER_SB_VHMSG
int mcu_vhmsg_connect_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int mcu_vhmsg_disconnect_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
#endif

#ifdef EMBER_SB_PYTHON
int mcu_python_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr  );
int mcu_pythonscript_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr  );
#endif

#ifdef EMBER_SB_STEER
int mcu_steer_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
#endif

int syncpoint_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
#ifdef USE_GOOGLE_PROFILER
int startprofile_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int stopprofile_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int startheapprofile_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int stopheapprofile_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
#endif

#ifdef EMBER_SB_KINECT
int mcu_joint_datareceiver_func(srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr);
#endif
int mcu_vrExpress_func(srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr);
#ifdef EMBER_SB_VHMSG
int vhmsglog_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
#endif

int xmlcachedir_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );
int xmlcache_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

void mcu_vhmsg_callback( const char *op, const char *args, void * user_data );
int mcuFestivalRemoteSpeechCmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr);

int mcu_echo_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr  );
#ifdef EMBER_SB_PYTHON
int sb_main_func( srArgBuffer & args, SmartBody::SBCommandManager* cmdMgr  );
#endif
int sbm_main_func( srArgBuffer & args, SmartBody::SBCommandManager* cmdMgr  );
#ifdef EMBER_SB_VHMSG
int sbm_vhmsg_send_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr  );
#endif
int deprecatedMessage( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr  );

//////////////////////////////////////////////////////////////////
#endif
