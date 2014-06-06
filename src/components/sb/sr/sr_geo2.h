/*
 *  sr_geo2.h - part of Motion Engine and SmartBody-lib
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

/** \file sr_geo2.h 
 * 2d geometric primitives */

# ifndef SR_GEO2_H
# define SR_GEO2_H

#include <sb/SBTypes.h>

# define SR_CCW(ax,ay,bx,by,cx,cy)  ((ax*by)-(bx*ay)+(bx*cy)-(cx*by)+(cx*ay)-(ax*cy))

/*! Returns true if segments (p1,p2) and (p3,p4) intersect, and false otherwise. */
SBAPI bool sr_segments_intersect ( double p1x, double p1y, double p2x, double p2y,
                             double p3x, double p3y, double p4x, double p4y );

/*! Returns true if segments (p1,p2) and (p3,p4) intersect, and false otherwise. 
    In case of intersection, p will be the intersection point. */
SBAPI bool sr_segments_intersect ( double p1x, double p1y, double p2x, double p2y,
                             double p3x, double p3y, double p4x, double p4y,
                             double& x, double &y );

/*! Returns true if lines (p1,p2) and (p3,p4) intersect, and false otherwise. */
SBAPI bool sr_lines_intersect ( double p1x, double p1y, double p2x, double p2y,
                          double p3x, double p3y, double p4x, double p4y );

/*! Returns true if lines (p1,p2) and (p3,p4) intersect, and false otherwise. 
    In case of intersection, p will be the intersection point. */
SBAPI bool sr_lines_intersect ( double p1x, double p1y, double p2x, double p2y,
                          double p3x, double p3y, double p4x, double p4y,
                          double& x, double &y );

/*! Orthogonal projection of p in the line (p1,p2). The projected point becomes q. */
SBAPI void sr_line_projection ( double p1x, double p1y, double p2x, double p2y,
                          double px, double py,
                          double& qx, double& qy );

/*! Returns true if the orthogonal projection of p is inside (within epsilon distance)
    the segment (p1,p2). In such a case, the projected point becomes q, otherwise
    false is returned. */
SBAPI bool sr_segment_projection ( double p1x, double p1y, double p2x, double p2y,
                             double px, double py,
                             double& qx, double& qy, double epsilon );

/*! Returns the square of the distance between p1 and p2 */
SBAPI double sr_dist2 ( double p1x, double p1y, double p2x, double p2y );

/*! Returns the minimum distance between p and segment (p1,p2) */
SBAPI double sr_point_segment_dist ( double px, double py, 
                               double p1x, double p1y, double p2x, double p2y );

/*! Returns the minimum distance between p and line (p1,p2) */
SBAPI double sr_point_line_dist ( double px, double py, double p1x, double p1y, double p2x, double p2y );

/*! Returns true if the distance between p1 and p2 is smaller (or equal) than epsilon */
SBAPI bool sr_next ( double p1x, double p1y, double p2x, double p2y, double epsilon );

/*! Returns >0 if the three points are in counter-clockwise order, <0 if
    the order is clockwise and 0 if points are collinear. */
SBAPI double sr_ccw ( double p1x, double p1y, double p2x, double p2y, double p3x, double p3y );

/*! Returns true if p is in the segment (p1,p2), within precision epsilon, and false
    otherwise. More precisely, true is returned if the projection of p in the segment (p1,p2)
    has a distance to p less or equal than epsilon. */
SBAPI bool sr_in_segment ( double p1x, double p1y, double p2x, double p2y,
                     double px, double py, double epsilon );

/*! Returns true if p is in the segment (p1,p2), within precision epsilon, and false
    otherwise. More precisely, true is returned if the projection of p in the segment
    p1,p2) has a distance to p less or equal than epsilon. Parameter dist2 contains
    the square of that distance. */
SBAPI bool sr_in_segment ( double p1x, double p1y, double p2x, double p2y, double px, double py,
                     double epsilon, double& dist2 );

/*! Returns true if p is inside (or in the border) of triangle (p1,p2,p3), otherwise false
    is returned. The test is based on 3 CCW>=0 tests and no epsilons are used. */
SBAPI bool sr_in_triangle ( double p1x, double p1y, double p2x, double p2y, double p3x, double p3y,
                      double px, double py );

/*! Returns true if p is inside the circle passing at p1, p2, and p3, otherwise false
    is returned. Points p1, p2 and p3 must be in ccw orientation.
    This is a fast 4x4 determinant evaluation that will return true only
    if p is strictly inside the circle, testing if determinant>1.0E-14 */
SBAPI bool sr_in_circle ( double p1x, double p1y, double p2x, double p2y, double p3x, double p3y,
                    double px, double py );

/*! Returns (u,v,w), w==1-u-v, u+v+w==1, such that p1*u + p2*v + p3w == p */
SBAPI void sr_barycentric ( double p1x, double p1y, double p2x, double p2y, double p3x, double p3y,
                      double px, double py, double& u, double& v, double& w );

//============================== end of file ===============================

# endif // SR_GEO2_H

