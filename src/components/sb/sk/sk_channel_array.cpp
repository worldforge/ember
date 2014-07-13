/*
*  sk_channel_array.cpp - part of Motion Engine and SmartBody-lib
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
*      Marcelo Kallmann, USC (currently at UC Merced)
*      Andrew n marshall, USC
*      Marcus Thiebaux, USC
*/

# include <sk/sk_channel_array.h>
# include <sk/sk_skeleton.h>
#include <sb/SBScene.h>
#include <sb/SBJointMapManager.h>
#include <sb/SBJointMap.h>

//============================= SkChannelArray ============================
//  A persistent empty SkChannelArray returned by empty_channel_array().
SkChannelArray SkChannelArray::EMPTY_CHANNELS = SkChannelArray();


SkChannelArray::SkChannelArray ()
{
	_floats = 0;
	jointMapName = "";

}

SkChannelArray::~SkChannelArray ()
{
}

void SkChannelArray::init()
{
	_floats = 0;
	_channelMap.clear();
	_channelMapedNameMap.clear();
	_channelList.clear();	
}

bool SkChannelArray::doesChannelExist(std::string name, SkChannel::Type t)
{
	std::map<std::string, std::map<SkChannel::Type, int> >::iterator iter = _channelMap.find(name);
	if (iter != _channelMap.end())
	{
		std::map<SkChannel::Type, int>& typeMap = (*iter).second;
		std::map<SkChannel::Type, int>::iterator typeIter = typeMap.find(t);
		if (typeIter != typeMap.end())
		{
			return true;
		}
	}

	return false;
}

void SkChannelArray::_add ( SkJoint* j, std::string name, SkChannel::Type t, bool connect ) {
	if ( name == "" && j )
		name = j->jointName();

	// does this channel name and type already exist
	Channel channel;
	channel.set(j, t);
	channel.name = name;	
	channel.mappedName = name; // should be the same initially

	bool exist = doesChannelExist(name, t);
	if (exist)
		return;

	std::map<std::string, std::map<SkChannel::Type, int> >::iterator iter = _channelMap.find(name);
	if (iter != _channelMap.end())
	{
		std::map<SkChannel::Type, int>& typeMap = (*iter).second;
		std::map<SkChannel::Type, int>::iterator typeIter = typeMap.find(t);

		_channelList.push_back(channel);
		typeMap.insert(std::pair<SkChannel::Type, int>(t, _channelList.size() - 1));
		_floats += SkChannel::size(t);
	}
	else
	{
		_channelList.push_back(channel);
		std::map<SkChannel::Type, int> typeMap;
		typeMap.insert(std::pair<SkChannel::Type, int>(t, _channelList.size() - 1));
		_channelMap.insert(std::pair<std::string, std::map<SkChannel::Type, int> >(name, typeMap));
		_floats += SkChannel::size(t);
	}
	_dirty = true;
}

bool SkChannelArray::insert ( int pos, std::string name, SkChannel::Type t )
{
	if (pos < 0 || size_t(pos) > _channelList.size())
		return false;

	bool exist = doesChannelExist(name, t);
	if (exist)
		return false;

	Channel channel;
	channel.set(NULL, t);
	channel.name = name;	
	channel.mappedName = name;
	int counter = 0;
	for (std::vector<Channel>::iterator iter = _channelList.begin();
		iter != _channelList.end();
		iter++)
	{
		if (counter == pos)
		{
			_channelList.insert(iter, channel);
			_floats += SkChannel::size(t);
			break;
		}
		counter++;
	}

	std::map<std::string, std::map<SkChannel::Type, int> >::iterator iter = _channelMap.find(name);
	if (iter != _channelMap.end())
	{
		std::map<SkChannel::Type, int>& typeMap = (*iter).second;
		std::map<SkChannel::Type, int>::iterator typeIter = typeMap.find(t);

		typeMap.insert(std::pair<SkChannel::Type, int>(t, counter));
	}
	else
	{
		std::map<SkChannel::Type, int> typeMap;
		typeMap.insert(std::pair<SkChannel::Type, int>(t, counter));
		_channelMap.insert(std::pair<std::string, std::map<SkChannel::Type, int> >(name, typeMap));
	}

	return true;
}

