/*
 *  sk_channel_array.h - part of Motion Engine and SmartBody-lib
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
 */

# ifndef SK_CHANNEL_ARRAY_H
# define SK_CHANNEL_ARRAY_H

#include <sb/SBTypes.h>
# include <sr/sr_buffer.h>
# include <sr/sr_shared_class.h>

# include <sk/sk_channel.h>
#include <string>
#include <vector>
#include <map>

class SkPosture;


/*! SkChannelArray manipulates an array of channels and is used to
    specify postures and motions. Referenced joints can be null
    if they are not 'connected' to an existing joint, in which
    case the given name is used to connect to a skeleton. */
class SkChannelArray : public SrSharedClass
{
private :
	//////////////////////////////////////////////////////////////////////////
	//  Private Constants

	/*! Persistent empty channel array.
	    See empty_channel_array() function.
	 */
	static SkChannelArray EMPTY_CHANNELS;

	//////////////////////////////////////////////////////////////////////////

	class Channel : public SkChannel
	{ 
		public:
			Channel()
			{
				name = "";
				fmap = 0;
			}
			void operator=(const Channel& c)
			{
				name = c.name;
				fmap = c.fmap;
				SkChannel::operator=(c);
			}					
		int fmap;		
		std::string name; 
		std::string mappedName;
	};
	//SrArray<Channel> _channels;
	//HashTable* _htable;
	int _floats;
	bool _dirty;

	std::map<std::string, std::map<SkChannel::Type, int> > _channelMap;
	std::map<std::string, std::map<SkChannel::Type, int> > _channelMapedNameMap;
	std::vector<Channel> _channelList;	
	std::vector<bool>    _channelUpdateTable;
	std::string jointMapName;
public :
	//////////////////////////////////////////////////////////////////////////
	//  Public Methods

	/*! Constructor */
	SkChannelArray ();

	/*! Destructor is public but be sure to check if unref() has to be used */
	virtual ~SkChannelArray ();

	void setJointMapName(const std::string& jmapName);
	std::string getJointMapName() const ;

	const std::string& getMappedChannelName(const Channel& chan) const;
	/*! Set the array as empty */
	void init ();

	/*! Compress internal tables */
//	void compress () { _channels.compress(); }

	/*! Returns the number of channels in the array */
	int size () const { return _channelList.size(); }

	bool doesChannelExist(std::string name, SkChannel::Type t);

	/*! Add in the end of the array a new channel, by giving
		only its name and type; the joint is set to null (disconnected) */
	SBAPI void add ( std::string name, SkChannel::Type t ) { _add(0,name,t,false); }

	/*! Add in the end of the array a new channel */
	void add ( SkJoint* j, SkChannel::Type t, bool connect=false )
		{ _add(j, "" ,t,connect); }

	/*! Same as add(ch.joint,ch.type) */
	void add ( SkChannel ch ) { _add(ch.joint, "",ch.type,false); }

	void changeChannelName(std::string oldName, std::string newName);
	void startChannelNameChange(); // reset bool table

	/*! Inserts a channel in the array at given position. The joint is
		set as null and can be 'connected' later */
	bool insert ( int pos, std::string name, SkChannel::Type type );

	/*! Access operator */
	SkChannel& operator[] ( int i ) { return _channelList[i]; }
	SkChannel& get ( int i ) { return _channelList[i]; }
	const SkChannel& const_get ( int i ) const { return _channelList[i]; }

	/*! Returns the last channel added */
	SkChannel& top () { return _channelList[_channelList.size() -1]; }

	/*! Returns the declared joint name for the channel. It is the
		user responsibility to ensure that 0<=i and i<size() */
	SBAPI const std::string name ( int i ) const;
	SBAPI const std::string mappedName ( int i ) const;

	/*! Returns the joint of channel i. The joint pointer will only be valid (i.e.!=0)
		if the channel was succesfully connected to a skeleton joint.
		It is the user responsibility to ensure that 0<=i and i<size() */
	SkJoint* joint ( int i ) const { return _channelList[i].joint; }

	/*! Returns the type of channel i. It is the user
		responsibility to ensure that i<=0 and i<size() */
	SkChannel::Type type ( int i ) const { return _channelList[i].type; }

