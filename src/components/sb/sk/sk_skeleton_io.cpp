/*
*  sk_skeleton_io.cpp - part of Motion Engine and SmartBody-lib
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
*      Ed Fast, USC
*      Andrew n marshall, USC
*		Jingqiao Fu, USC
*/

# include <sr/sr_model.h>
# include <sr/sr_path_array.h>


# include <sk/sk_skeleton.h>
# include <sk/sk_posture.h>
#include <sb/SBJoint.h>

//============================ load_skeleton ============================

enum KeyWord { ROOT, OFFSET, CENTER, CHANNELS, CHANNEL, JOINT, END, 
VISGEO, COLGEO, MODELMAT, MODELROT, PREROT, POSTROT,
ALIGN, TYPE, EULER, NOKEYWORD }; 

static KeyWord read_keyword ( SrInput& in )
{
	KeyWord kw = NOKEYWORD;

	if ( in.get_token()!=SrInput::Name ) return kw;

	SrString& s = in.last_token();

	if ( s=="root" ) kw=ROOT; // root declaration
	else 
		if ( s=="offset" ) kw=OFFSET; // offset to position joints
		else
			if ( s=="center" ) kw=CENTER; // same as offset, more natural for joint list mode
			else
				if ( s=="channels" ) kw=CHANNELS; // bvh channels definition
				else
					if ( s=="channel" ) kw=CHANNEL; // bvh channels definition
					else
						if ( s=="joint" ) kw=JOINT; // joint declaration
						else
							if ( s=="modelmat" ) kw=MODELMAT; // matrix to adjust geometries
							else
								if ( s=="modelrot" ) kw=MODELROT; // matrix to adjust geometries
								else
									if ( s=="prerot" || s=="framerot" ) kw=PREROT; // to pre-multiply joint rotations
									else
										if ( s=="postrot" ) kw=POSTROT; // to post-multiply joint rotations
										else
											if ( s=="align" ) kw=ALIGN; // adjusts pre/post to have 1st child with given offset
											else
												if ( s=="visgeo" ) kw=VISGEO; // visualization geometry
												else
													if ( s=="colgeo" ) kw=COLGEO; // collision geometry
													else
														if ( s=="euler" ) kw=EULER; // euler order/type specification
														else
															if ( s=="end" ) kw=END; // default string comparison is case-insensitive

	return kw;
}

// used in bvh format
static bool read_channels ( SrInput& in, SkJoint* j, SrArray<SkChannel::Type>& channels )
{
	int c;
	int n = in.getn().atoi();

	channels.size(0);

	if ( in.last_error()==SrInput::UnexpectedToken ) return false;

	j->rot_type ( SkJoint::TypeEuler );
	j->euler()->type ( SkJointEuler::TypeYXZ );

	while ( n-->0 )
	{ if ( in.get_token()!=SrInput::Name ) return false;
	SrString& s = in.last_token();
	channels.push() = SkChannel::get_type(s);
	c = channels.top();
	if ( c<=SkChannel::ZPos )
	{ j->pos()->limits(c,false); } // turn off dof limits
	else if ( c<=SkChannel::ZRot )
	{ j->euler()->limits(c-3,false); // turn off dof limits
	j->euler()->activate();
	}
	else
	{ j->quat()->activate(); }
	}         
	return true;
}