SkSkeleton* SkChannelArray::skeleton () const
{
	if (_channelList.size() == 0)
	{
		return NULL;
	}
	else
	{
		if (!_channelList[0].joint)
			return NULL;
		else
			return _channelList[0].joint->skeleton();
	}
}

void SkChannelArray::get_active_channels ( SkSkeleton* sk, bool connect )
{
	init();
	add_active_channels( sk, connect );
}

void SkChannelArray::add_active_channels ( SkSkeleton* sk, bool connect )
{
	int i;
	int jsize = sk->joints().size();
	SkJoint* joint;

	for (i = 0; i < jsize; i++)
	{
		joint = sk->joints()[i];

		// position channels:
		if (!joint->pos()->frozen(0))
			add(joint, SkChannel::XPos, connect);
		if (!joint->pos()->frozen(1))
			add(joint, SkChannel::YPos, connect);
		if (!joint->pos()->frozen(2))
			add(joint, SkChannel::ZPos, connect);

		// rotation channels:
		if (!joint->quat()->active()) 
			continue;
		switch (joint->rot_type())
		{
		case SkJoint::TypeQuat:
			add(joint, SkChannel::Quat, connect);
			break;
		case SkJoint::TypeSwingTwist:
			add(joint, SkChannel::Swing);
			if (!joint->st()->twist_frozen())
				add(joint, SkChannel::Twist, connect);
			break;
		case SkJoint::TypeEuler:
			if (!joint->euler()->frozen(0))
				add(joint, SkChannel::XRot, connect);
			if (!joint->euler()->frozen(1))
				add(joint, SkChannel::YRot, connect);
			if (!joint->euler()->frozen(2))
				add(joint, SkChannel::ZRot, connect);
			break;
		}
	}
	count_floats();
	if (jointMapName != sk->getJointMapName())
	{
		jointMapName = sk->getJointMapName();			
	}		
	rebuild_hash_table();
}

int SkChannelArray::count_floats ()
{
	int csize = _channelList.size();
	_floats = 0;
	for (int i = 0; i < csize; i++)
	{ 
		_floats += _channelList[i].size();
	}
	return _floats;
}

int SkChannelArray::float_position ( int c ) const
{
	if (size_t(c) >= _channelList.size()) 
		c = _channelList.size() - 1;
	int pos = 0;
	for (int i = 0; i < c; i++)
	{ 
		pos += _channelList[i].size();
	}
	return pos;	
}

void SkChannelArray::get_values ( float* fp )
{
	int csize = _channelList.size();
	for (int i = 0; i < csize; i++)
	{ 
		fp += _channelList[i].get(fp);
	}
}

void SkChannelArray::set_values ( const float* fp )
{
	int csize = _channelList.size();
	for (int i = 0; i < csize; i++)
	{ 
		fp += _channelList[i].set(fp);
	}
}

//void SkChannelArray::get_random_values ( float* fp ) const
// {
//   int i, csize = _channels.size();
//   for ( i=0; i<csize; i++ )
//    { fp += _channels[i].get_random ( fp );
//    }
// }