	/*! Returns the skeleton associated with the first channel joint or null if empty.
		Note that the skeleton is expected to be the same for all channels */
	SkSkeleton* skeleton () const;

	/*! Builds the channels from the active joints in the given skeleton */
	SBAPI void get_active_channels ( SkSkeleton* sk, bool connect=false );

	/*! Adds the channels from the active joints in the given skeleton */
	void add_active_channels ( SkSkeleton* sk, bool connect=false );

	/*! Recalculates and stores the number of floats required to store all the
		channels. Note that it is invalidated each time a channel type is
		changed without using the SkChannelArray methods. */
	int count_floats ();

	/*! Returns the number of floats required to store all the channels. */
	int floats () const { return _floats; }

	/*! Counts and returns the position of the 1st value used by channel c */
	SBAPI int float_position ( int c ) const;

	/*! Put in fp all the floats specified by the joints of the channel array */
	void get_values ( float* fp );

	/*! Put in the channels joints the floats specified in fp */
	void set_values ( const float* fp );

	///*! Get random values according to each channel type and limits */
	//void get_random_values ( float* fp ) const;

	/*! Get only the channels with a varying value, in relation to the first
		posture in the given list of postures.
		Array pointer indices, if not null, will translate the indices of the old
		and new values, eg, newval[index[i]] will reference oldval[i], if index[i]>=0.
		True is returned if parameters were correct, false otherwise. */
	bool get_used_channels ( const std::vector<SkPosture*>& postures, std::vector<int>* indices=0 );

	/*! Returns the channel position of given channel type and joint name, or -1 if not found.
		This method uses a linear search, not relying in the internal hash table option. */
	int linear_search ( const std::string& name, SkChannel::Type type ) const;

	/*! Returns the channel position of given channel type and joint name, or -1 if not found.
		This method uses an internal hash table to speed up the search; however the hash
		table must be up to date. If the hash table is not built, it will automatically be
		built, but if the channel array is changed after that, it is the user responsibility
		to manually call method rebuild_hash_table(). */
	int search ( const std::string& name, SkChannel::Type type );

	

	/*! An internal hash table is used to optimize search in two methods: search() and map().
		Whenever the channel array is edited, the user must call this method to ensure the
		internal hash table is up to date. */
	void rebuild_hash_table ();

	/*! Matches the channel names with the joint names in the given skeleton,
		storing the corresponding joint pointers in the channels. Joints
		not matched will result in null joint pointers in the channel array.
		The number of channels matched is returned. A null
		parameter will set all joint pointers to null (disconnection).
		This method is efficient as it uses the SkSkeleton::search_joint() */
	int connect ( SkSkeleton* s );

	/*! Simply calls connect(0) */
	void disconnect () { connect(0); }

	/*! Builds a mapping between the channel array and ca, such that 
		the name and type of channel i are the same as in ca[m[i]],
		if m[i] is >-1. SrBuffer m will have size floats().
		The internal hash table is used to speed up the search and
		it must be up to date. It is here automatically built if needed,
		but if the channel array is changed after that, method 
		rebuild_hash_table() must be called before calling map().
		('ca' can be considered const, only its hash table may be changed)*/
//	void map ( /*const*/ SkChannelArray& ca, SrBuffer<int>& m );

	/*! Adds to the channel array all channels that are in ca,
		but are not in the channel array. The "floats counter" is updated. */
	bool merge ( SkChannelArray& ca );

	/*! Copy operator copies all fields. */
	void operator = ( const SkChannelArray& a );

	/*! Comparison operator checks if the channel names and types are the same. */
	bool operator == ( const SkChannelArray& a );

	/*! Output operator */
	friend SrOutput& operator<< ( SrOutput& o, const SkChannelArray& ca );

	/*! Input operator */
	friend SrInput& operator>> ( SrInput& in, SkChannelArray& ca );
    
	/*! Get a reference to the empty channel array.
	    Needed by odd cases in the motion engine
	 */
	static SkChannelArray& empty_channel_array() {
		EMPTY_CHANNELS.init();  // Just in case someone messed with it...
		return EMPTY_CHANNELS;
	}

private:
	void _add ( SkJoint* j, std::string name, SkChannel::Type t, bool connect );
};

//==================================== End of File ===========================================

# endif  // SK_CHANNEL_ARRAY_H