// only in sk format:
//  XPos <val> [free | <min><max> | lim <min><max>]
//  Quat [axis <x> <y> <z> ang <degrees>] [frozen]
//  Swing [axis <x> <y> ang <degrees>] [lim <xradius> <yradius>]
//  Twist <val> [free | <min><max> | lim <min><max>]
static bool read_channel ( SrInput& in, SkJoint* j )
{
	in.get_token();
	SkChannel::Type c =  SkChannel::get_type ( in.last_token() );

	if ( c==SkChannel::Quat )
	{ j->rot_type ( SkJoint::TypeQuat );
	j->quat()->activate();
	in.get_token();
	SrString s = in.last_token();
	in.unget_token();
	if ( s=="axis" )
	{ SrQuat q; in>>q; //j->quat()->prerot(q);
	in.get_token();
	if ( in.last_token()=="frozen" )
		j->quat()->deactivate();
	else
		in.unget_token();
	}
	return true;
	}

	if ( c==SkChannel::Swing )
	{ j->rot_type ( SkJoint::TypeSwingTwist );
	j->st()->activate();
	in.get_token();
	if ( in.last_token()=="axis" )
	{ SrVec2 axis;
	float ang;
	in >> axis;
	in.get_token(); // "ang"
	in >> ang;
	axis.len(SR_TORAD(ang));
	j->st()->swing ( axis.x, axis.y );
	in.get_token();
	}
	if ( in.last_token()=="lim" )
	{ float rx, ry;
	in >> rx >> ry;
	j->st()->ellipse ( SR_TORAD(rx), SR_TORAD(ry) );
	} 
	else // not a parameter
	{ in.unget_token(); }
	return true;
	}

	if ( c==SkChannel::Twist )
	{ j->rot_type ( SkJoint::TypeSwingTwist );
	float tw;
	in >> tw;
	j->st()->twist ( tw );
	in.get_token();
	if ( in.last_token()=="free" )
	{ j->st()->twist_limits(false); }
	else
	{ if ( in.last_token()!="lim" ) in.unget_token();
	float min, max;
	in >> min >> max;
	j->st()->twist_limits ( SR_TORAD(min), SR_TORAD(max) );
	} 
	return true;
	}

	// at this point the final option is PosX/Y/Z or RotX/Y/Z

	float lo=0, up=0, val=0;
	bool free;

	in >> val; // first num is the value

	in.get_token();
	if ( in.last_token()=="free" )
	{ free = true;
	}
	else
	{ if ( in.last_token()!="lim" ) in.unget_token();
	in >> lo >> up;
	if ( in.last_error()==SrInput::UnexpectedToken ) return false;
	free = false;
	}

	if ( c<=2 ) // 0:XPos, 1:YPos, or 2:ZPos
	{ if ( free )
	{ j->pos()->limits ( c, false ); }
	else
	{ j->pos()->limits ( c, true );
	j->pos()->limits ( c, lo, up );
	}
	j->pos()->value ( c, val );
	return true;
	}

	if ( c<=5 ) // 3:XRot, 4:YRot, 5:ZRot
	{ j->rot_type ( SkJoint::TypeEuler );
	j->euler()->activate();
	int d = c-3;
	if ( free )
	{ j->euler()->limits ( d, false ); }
	else
	{ j->euler()->limits ( d, true );
	j->euler()->limits ( d, SR_TORAD(lo), SR_TORAD(up) );
	}
	j->euler()->value ( d, SR_TORAD(val) );

	return true;
	}

	return false; // should not reach here
}

static SrModel* read_model ( SrInput& in, SrPathArray& paths, SrMat* mat )
{
	if ( in.get_token()!=SrInput::String ) return 0;
	SrString file = in.last_token();

	int i=0;
	SrInput fi;

	if ( !paths.open(fi,file) )
	{ //SR_TRACE2 ( "Could not read model." );
		return 0; // file not found
	}

	SrModel* m = new SrModel;

	bool ok=false;
	SrString ext;
	file.get_file_extension ( ext );

	if ( ext=="srm" )
	{ //SR_TRACE2 ( "Loading srm geometry..." );
		ok = m->load(fi);
	}
	else if ( ext=="obj" )
	{ //SR_TRACE2 ( "Importing obj geometry..." );
		SrString filename = fi.filename();
		fi.close ();
		ok = m->import_obj(filename);
	}

	if ( ok )
	{ //SR_TRACE2 ( "Loaded model with "<<m->F.size()<<" triangles" );
		if ( mat ) m->apply_transformation ( *mat );
		//m->ref();
	}
	else
	{ //SR_TRACE2 ( "Loading Error!" );
		delete m; m=0;
	}

	return m;
}