bool SkChannelArray::get_used_channels ( const std::vector<SkPosture*>& postures, std::vector<int>* indices )
{
	init ();
	if ( postures.size()<1 ) return 0;

	// check the actual channels being used:
	SrBuffer<float>& firstpost = postures[0]->values;
	int firstpostsize = firstpost.size();
	std::vector<int> locindices;
	std::vector<int>& index = (indices? *indices:locindices);

	// init as -1 meaning indices not used (or 1 if one channel):
	index.resize ( firstpost.size() ); // index has size==floats, not channels
	if ( firstpost.size()==1 )
		index[0] = 1;
	else
	{
		for (size_t i = 0; i < index.size(); i++)
			index[i] = -1;
	}

	// mark with 1 the indices of active channels, ie, those with changing values:
	for (size_t i=1; i<postures.size(); i++ )
	{ if ( postures[i]->values.size()!=firstpost.size() ) return false;
	for (int j=0; j<firstpostsize; j++ )
	{ if ( postures[i]->values[j]!=firstpost[j] ) index[j]=1;
	}
	}

	// now get only the used channels:
	SkChannelArray& ch = *postures[0]->channels();
	int csize;
	int postsize=0;
	int j = 0;
	for (int i=0; i<ch.size(); i++ )
	{ csize = ch[i].size();//ch[i].size(ch[i].type);

	if ( index[j]<0 ) // not used: advance j to next value and continue
	{ j+=csize; continue; }
	else
	{ while ( csize-->0 ) // save in index the new indices of this channel
	{ index[j++] = postsize++; }
	add ( ch[i] ); // get the channel
	}
	}

	return true;
}

int SkChannelArray::linear_search (const std::string& name, SkChannel::Type type) const
{
	int chs = _channelList.size();

	for (int i = 0; i < chs; i++)
	{ 
		if ( _channelList[i].type == type && 
			(_channelList[i].mappedName == name) || (_channelList[i].name == name))
		{ 
			return i;
		}
	}

	return -1;
}

int SkChannelArray::search ( const std::string& name, SkChannel::Type type )
{
	if( _channelList.size()==0 )
		return -1;                  

	if (_dirty || _channelMap.size() != _channelMapedNameMap.size())
	{
		rebuild_hash_table();
		_dirty = false;
	}

	std::map<std::string, std::map<SkChannel::Type, int> >::iterator iter = _channelMapedNameMap.find(name);
	if (iter != _channelMapedNameMap.end())
	{
		std::map<SkChannel::Type, int>& typeMap = (*iter).second;
		std::map<SkChannel::Type, int>::iterator typeIter = typeMap.find(type);
		if (typeIter == typeMap.end())
		{
			return -1;
		}
		else
		{
			return (*typeIter).second;
		}
	}
	else
	{
		return -1;
	}
}

void SkChannelArray::rebuild_hash_table()
{
	_channelMapedNameMap.clear();
        _channelMap.clear();
	_floats = 0;

	int index = 0;
	for (std::vector<Channel>::iterator iter = _channelList.begin();
		iter != _channelList.end();
		iter++)
	{
		Channel& channel = (*iter);
		const std::string& mappedChannelName = getMappedChannelName(channel);
		std::string& channelName = channel.name;
		std::map<std::string, std::map<SkChannel::Type, int> >::iterator origMapIter = _channelMap.find(channelName);
		std::map<std::string, std::map<SkChannel::Type, int> >::iterator mapIter = _channelMapedNameMap.find(mappedChannelName);
		// this is the only time we update the mapped name cache in each channel
		channel.mappedName = mappedChannelName;

		if (mapIter != _channelMapedNameMap.end())
		{
			std::map<SkChannel::Type, int>& typeMap = (*mapIter).second;
			std::map<SkChannel::Type, int>::iterator typeIter = typeMap.find(channel.type);

			typeMap.insert(std::pair<SkChannel::Type, int>(channel.type, index));
		}
		else
		{
			std::map<SkChannel::Type, int> typeMap;
			typeMap.insert(std::pair<SkChannel::Type, int>(channel.type, index));
			_channelMapedNameMap.insert(std::pair<std::string, std::map<SkChannel::Type, int> >(mappedChannelName, typeMap));
		}
		// keep track of original map as well
		if (origMapIter != _channelMap.end())
		{
			std::map<SkChannel::Type, int>& typeMap = (*origMapIter).second;
			std::map<SkChannel::Type, int>::iterator typeIter = typeMap.find(channel.type);

			typeMap.insert(std::pair<SkChannel::Type, int>(channel.type, index));
		}
		else
		{
			std::map<SkChannel::Type, int> typeMap;
			typeMap.insert(std::pair<SkChannel::Type, int>(channel.type, index));
			_channelMap.insert(std::pair<std::string, std::map<SkChannel::Type, int> >(channelName, typeMap));
		}

		_floats +=  channel.size();
		index++;
	}
}

