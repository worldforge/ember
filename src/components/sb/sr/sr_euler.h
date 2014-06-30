/*
 *  sr_euler.h - part of Motion Engine and SmartBody-lib
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
 *  License along with SmarBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Marcelo Kallmann, USC (currently UC Merced)
 */

/** \file sr_euler.h 
 * euler angles tools */

# ifndef SR_EULER_H
# define SR_EULER_H

#include <sb/SBTypes.h>
//================================ Angles from Mat ================================

class SrMat;

/*! Extract from the rotation matrix m the x,y,z Euler angles, 
    assuming they were encoded in m in the given order.
    Parameter order should be: 123 for XYZ, 132 for XZY, etc.
    The other sr_euler_angles* functions are similar, but specific to the desired order. */
SBAPI void sr_euler_angles ( int order, const SrMat& m, float& rx, float& ry, float& rz );

SBAPI void sr_euler_angles_xyz ( const SrMat& m, float& rx, float& ry, float& rz );
SBAPI void sr_euler_angles_xzy ( const SrMat& m, float& rx, float& ry, float& rz );
SBAPI void sr_euler_angles_yxz ( const SrMat& m, float& rx, float& ry, float& rz );
SBAPI void sr_euler_angles_yzx ( const SrMat& m, float& rx, float& ry, float& rz );
SBAPI void sr_euler_angles_zxy ( const SrMat& m, float& rx, float& ry, float& rz );
SBAPI void sr_euler_angles_zyx ( const SrMat& m, float& rx, float& ry, float& rz );

//================================ Mat from Angles ================================
    
/*! Set the rotation matrix m according to the given x,y,z Euler angles.
    Parameter order should be: 123 for XYZ, 132 for XZY, etc.
    Note: the 4th line and column of m are not changed.
    The other sr_euler_mat* functions are similar, but specific to the desired order. */
SBAPI void sr_euler_mat ( int order, SrMat& m, float rx, float ry, float rz );

SBAPI void sr_euler_mat_xyz ( SrMat& m, float rx, float ry, float rz );
SBAPI void sr_euler_mat_xzy ( SrMat& m, float rx, float ry, float rz );
SBAPI void sr_euler_mat_yxz ( SrMat& m, float rx, float ry, float rz );
SBAPI void sr_euler_mat_yzx ( SrMat& m, float rx, float ry, float rz );
SBAPI void sr_euler_mat_zxy ( SrMat& m, float rx, float ry, float rz );
SBAPI void sr_euler_mat_zyx ( SrMat& m, float rx, float ry, float rz );

//============================== end of file ===============================

# endif // SR_EULER_H