static void _setmodel ( SrModel*& curm, SrModel* newm )
{
	if ( !newm ) return;
	if ( curm )
	{ curm->add_model ( *newm );
	delete newm;
	}
	else
	{ curm = newm;
	curm->ref();
	}
}

// Load joint data and returns -1 if } is reached, or kw JOINT, END or NOKEYWORD
int SkSkeleton::_loadjdata ( SrInput& in, SkJoint* j, SrStringArray& paths )
{
	KeyWord kw;
	bool hasmat=false;
	SrMat curmat;
	SrArray<SkChannel::Type> channels;

	while (true) // get next items
	{ 
		kw = read_keyword(in);
		//SR_TRACE1 ( in.last_token() );

		if ( kw==OFFSET || kw==CENTER )
		{
			in >> j->_offset;
		}
		else if ( kw==CHANNELS ) // only in bvh format
		{
			if ( !read_channels(in,j,channels) ) return 0;
		}
		else if ( kw==EULER )
		{ 
			j->rot_type ( SkJoint::TypeEuler );
			in.get_token();
			if ( in.last_token()=="XYZ" )
				j->euler()->type ( SkJointEuler::TypeXYZ );
			else if ( in.last_token()=="YXZ" )
				j->euler()->type ( SkJointEuler::TypeYXZ );
			else if ( in.last_token()=="ZY" )
				j->euler()->type ( SkJointEuler::TypeZY );
		}
		else if ( kw==CHANNEL ) // only in sk format
		{
			if ( !read_channel(in,j) ) return 0;
		}
		else if ( kw==MODELMAT )
		{
			in >> curmat;
			hasmat = true;
			if ( j->_visgeo ) j->_visgeo->apply_transformation ( curmat );
			if ( j->_colgeo ) j->_colgeo->apply_transformation ( curmat );
		}
		else if ( kw==MODELROT )
		{
			SrQuat q;
			in >> q;
			q.get_mat ( curmat );
			hasmat = true;
			if ( j->_visgeo ) j->_visgeo->apply_transformation ( curmat );
			if ( j->_colgeo ) j->_colgeo->apply_transformation ( curmat );
		}
		else if ( kw==PREROT )
		{
			SrQuat q;
			in >> q;
			j->quat()->prerot ( j->quat()->prerot()*q );
		}
		else if ( kw==POSTROT )
		{
			SrQuat q;
			in >> q;
			j->quat()->postrot ( j->quat()->postrot()*q );
		}
		else if ( kw==ALIGN )
		{
			SrVec v;
			SrString type;
			in >> type >> v;
			j->quat()->align ( type=="pre"? SkJointQuat::AlignPre :
				type=="post"? SkJointQuat::AlignPost :
				type=="prepost"? SkJointQuat::AlignPrePost :
				type=="preinv"? SkJointQuat::AlignPreInv :
				SkJointQuat::AlignPostInv, v );
		}
		else if ( kw==VISGEO ) // keyword only in hm format
		{
			_setmodel ( j->_visgeo, read_model ( in, (SrPathArray&)paths, hasmat? &curmat:0 ) );
		}
		else if ( kw==COLGEO ) // keyword only in hm format
		{
			_setmodel ( j->_colgeo, read_model ( in, (SrPathArray&)paths, hasmat? &curmat:0 ) );
		}
		else if ( kw==JOINT || kw==END )
		{
			return kw;
		}
		else if ( in.last_token()[0]=='}' ) return -1;
	}
}