int SkChannelArray::connect(SkSkeleton* s)
{
	int count = 0;
	int chsize = _channelList.size();
	SkJoint* joint = NULL;

	if (!s)
	{ 
		for (int i=0; i < chsize; i++ ) 
			_channelList[i].joint = NULL;
		return 0;
	}

	for (int i = 0; i < chsize; i++ )
	{ 
		//joint = s->search_joint(_channelList[i].name.c_str());
		//joint = s->search_joint(name(i).c_str());
		joint = s->search_joint(mappedName(i).c_str());
		_channelList[i].joint = 0;
		if (joint)
		{ 
			if (SkChannel::valid(type(i), joint->rot_type()) )
			{ 
				_channelList[i].joint = joint;
				count++;
			}
		}
	}

	return count;
}
/*
// Without using the hash table, with 53 channels, a test performed 1431 comparisons
// to accomplish the mapping.
// With a hash size of 53*2=106, the hash table had longest entry==2, and 111 elements,
// meaning only 111-106==5 colliding elements!
// Therefore the comparisons number should drop to (53-5)+2*5==58!
void SkChannelArray::map ( SkChannelArray& ca, SrBuffer<int>& m )
{
// we rebuild here (instead of in search()) to also have _floats recalculated:
if ( !_htable ) rebuild_hash_table();

m.size ( _floats );

int c1, c2, cmap, csize, v, i=0;
for ( c1=0; c1<size(); ++c1 ) // for each channel c1
{ 
c2 = ca.search ( name(c1), type(c1) ); // search it in ca
if ( c2<0 )
{ cmap=-1; } // not there, mark as -1
else
{ cmap=ca._channels[c2].fmap; } // found, get its float mapping

csize = _channels[c1].size();
for ( v=0; v<csize; ++v )
m[i++] = cmap<0? cmap:cmap+v;
}
}
*/

/* Old O(n^2) implementation:
void SkChannelArray::map ( const SkChannelArray& ca, SrBuffer<int>& m )
{
if ( !_htable ) rebuild_hash_table();

m.size ( _floats );

int c1, c2, mfloat, cmap, csize, v, i=0;
for ( c1=0; c1<size(); c1++ ) // for each channel c1
{ 
cmap = -1;
mfloat = 0;
for ( c2=0; c2<ca.size(); c2++ )
{ if ( name(c1)==ca.name(c2) && type(c1)==ca.type(c2) ) // a match
{ cmap=mfloat; break; }
mfloat += ca.const_get(c2).size();
}

csize = _channels[c1].size();
for ( v=0; v<csize; v++ )
m[i++] = cmap<0? cmap:cmap+v;
}
}*/

bool SkChannelArray::merge ( SkChannelArray& ca )
{
	std::vector<std::string> namesToAdd;
	std::vector<SkChannel::Type> typesToAdd;
	for (int c=0; c<ca.size(); c++ ) // for each channel c in ca
	{ 
		std::string name = ca.name(c);
		SkChannel::Type t = ca.type(c);
		int pos = search(name, t);
		if (pos < 0) // missing channel found
		{ 
			namesToAdd.push_back(name);
			typesToAdd.push_back(t);
		}
	}

	for (unsigned int x = 0; x < namesToAdd.size(); x++)
	{
		add(namesToAdd[x], typesToAdd[x]); // add it	
	}


	if (jointMapName != ca.getJointMapName())
	{
		jointMapName = ca.getJointMapName();		
	}

	if (namesToAdd.size() > 0)
	{
		rebuild_hash_table();
		return true;
	}
	else
	{
		return false;
	}
}

void SkChannelArray::operator = ( const SkChannelArray& a )
{
	if (this == &a)
	{
		return;
	}

	_channelList.clear();
	for (size_t x = 0; x < a._channelList.size(); x++)
	{
		_channelList.push_back(a._channelList[x]);
	}
	if (jointMapName != a.getJointMapName())
	{
		jointMapName = a.getJointMapName();		
	}
	rebuild_hash_table();
	_floats = a._floats;	
	_dirty = true;
}

