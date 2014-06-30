/*
 *  gwiz_spline.cpp - part of SmartBody-lib
 *  Copyright (C) 2001-2008  Marcus Thiebaux, University of Southern California
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
 *      Marcus Thiebaux, USC
 */

#include "sbm/gwiz_math.h"

using namespace gwiz;

////////////////////////////////

bool gwiz::ctrl_key::bound_box( 
	float_t p_comp, 
	float_t v_comp, 
	float_t radius 
) const {
	return(
		( fabs( param - p_comp ) < radius )&&( fabs( value - v_comp ) < radius )
	);
}

////////////////////////////////

void gwiz::tempordinal_key::simple( 
	const ctrl_key& k0, 
	const ctrl_key& k1, 
	const ctrl_key& k2 
)	{

	// SAFETY: if( ( k2.p() - k0.p() ) < epsilon8() )...

	float_t m = ( k2.v() - k0.v() ) / ( k2.p() - k0.p() );
	set( k1.p(), k1.v(), m, m, 1.0, 1.0 );
}

void gwiz::tempordinal_key::cardinal(
	float_t c,
	const ctrl_key& k0,
	const ctrl_key& k1,
	const ctrl_key& k2
)	{

	// SAFETY: if( ( k2.p() - k0.p() ) < epsilon8() )...

	float_t m = ( 1.0 - c ) * ( k2.v() - k0.v() ) / ( k2.p() - k0.p() );

	set( k1.p(), k1.v(), m, m, k1.p() - k0.p(), k2.p() - k1.p() );

#if 0
	// clamping m experiment:
	float_t lim = 4.0;
	float_t local_lim = lim - k1.v();
	float py = ml() * dl();
	if( py > local_lim )	{
	
	}
	
	float_t tan_dif = local_lim - ml() * dl();
	if( tan_dif > 0.0 ) {

		m = tan_dif / local_lim;
//		m = ;
	}
	else	{
		tan_dif = local_lim - mr() * dr();
		if( tan_dif > 0.0 ) {
		
			m = tan_dif / local_lim;
		}
	}

/*
	float_t tan_dif = k1.v() + ml() * dl();
	float_t v_dif = v_lim - k1.v();
	if( tan_dif > v_lim )	{

		m = tan_dif / v_dif;
	}
	else	{
		tan_dif = k1.v() + mr() * dr();
		if( tan_dif > v_lim )	{

			m = tan_dif / v_dif;
		}
	}
*/
	set( k1.p(), k1.v(), m, m, k1.p() - k0.p(), k2.p() - k1.p() );
#endif
}

void gwiz::tempordinal_key::cardinal_alt(
	float_t c,
	const ctrl_key& k0,
	const ctrl_key& k1,
	const ctrl_key& k2
)	{

	// SAFETY:
	//	if( ( k2.p() - k1.p() ) < epsilon8() )...
	//	if( ( k2.p() - k0.p() ) < epsilon8() )...

	float_t m = 0.5 * (
		( k2.v() - k1.v() ) / ( k2.p() - k1.p() ) +
		( k1.v() - k0.v() ) / ( k1.p() - k0.p() )
	);
	set( k1.p(), k1.v(), m, m, k1.p() - k0.p(), k2.p() - k1.p() );
}

void gwiz::tempordinal_key::kochbartels(
	float_t tension,
	float_t bias,
	float_t continuity,
	const ctrl_key& k0,
	const ctrl_key& k1,
	const ctrl_key& k2
)	{
	float_t dv0 = k1.v() - k0.v();
	float_t dv1 = k2.v() - k1.v();
	float_t m0 = 
		0.5 * ( 1.0 - tension ) * ( 1.0 + bias ) * ( 1.0 + continuity ) * dv0 +
		0.5 * ( 1.0 - tension ) * ( 1.0 - bias ) * ( 1.0 - continuity ) * dv1;
	float_t m1 = 
		0.5 * ( 1.0 - tension ) * ( 1.0 + bias ) * ( 1.0 - continuity ) * dv0 +
		0.5 * ( 1.0 - tension ) * ( 1.0 - bias ) * ( 1.0 + continuity ) * dv1;
	set( k1.p(), k1.v(), m0, m1, k1.p() - k0.p(), k2.p() - k1.p() );
}

