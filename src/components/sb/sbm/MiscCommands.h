#ifndef _MISC_COMMANDS_H
#define _MISC_COMMANDS_H

#include <string>
#include <sbm/sr_arg_buff.h>
#include <sb/SBCommandManager.h>


class SbmPawn;
class SbmCharacter;

int pawn_set_cmd_funcx( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr);
int pawn_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr);
int pawn_parse_pawn_command( SbmPawn* pawn, std::string cmd, srArgBuffer& args);

int character_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr);
int create_remote_pawn_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr);
int character_set_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr);
int character_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr);
int character_parse_character_command( SbmCharacter* character, std::string cmd, srArgBuffer& args, bool all_characters );


int set_world_offset_cmd( SbmPawn* pawn, srArgBuffer& args );


#endif