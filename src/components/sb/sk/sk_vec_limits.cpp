/*
 *  sk_vec_limits.cpp - part of Motion Engine and SmartBody-lib
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
 *      Marcelo Kallmann, USC (currently UC Merced)
 */

# include <sk/sk_vec_limits.h>

//============================= SkVecLimits ============================

void SkVecLimits::init ()
 { 
   int i;
   for ( i=0; i<3; i++ )
    { _value[i] = 0;
      _uplim[i] = 0;
      _lolim[i] = 0;
      _limits[i] = 1;
    }
 }

//float SkVecLimits::get_random ( int d ) const
// {
//   if ( _limits[d] )
//    return _lolim[d] + SrRandom::randf()*(_uplim[d]-_lolim[d]);
//   else
//    return -srpi + SrRandom::randf()*sr2pi;
// }
 
//============================ End of File ============================