////////////////////////////////

gwiz::float_t gwiz::ssvvcc_extend( float_t f0, float_t f1, float_t f2 )	{
	return( f2 + 2.0 * ( f2 - f1 ) + ( f0 - f1 ) );
}

gwiz::ctrl_key gwiz::ssvvcc_extend( 
	float_t t0, float_t f0, 
	float_t t1, float_t f1, 
	float_t t2, float_t f2 
	)	{
	
	// SAFETY: if( ( t1 - t0 ) < epsilon8() )...

	return( 
		ctrl_key( 
			t2 + ( t2 - t1 ), 
			f2 + 
			2.0 * ( f2 - f1 ) + 
			( f0 - f1 ) * ( 
				( t2 - t1 )/( t1 - t0 ) 
			)
		)
	);
}

gwiz::ctrl_key gwiz::ssvvcc_extend( 
	const ctrl_key& k0, const ctrl_key& k1, const ctrl_key& k2 
	)	{

	return( 
		ssvvcc_extend(
			k0.p(), k0.v(),
			k1.p(), k1.v(),
			k2.p(), k2.v()
		)
	);
}

////////////////////////////////

gwiz::float_t gwiz::bezier(
	float_t s, // unit interpolant
	float_t f0,
	float_t f1,
	float_t f2,
	float_t f3
)	{
	// de Casteljau linear recursion
	float_t A = f0 + s * ( f1 - f0 );
	float_t B = f1 + s * ( f2 - f1 );
	float_t C = A + s * ( B - A );
	return( C + s*( ( B + s*( ( f2 + s*( f3 - f2 ) ) - B ) ) - C ) );
}

gwiz::float_t gwiz::hermite(
	float_t s,
	float_t v1,
	float_t v2,
	float_t m1,
	float_t m2
) {
#if 1
		return(
			bezier(
				s,
				v1,
				v1 + m1 * 0.333333333,
				v2 - m2 * 0.333333333,
				v2
			)
		);
#elif 0
	// equivalents...
		register float_t s_2 = s * s;
		register float_t s_3 = s_2 * s;
		return(
			v1 * ( 2.0 * s_3 - 3.0 * s_2 + 1.0 ) +
			m1 * ( s_3 - 2.0 * s_2 + s ) +
			v2 * ( -2.0 * s_3 + 3.0 * s_2 ) +
			m2 * ( s_3 - s_2 );
		);
#else
		return(
			s * (
				s * (
					s * ( 2.0 * v1 - 2.0 * v2 + m1 + m2 ) +
					( -3.0 * v1 + 3.0 * v2 - 2.0 * m1 - m2 )
				) + m1
			) + v1
		);
#endif
}

gwiz::float_t gwiz::hermite(
	float_t t, 
	const cardinal_key& K1, 
	const cardinal_key& K2 
)	{

	float_t dp = K2.p() - K1.p();
	if( dp < 0.0 ) { return( K1.v() ); }
	if( dp < epsilon8() ) { return( K1.v() ); }

	float_t s = ( t - K1.p() ) / dp;
	return(
		hermite( s, K1.v(), K2.v(), K1.mr(), K2.ml() )
	);
}

gwiz::float_t gwiz::hermite(
	float_t t, 
	const tempordinal_key& K1, 
	const tempordinal_key& K2 
)	{

	// SAFETY: 
	float_t dp = K2.p() - K1.p();
	if( dp < 0.0 ) { return( K1.v() ); }
	if( dp < epsilon8() ) { return( K1.v() ); }
//	if( t < K1.t )
//	if( t >= K2.t )

	float_t s = ( t - K1.p() ) / dp; // normalize parametric interpolant

	// FaceFX algorithm from 
	//	http://www.facefx.com/documentation/2010/W99
	float_t m1 = K1.mr() * K1.dr();
	float_t m2 = K2.ml() * K2.dl();

//	double h = hermite( s, K1.v(), K2.v(), m1, m2 );
//if( s >= 1.0 ) printf( "herm: s:%.12f\n", s );
//if( h < 1.001 ) printf( "herm: h:%.12f\n", h );

	return(
//		h
		hermite( s, K1.v(), K2.v(), m1, m2 )
	);
}

////////////////////////////////

