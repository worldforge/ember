/*
 *  sbm_test_cmds.hpp - part of SmartBody-lib
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

#ifndef SBM_TEST_CMDS_HPP
#define SBM_TEST_CMDS_HPP

#include <sb/SBCommandManager.h>
#include "sr_arg_buff.h"


/**
 *  Handles the "test bml ..." commands.
 *  Try "test bml help".
 */
int test_bml_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

/**
 *  Handles the "test fml ..." commands.
 *  Try "test fml help".
 */
int test_fml_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );


/**
 *  This function is used to test the face, but can be used to test 
 *  any joints with position parameterizion, to use at the SBM command
 *  line put:
 *    test bone_pos <joint name> <x pos> <y pos> <z pos>
 */
int test_bone_pos_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );


#endif // SBM_TEST_CMDS_HPP