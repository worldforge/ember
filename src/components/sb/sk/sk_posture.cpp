/*
 *  sk_posture.cpp - part of Motion Engine and SmartBody-lib
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
 *      Ed Fast, USC
 */
 
# include <sk/sk_skeleton.h>
# include <sk/sk_posture.h>
# include <sr/sr_vec.h>


//=========================== SkPostureDfJoints ===============================

int SkPostureDfJoints::input ( SrInput& in, SkSkeleton* sk )
 {
   int i, n;
   in >> n;
   joints.size(n); // to reserve enought space
   joints.size(0);
   for ( i=0; i<n; i++ )
    { in.get_token();
      joints.push() = sk->search_joint(in.last_token());
      if ( !joints.top() ) joints.pop();
    }
   joints.compress();
   return joints.size();
 }

int SkPostureDfJoints::search ( const char* jname )
 {
   int i;
   for ( i=0; i<joints.size(); i++ )
    {
		if ( sr_compare(joints[i]->jointName().c_str(),jname)==0 ) return i;
    }
   return -1;
 }

//============================= SkPosture ============================

 SkPosture::SkPosture () :
	_channels( NULL ),
	_dfjoints( NULL ),
	_syncpoints( false ),
	_name( NULL ),
	_filename( NULL )
{}

SkPosture::SkPosture ( const SkPosture& p )
          : values ( p.values ),
            points ( p.points )
 {
   _channels = 0;
   _dfjoints = 0;
   if ( p._channels ) { _channels=p._channels; _channels->ref(); }
   if ( p._dfjoints ) { _dfjoints=p._dfjoints; _dfjoints->ref(); }
      
   _syncpoints = p._syncpoints;
   _name = 0;
   name ( p.name() );
 }

SkPosture::SkPosture ( SkChannelArray* ca, SkPostureDfJoints* dfj )
 {
   _dfjoints = 0;
   _syncpoints = false;
   _name = 0;

   _channels = ca;
   _channels->ref();
   values.size ( _channels->floats() );
   
   if ( dfj ) dfjoints(dfj);
 }

SkPosture::SkPosture ( SkSkeleton* s )
 {
   _dfjoints = 0;
   _syncpoints = false;
   _name = NULL;
   _filename = NULL;

   _channels = &s->channels();
   _channels->ref();
   values.size ( _channels->floats() );
 }

SkPosture::~SkPosture ()
 {
   init ();
   delete [] _name;
   _name = NULL;
   delete [] _filename;
   _filename = NULL;
 }

void SkPosture::init ()
 {
   if ( _channels ) { _channels->unref(); _channels=0; }
   if ( _dfjoints ) { _dfjoints->unref(); _dfjoints=0; }
   _syncpoints = false;
   values.size(0);
   points.size(0);
 }

void SkPosture::init ( SkChannelArray* ca )
 {
   init ();
   _channels = ca;
   _channels->ref();
   values.size ( _channels->floats() );
 }

void SkPosture::init ( SkChannelArray* ca, SkPostureDfJoints* ja )
 {
   init ( ca );
   dfjoints ( ja );
 }

void SkPosture::init ( SkSkeleton* s )
 {
   init ( &s->channels() );
 }
 
void SkPosture::name ( const char* n )
 {
   sr_string_set ( _name, n );
 }

void SkPosture::get ()
 {
   _syncpoints = false;
   _channels->get_values ( &values[0] );
 }

//void SkPosture::random ()
// {
//   _syncpoints = false;
//   _channels->get_random_values ( &values[0] );
// }

void SkPosture::dfjoints ( SkPostureDfJoints* dfjoints )
 {
   if ( _dfjoints ) _dfjoints->unref();
   _dfjoints = dfjoints;
   _dfjoints->ref();
   points.size ( _dfjoints->joints.size() );
   _syncpoints = false;
 }

void SkPosture::make_points ( SkJoint* j )
 {
   // apply this configuration to skeleton :
   apply ();

   // update matrices:
   if ( !j )
    skeleton()->update_global_matrices();
   else
    j->update_gmat();

   // get points:
   int i;
   SrPnt p;
   if ( _dfjoints )
    { int size = _dfjoints->joints.size();
      if ( points.size()!=size ) points.size ( size );
      for ( i=0; i<size; i++ ) points[i] = p * _dfjoints->joints[i]->gmat();
    }
   else
    { int size = _channels->size();
      if ( points.size()!=size ) points.size ( size );
      for ( i=0; i<size; i++ ) points[i] = p * _channels->get(i).joint->gmat();
    }

   _syncpoints = true;
 }

