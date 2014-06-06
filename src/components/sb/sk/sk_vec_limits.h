/*
 *  sk_vec_limits.h - part of Motion Engine and SmartBody-lib
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
 *      Andrew n marshall, USC
 */

# ifndef SK_VEC_LIMITS_H
# define SK_VEC_LIMITS_H

# include <sr/sr.h>

/*! SkVecLimits maintains a vector and min/max limits for its three (x,y,z) coordinates. */
class SkVecLimits
 { protected :
    float _value[3]; // the values for X, Y, Z
    float _lolim[3]; // lower limits for X, Y, Z
    float _uplim[3]; // upper limits for X, Y, Z
    bool  _limits[3]; // if limits are active or not
    
   public :
    /*! All parameters 'int d' in the methods below are supposed to
        receive only values 0, 1 or 2. We keep here the original used
        enumerator however we use 'int d' in the methods to save
        lengthy typing when accesing these methods. */
    enum Dof { X=0, Y=1, Z=2 };

   public :
   
    /* Constructor calls init(), which set all values frozen in 0. */
    SkVecLimits () { init(); }

    /* Sets all values and limits to 0. The limits are activated.
       Therefore all values are initialized as frozen in 0. */
    void init ();

    /*! Set values per degree of freedom. In case joint limits are activated,
        values are bounded to the limits before inserted. Returns true if
        a new value was entered, and false otherwise. */
    bool value ( int d, float val )
     { if ( _limits[d] ) { val = SR_BOUND(val,_lolim[d],_uplim[d]); }
       if ( val==_value[d] ) return false;
       _value[d] = val;
       return true;
     }

    /*! Get the value of the X, Y or Z component */
    float value ( int d ) const { return _value[d]; }
    
    /*! Get a const pointer to the array containing the three X, Y and Z values */
    const float* value () const { return _value; }
    
    ///*! Returns a random value inside the joint limits */
    //float get_random ( int d ) const;
    
    /*! Set upper and lower limits and ensures val is inside if limits are active */
    void limits ( int d, float l, float u ) { _lolim[d]=l; _uplim[d]=u; value(d,_value[d]); }
    void lower_limit ( int d, float l ) { _lolim[d]=l; value(d,_value[d]); }
    void upper_limit ( int d, float u ) { _uplim[d]=u; value(d,_value[d]); }

    /*! Get upper and lower limits */
    void limits ( int d, float& l, float& u ) const { l=_lolim[d]; u=_uplim[d]; }
    float lower_limit ( int d ) const { return _lolim[d]; }
    float upper_limit ( int d ) const { return _uplim[d]; }

    /*! Activate/deactivate joint limits control */
    void limits ( int d, bool b )  { _limits[d]=b?1:0; }
    bool limits ( int d ) const { return _limits[d]? true:false; }

    /*! Returns true if the given value respects current limits and false otherwise */
    bool value_in_limits ( int d, float val ) const
     { return _lolim[d]<=val && val<=_uplim[d]? true:false; }

    /*! Freezes all values by activating joint limits and setting
        both lower and upper limits to the current value. */
    void freeze ( int d ) { limits(d,true); limits(d,value(d),value(d)); }

    /*! Returns true if the value d is frozen, i.e if it cannot receive new values.
        If the upper limit is equal to the lower limit, and the limits(d)
        is activated, the joint is considered frozen. */
    bool frozen ( int d ) const
     { if ( !_limits[d] ) return false;
       return _lolim[d]==_uplim[d]? true:false;
     }
 };

//==================================== End of File ===========================================

# endif  // SK_VEC_LIMITS_H