// recursivelly load joint definitions
SkJoint* SkSkeleton::_loadj ( SrInput& in, SkJoint* p, SrStringArray& paths, bool merge )
{
	SkJoint* j;

	SrString name;
	while ( true ) // we try to cope with names containing spaces
	{ in.get_token();
	if ( in.finished() ) return 0; // unexpected EOF
	if ( in.last_token()[0]=='{' ) break;
	if ( in.last_token()[0]=='}' ) return 0; // should never enter here
	if ( in.last_token() == "joint" ) //keyword 'joint' should be an indication of the start of a new joint regardless of the unfinished definition of previous one
	{
		name.set("\0");
		continue;
	}
	if ( name.len()>0 ) name << ' ';
	name << in.last_token();
	}

	if ( merge )
	{ j = search_joint ( name );
	if ( !j ) // name not found, skip this joint definitions
	{ do { in.get_token(); } while ( !in.finished() && in.last_token()[0]!='}' ); }
	else
	{ j->set_lmat_changed ();
	_loadjdata ( in, j, paths );
	}
	return j;
	}

	j = new SmartBody::SBJoint  ( this, p, SkJoint::TypeQuat, _joints.size() );
	_joints.push_back(j);

	j->name ( (const char*) name );
	j->extName ( (const char*) name );

	int kw=0;
	while ( kw>=0 )
	{ kw = _loadjdata ( in, j, paths );

	if ( kw==JOINT )
	{
		SkJoint* child = _loadj(in,j,paths,merge);
		if ( !child ) return 0;  // Error!
	}
	else if ( kw==END )
	{
		SkJoint* child = _loadj(in,j,paths,merge);
		if ( !child ) return 0;  // Error!
	}
	}

	return j;
}

// parse main keywords
bool SkSkeleton::loadSk ( SrInput& in, double skScale, const char* basedir )
{
	SrString path;
	SrPathArray paths;
	std::vector<std::string> coldet_pairs;
	SrStringArray distrib_joints;
	std::vector<SrArray<float>*> distrib;
	SrInput::TokenType type;
	float scale=1.0f;
	bool scale_offsets=false;
	bool scale_limits=false;
	scale *= (float)skScale;

	in.comment_style ( '#' );
	in.lowercase_tokens ( false );

	// test if this is a merge:
	bool merge=false;
	in.get_token();
	if ( in.last_token()=="joint_data" )
	{ merge=true; }
	else
	{ in.unget_token();
	init ();
	}

	// check basedir:
	if ( basedir )
	{ paths.basedir ( basedir );
	}
	else if ( in.filename() )
	{ paths.basedir_from_filename ( in.filename() );
	}

	while (true)
	{ type = in.get_token();
	if ( type==SrInput::EndOfFile || in.finished() ) break;
	if ( type!=SrInput::Name ) continue;

	SrString& s = in.last_token();

	if ( s=="add_path" || s=="path" )
	{ type = in.get_token();
	if ( type!=SrInput::String ) return false;
	path = in.last_token();
	if ( path.make_valid_path(in.last_token()) )
	{ paths.push(path);
	//SR_TRACE1 ( "added path [" << path << ']' );
	}
	}
	else if ( s=="set_name" || s=="name" )
	{ in.get_token();
	SrString tmpName = in.last_token();
	setName((const char*)tmpName);
	}
	else if ( s=="set_scale" )
	{ in >> scale;
	scale_offsets = true;
	scale_limits = true;
	}
	else if ( s=="set_collision_free_pairs" )
	{ while ( in.get_token()==SrInput::Name )
	coldet_pairs.push_back( (const char*) in.last_token() );
	}
	else if ( s=="add_posture" )
	{ if ( _channels->size()==0 ) make_active_channels ();
	SkPosture* post = new SkPosture;
	post->init(this);
	_postures.push() = post;
	in >> *post;
	}
	else if ( s=="skeleton" || s=="HIERARCHY" )
	{ KeyWord kw = read_keyword(in);
	//if ( kw!=ROOT ) return false;
	if (kw != ROOT) continue; // maybe just another token which happens to be "skeleton"
	_root = _loadj ( in, 0, paths, false /*no merge*/ ); // 0 (null) is the root parent
	}
	else if ( s=="joint_list" )
	{ sr_out<<"joint_list should not exist!!\n";
	_root = _loadjlist ( in, scale, paths ); // 0 (null) is the root parent
	scale_offsets = false;
	}
	else if ( s=="joint" || s=="root" ) // merge joint data case
	{ _loadj ( in, 0, paths, true /*this is a merge*/ );
	}
	else if ( s=="end"  )
	{ break;
	}
	}

	int d;
	float val;
	SkJoint* j;
	SkJoint* j1;
	SkJoint* j2;

	// check scalings
	//   if ( scale_offsets )
	for (size_t i=0; i<_joints.size(); i++ )
		_joints[i]->_offset *= scale;

	if ( scale_limits )
	{ for (size_t i=0; i<_joints.size(); i++ )
	{ j = _joints[i];
	for ( d=0; d<3; d++ )
	{ if ( j->pos()->frozen(d) ) continue;
	val = j->pos()->value(d);
	j->pos()->limits ( d, j->pos()->lower_limit(d)*scale,
		j->pos()->upper_limit(d)*scale );
	j->pos()->value ( d, val*scale );
	}
	}
	}

	/* // this is to debug/test some models:
	if ( 1 )
	{ SrMat m;
	m.scale ( 35.4 );
	for ( i=0; i<_joints.size(); i++ )
	{ j = _joints[i];
	if ( j->visgeo() )
	{ j->visgeo()->apply_transformation(m);
	j->visgeo()->smooth ();
	}
	if ( j->colgeo() ) j->colgeo()->apply_transformation(m);
	}
	}*/

	// transform coldet pair names to joint pointers
	for (size_t i=0; i<coldet_pairs.size(); i+=2 )
	{ j1 = search_joint ( coldet_pairs[i].c_str() );
	j2 = search_joint ( coldet_pairs[i+1].c_str() );
	if ( j1 && j2 )
	{ _coldet_free_pairs.push_back(j1);
	_coldet_free_pairs.push_back(j2);
	}
	}

	// (re)make active channels table
	make_active_channels ();

	compress ();

	updateGlobalMatricesZero();
	//SR_TRACE1 ( "DONE." );

	return _root? true:false;
}