void SkPosture::operator= ( const SkPosture& p )
 {
   if ( &p==this ) return;

   _syncpoints = p._syncpoints;

   if ( _channels!=p._channels )
    { if ( _channels ) _channels->unref();
      _channels = p._channels;
      if ( _channels ) _channels->ref();
    }

   if ( _dfjoints!=p._dfjoints )
    { if ( _dfjoints ) _dfjoints->unref();
      _dfjoints = p._dfjoints;
      if ( _dfjoints ) _dfjoints->ref();
    }

   values = p.values;
   points = p.points;
 }

//============================= friends ============================

void interp ( const SkPosture& p1, const SkPosture& p2, float t, SkPosture& p )
 {
   //sr_out<<"SIZES: "<< p1.values.size()<<srspc<<p2.values.size()<<srspc<<p.values.size()<<srnl;

   SkChannelArray& ch = *p._channels;
   const float* fp1 = &p1.values[0];
   const float* fp2 = &p2.values[0];
   float* fp = &p.values[0];
   int chsize=ch.size();
   int i, dp;

   for ( i=0; i<chsize; i++ )
    { dp = ch[i].interp ( fp, fp1, fp2, t );
      fp1 += dp;
      fp2 += dp;
      fp += dp;
    }

   p._syncpoints = false;
 }

float dist ( SkPosture& p1, SkPosture& p2 )
 {
   int i, size;

   if ( !p1._syncpoints ) p1.make_points();
   if ( !p2._syncpoints ) p2.make_points();

   size = p1.points.size();
   if ( size>p2.points.size() ) size=p2.points.size();

   //sr_out<<"SIZES: "<< p1.values.size()<<srspc<<p2.values.size()<<srnl;
   
   // Max norm :
   float dist2, d=0;
   for ( i=0; i<size; i++ )
    { 
#if 0
		dist2 = ::dist2 ( p1.points[i], p2.points[i] );
#else
		float dx = p1.points[i].x - p2.points[i].x;
		float dy = p1.points[i].y - p2.points[i].y;
		float dz = p1.points[i].z - p2.points[i].z;
		dist2 = dx*dx + dy*dy + dz*dz;
#endif
		if ( dist2>d ) d=dist2;
    }
   return sqrtf(d);
   
   // average Euclidian distance:
/*   double d=0;
   for ( i=0; i<size; i++ )
    { d += (double) ::dist ( p1.points[i], p2.points[i] );
    }
   d /= double(size);
   return float(d);*/
 }

void SkPosture::output ( SrOutput& out, bool channels, bool values ) const
 {
   int i;

   if ( channels && _channels )
    { out << (*_channels);
    }
    
   if ( _name ) if ( _name[0] )
    { SrString s;
      s.make_valid_string(_name);
      out << "name " << s << srspc;
    }

   if ( values && _channels )
    { const float* fp = &SkPosture::values[0];
      int chsize = _channels->size();
      for ( i=0; i<chsize; i++ )
       { fp += _channels->const_get(i).save(out,fp);
         if ( i<chsize-1 ) out<<srspc;
       }
    }

   out << srnl;
 }

SrOutput& operator<< ( SrOutput& out, const SkPosture& p )
 {
   p.output ( out, true, true );
   return out;
 }

SrInput& operator>> ( SrInput& inp, SkPosture& p )
 {
   p.name ( 0 );

   inp.get_token();

   if ( inp.last_token()=="add_posture" )
	   inp.get_token();  // Ignore and get next
   if ( inp.last_token()=="channels" )
    { if ( p._channels ) p._channels->unref();
      p._channels = new SkChannelArray;
      p._channels->ref();
      inp.unget_token();
      inp >> *p._channels;
      inp.get_token();
    }

   if ( inp.last_token()=="name" )
    { inp.get_token ();
      p.name ( inp.last_token() );
    }
   else inp.unget_token();

   if ( !p._channels ) return inp;

   SkChannelArray& ch = *p._channels;
   p.values.size ( ch.floats() );
   float* fp = &(p.values[0]);
   int i, chsize = ch.size();

   for ( i=0; i<chsize; i++ )
    { fp += ch[i].load(inp,fp);
    }

   p._syncpoints = false; // because we do not save/read the points

   return inp;
 }

//================================ End of File =================================================