bool SkChannelArray::operator == ( const SkChannelArray& a )
{
	if (size() != a.size())
		return false;

	for (int i=0; i<size(); i++ )
	{ 
		if (_channelList[i].type != a._channelList[i].type ||
			_channelList[i].name != a._channelList[i].name ) 
			return false;
	}
	return true;
}

SrOutput& operator<< ( SrOutput& o, const SkChannelArray& ca )
{
	int i;
	o << "channels " << ca.size() << srnl;
	for ( i=0; i<ca.size(); i++ )
	{ 
		o << ca.name(i).c_str() << srspc
			<< ca.const_get(i).type_name() << srnl;
	}
	return o;
}

SrInput& operator>> ( SrInput& in, SkChannelArray& ca )
{
	in.get_token(); // read "channels"

	int n = in.getn().atoi(); // number of channels
	if ( in.last_error()==SrInput::UnexpectedToken ) return in;

	ca.init ();
	ca._channelList.reserve(256); // reserve 256 entries
/*
	std::vector<std::string> tokens;
	SrString curTok;
	bool firstTime = true;
	int curLine = in.curline();
	int tmpLine = 0;
	while ( n >= 0 )
	{
		in.get_token (curTok);
		int tmpLine = in.curline();
		if (firstTime)
		{
			curLine= tmpLine;
			firstTime = false;
		}
		if (tmpLine > curLine && !firstTime)
		{
			std::string name;
			for (unsigned int i = 0; i < tokens.size() - 1; i++)
			{
				if (i > 0)
					name.append(" ");
				name.append(tokens[i]);
			}
			ca.add ( name, SkChannel::get_type(tokens[tokens.size() - 1].c_str()) );
			n--;
			tokens.clear();
			tokens.push_back(std::string(curTok));
			curLine = tmpLine;
		}
		else
		{
			tokens.push_back(std::string(curTok));
		}


	}

	in.unget_token();
*/

	SrString name;
	while ( (n--) > 0 )
	{ in.get_token ( name );
	in.get_token ();
	ca.add ( std::string(name), SkChannel::get_type(in.last_token()) );
	}

	//   ca.compress();

	return in;
}


void SkChannelArray::changeChannelName(std::string oldName, std::string newName)
{
	bool channelFound = false;
	for (size_t c = 0; c < _channelList.size(); c++)
	{	
		if ( (oldName == _channelList[c].name || oldName == _channelList[c].mappedName) && !_channelUpdateTable[c])
		{
			_channelList[c].name = newName;
			channelFound = true;
			_channelUpdateTable[c] = true;
		}
	}

	if (_channelMap.find(oldName) != _channelMap.end())
	{
		std::map<SkChannel::Type, int> entry = _channelMap[oldName];
		_channelMap[newName] = entry;
		_channelMap.erase(oldName);
	}

	_dirty = true;
}

void SkChannelArray::startChannelNameChange()
{
	_channelUpdateTable.resize(_channelList.size());
	for (unsigned int c=0;c< _channelList.size(); c++)
		_channelUpdateTable[c] = false;
}

const std::string& SkChannelArray::getMappedChannelName( const Channel& chan ) const
{
	SmartBody::SBJointMap* jointMap = SmartBody::SBScene::getScene()->getJointMapManager()->getJointMap(jointMapName);
	if (jointMap)
	{
		const std::string& target = jointMap->getMapTarget(chan.name);
		if (target != "")
			return target;
	}
	return chan.name;
}

const std::string SkChannelArray::name( int i ) const
{
	const Channel& ch = _channelList[i];
	//return getChannelName(ch);
	return ch.name;
}

const std::string SkChannelArray::mappedName( int i ) const
{
	const Channel& ch = _channelList[i];
	return ch.mappedName;
}


void SkChannelArray::setJointMapName( const std::string& jmapName )
{
	jointMapName = jmapName;
}

std::string SkChannelArray::getJointMapName() const
{
	return jointMapName;
}
//============================ End of File ============================