SkJoint* SkSkeleton::_loadjlist ( SrInput& in, float scale, SrStringArray& paths )
{
	int kw;
	SkJoint* j;
	SrString name;
	SrStringArray pnames;
	SrArray<SrVec> offsets;

	in.get_token(); // read "{"
	while ( true )
	{ in.get_token();

	if ( in.last_token_type()!=SrInput::Name && in.last_token_type()!=SrInput::String ) break;

	j = new SmartBody::SBJoint  ( this, 0/*parent*/, SkJoint::TypeQuat, _joints.size() );
	_joints.push_back(j);

	j->name ( (const char*) in.last_token() );
	j->extName( j->jointName() ); 

	in.get_token(); 
	if ( in.last_token()[0]==':' )
	{ in >> name; } // parent name
	else
	{ name="0"; // will be the root
	in.unget_token();
	}
	pnames.push(name);

	in.get_token(); // read "{"

	do { kw = _loadjdata ( in, j, paths );
	} while ( kw>=0 ); // read joint data until finding "}"

	offsets.push() = j->_offset;
	}

	if ( _joints.size()==0 ) return 0;

	_root = _joints[0];
	_root->_offset = offsets[0]*scale;

	size_t k;
	for (size_t i=1; i<_joints.size(); i++ )
	{ // find the parent joint of joint i:
		for ( k=0; k<_joints.size(); k++ )
		{ 
			if ( sr_compare(pnames[i],_joints[k]->jointName().c_str())==0 )
				break;
		}
		// joint k is the parent of joint i:
		if (k<_joints.size() )
		{ _joints[i]->_parent = _joints[k];
		_joints[k]->_children.push_back(_joints[i]);
		_joints[i]->_offset = offsets[i]-offsets[k];
		_joints[i]->_offset *= scale;
		}
	}

	// put geometry in local coordinates, without considering
	// initial dof values:
	make_active_channels ();
	SkPosture* post = new SkPosture;
	post->init ( this ); // save current posture
	post->get();

	for (size_t i=0; i<_joints.size(); i++ ) _joints[i]->init_values();
	_root->update_gmat();
	SrMat mat;
	for (size_t i=0; i<_joints.size(); i++ )
	{ j = _joints[i];
	mat = j->gmat();
	mat.invert();
	if ( j->_visgeo ) j->_visgeo->apply_transformation(mat);
	if ( j->_colgeo ) j->_colgeo->apply_transformation(mat);
	}
	post->apply();
	delete post;

	update_global_matrices ();

	//SR_TRACE1 ( "DONE." );

	return _root;
}

//============================ Save ============================

static void outgeo ( SrOutput& out, const char* s, SkJoint* j, const char* geopath )
{
	SrString name;
	name << j->skeleton()->getName().c_str() << '_'
		<< j->jointName().c_str() << '_'
		<< (const char*)(s[0]=='v'? "vis":"col" ) << ".srm";
	name.lower ();        

	out.outm ();
	out << s << " \"" << name << "\"\n";

	if ( geopath )
	{ SrString fullname(geopath);
	fullname.append ( name );
	SrOutput o ( fopen(fullname,"wt") );
	if ( o.valid() )
	{ if ( s[0]=='v' )
	j->visgeo()->save(o);
	else
		j->colgeo()->save(o);
	}
	}
}

// syntax: channel <?Pos/?Rot> <val> [free | <min><max> | lim <min><max>]
static void outvlim ( SrOutput& out, const char* ctype, SkVecLimits* vl, int d )
{
	if ( vl->value(d)==0 && vl->frozen(d) ) return;

	char clet = 'X'+d;
	if ( ctype[0]=='w' ) clet='T'; // we also handle here Twist channel

	float val = vl->value(d);
	float llim = vl->lower_limit(d);
	float ulim = vl->upper_limit(d);

	if ( ctype[0]!='P' ) // handle euler and twist
	{ val=SR_TODEG(val); llim=SR_TODEG(llim); ulim=SR_TODEG(ulim); }

	out.outm ();
	out << "channel " << clet << ctype << srspc << val;

	if ( vl->limits(d) )
		out << " lim " << llim << srspc << ulim << srnl;
	else    
		out << " free\n";
}

// syntax: channel Quat [axis <x> <y> <z> ang <degrees>] [frozen]
static void outquat ( SrOutput& out, SkJointQuat* q )
{
	float w = q->rawValue().w;
	if ( !q->active() && w==1.0f ) return;
	out.outm ();
	out << "channel Quat";
	if ( w!=1.0 ) out << srspc << q->rawValue();
	if ( !q->active() ) out << " frozen";
	out<<srnl;
}

// syntax: channel Swing [axis <x> <y> ang <degrees>] [lim <xradius> <yradius>]
//         channel Twist <val> [free | <min><max> | lim <min><max>]
static void outst ( SrOutput& out, SkJointSwingTwist* st )
{
	// output swing:  
	out.outm();
	SrVec2 v ( st->swingx(), st->swingy() );
	out << "channel Swing";
	if ( v!=SrVec::null )
	{ float ang = v.len();
	v.normalize();
	out << " axis " << v << " ang " << SR_TODEG(ang);
	}
	// output swing limits:
	v.set ( SR_TODEG(st->ellipsex()), SR_TODEG(st->ellipsey()) );
	if ( v.x<179.999f || v.y<179.999f )  out << " lim " << v;
	out << srnl;

	// output twist, using the VecLimits class:   
	SkVecLimits vl;
	vl.limits ( 0, st->twist_min(), st->twist_max() );
	vl.limits ( 0, st->twist_limits() );
	vl.value ( 0, st->twist() );
	outvlim ( out, "wist", &vl, 0 );
}

static void write_joint ( SrOutput& out, SkJoint* j, int marg, const char* geopath, bool writeall )
{
	int i;
	out<<srnl;
	out.margin ( marg );

	out.outm();
	if ( j->parent() ) out << "joint "; else out << "root ";
	out << j->jointName().c_str() << srnl;

	// write offset :
	out.outm();
	out << "{ offset " << j->offset() << srnl;
	out.margin ( marg+2 );

	if ( writeall )
	{ // write visualization geometry if any :
		if ( j->visgeo() ) outgeo ( out, "visgeo", j, geopath );

		// write collision geometry if any :
		if ( j->colgeo() ) outgeo ( out, "colgeo", j, geopath );
	}

	// write translation channels:  XPos <val> [free | <min><max> | lim <min><max>]
	for ( i=0; i<3; i++ ) outvlim ( out, "Pos", j->pos(), i );

	// write rotation channels:
	switch ( j->rot_type() )
	{ case SkJoint::TypeSwingTwist : outst ( out, j->st() ); break;
	case SkJoint::TypeQuat : outquat ( out, j->quat() ); break;
	case SkJoint::TypeEuler :
		if ( j->euler()->type()==SkJointEuler::TypeZY )
		{ out.outm(); out << "euler ZY\n"; }
		else if ( j->euler()->type()==SkJointEuler::TypeXYZ )
		{ out.outm(); out << "euler XYZ\n"; }
		for ( i=0; i<3; i++ ) outvlim ( out, "Rot", j->euler(), i );
		break;
	};

	// write frame adjustments if any (must come after joint rotation definition):
	if ( j->quat()->has_prepost() )
	{ out.outm();
	out << "prerot " << j->quat()->prerot() << srnl;
	}
	if ( j->quat()->has_prepost() )
	{ out.outm();
	out << "postrot " << j->quat()->postrot() << srnl;
	}

	if ( writeall )
	{ // recurse into children:
		for ( i=0; i<j->num_children(); i++ )
			write_joint ( out, (SkJoint*)j->child(i), marg+2, geopath, writeall );
	}

	out.margin ( marg );
	out.outm();
	out << '}' << srnl;
}

bool SkSkeleton::save ( SrOutput& out, const char* geopath )
{
	int i;
	SrString s;

	// write header:
	out << "# SK Skeleton Definition - M. Kallmann 2004\n\n";
	if ( !_root ) return false;

	SrString fmt ( out.fmt_float() );
	out.fmt_float ( "i" );
	out.margin_char ( srspc );

	// write name, if any:
	s.make_valid_string(getName().c_str());
	if ( s[0] ) out << "set_name " << s << srnl << srnl;

	// check geopath:
	SrString geopathst;
	if ( !geopath && out.filename() )
	{ geopathst = out.filename();
	geopathst.remove_file_name();
	geopath = geopathst;
	}
	if ( geopath )
	{ if ( geopathst.make_valid_path(geopath) )
	geopath = geopathst;
	else
		geopath=0;
	}

	// write the skeleton:
	out << "skeleton";
	write_joint ( out, _root, 0, geopath, true );
	out << srnl;

	// write posture list:
	if ( _postures.size()>0 )
	{ for ( i=0; i<_postures.size(); i++ )
	{ out << "add_posture ";
	if ( _channels==_postures[i]->channels() )
		_postures[i]->output(out,false,true);
	else
		_postures[i]->output(out,true,true);
	}
	out << srnl;
	}

	// write collision-free pairs list:
	if ( _coldet_free_pairs.size()>0 )
	{ out << "set_collision_free_pairs\n";
	for (size_t i=0; i<_coldet_free_pairs.size(); i++ )
	{ s.make_valid_string(_coldet_free_pairs[i]->jointName().c_str());
	out << s;
	if ( i%2==0 ) out<<srspc;
	else if ( i+1==_coldet_free_pairs.size() ) out<<";\n\n";
	else out<<srnl;
	}
	}

	// write the (optional) end keyword:
	out << "end\n";

	// put back the original format:
	out.fmt_float ( fmt );

	return true;
}

// export in joint list format
// and adapt load to do also a merge
bool SkSkeleton::export_joints ( SrOutput& out )
{
	SrString s;

	SrString fmt ( out.fmt_float() );
	out.fmt_float ( "i" );
	out.margin_char ( srspc );

	// write header:
	out << "# SR Skeleton Joint Data - M. Kallmann 2004\n\n";
	out << "joint_data\n";

	if ( _root ) // write joints:
	{ for (size_t i=0; i<_joints.size(); i++ )
	write_joint ( out, _joints[i], 0, "", false );
	}

	// write the (optional) end keyword:
	out << "\nend\n";

	// put back the original format:
	out.fmt_float ( fmt );

	return true;
}


//============================ End of File ============================
