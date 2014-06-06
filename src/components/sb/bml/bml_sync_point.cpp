/*
 *  bml_sync_point.cpp - part of SmartBody-lib
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
 *      Andrew n marshall, USC
 *      Ed Fast, USC
 */

#include "vhcl.h"

#include "bml_sync_point.hpp"

#include <sstream>

#include "bml.hpp"
#include "bml_exception.hpp"
#include "bml_xml_consts.hpp"
#include "sbm/BMLDefs.h"


using namespace std;
using namespace BML;


#define HACK_BAD_SYNC_POINT_REF_WARN_ONLY (1)


// shorthand type for name_to_pos
typedef map<const wstring,BehaviorSyncPoints::iterator> MapNameToPos;  



//const bool USE_CUSTOM_PRUNE_POLICY          = false; // Future feature
//
//const bool LOG_BEHAVIOR_SYNCHPOINTS         = false;
//const bool LOG_BML_BEHAVIOR_SCHEDULE        = false;
//const bool LOG_METHODS						= false;
//const bool LOG_CONTROLLER_SCHEDULE			= false;
//const bool LOG_REQUEST_REALIZE_TIME_SPAN	= false;

TriggerEvent::TriggerEvent( const wstring& name, BmlRequestPtr request )
:	name( name ),
	request( request )
{
	int answer = 42;  //break here
}

void TriggerEvent::init( TriggerEventPtr self ) {
	// TODO: Assert self.get() == this
	weak_ptr = self;
}

SyncPointPtr TriggerEvent::addSyncPoint() {
	return addSyncPoint( SyncPointPtr(), 0 );
}

SyncPointPtr TriggerEvent::addSyncPoint( SyncPointPtr par, float off ) {
	SyncPointPtr sync( new SyncPoint( weak_ptr.lock(), par, off ) );
	sync->init( sync );

	return sync;
}

SyncPoint::SyncPoint( const TriggerEventPtr trigger )
:	trigger(trigger),
	time(TIME_UNSET),
	offset( 0 )  // not used if parent is NULL
{}

SyncPoint::SyncPoint( const TriggerEventPtr trigger, SyncPointPtr par, float off )
:	trigger(trigger),
	time(TIME_UNSET),
	parent( par ),
	offset( off )
{}

void SyncPoint::init( SyncPointPtr self ) {
	// TODO: Assert self.get() == this
	weak_ptr = self;
}

///////////////////////////////////////////////////////////////////////////
//  NamedSyncPointPtr

NamedSyncPointPtr::NamedSyncPointPtr( const wstring& name, SyncPointPtr sync )
:	_name( name ),
	_sync( sync )
{}

NamedSyncPointPtr::NamedSyncPointPtr( const NamedSyncPointPtr& other )
:	_name( other.name() ),
	_sync( other.sync() )
{}



///////////////////////////////////////////////////////////////////////////
//  BehaviorSyncPoints

// default constructor
BehaviorSyncPoints::BehaviorSyncPoints()
{
	start_it = insert( xml_utils::xml_translate_wide( BMLDefs::ATTR_START ), SyncPointPtr(), end() );
	ready_it = insert( xml_utils::xml_translate_wide( BMLDefs::ATTR_READY ), SyncPointPtr(), end() );
	stroke_start_it = insert( xml_utils::xml_translate_wide( BMLDefs::ATTR_STROKE_START ), SyncPointPtr(), end() );
	stroke_it = insert( xml_utils::xml_translate_wide( BMLDefs::ATTR_STROKE ), SyncPointPtr(), end() );
	stroke_end_it = insert( xml_utils::xml_translate_wide( BMLDefs::ATTR_STROKE_END ), SyncPointPtr(), end() );
	relax_it = insert( xml_utils::xml_translate_wide( BMLDefs::ATTR_RELAX ), SyncPointPtr(), end() );
	end_it = insert( xml_utils::xml_translate_wide( BMLDefs::ATTR_END ), SyncPointPtr(), end() );
}

BehaviorSyncPoints::BehaviorSyncPoints( const BehaviorSyncPoints& other )
:	named_syncs( other.named_syncs )
{
	// Repopulate name_to_pos with local iterators
	iterator it = named_syncs.begin();
	iterator end = named_syncs.end();

	for( ; it!=end; ++it ) {
		name_to_pos.insert( make_pair( it->name(), it ) );
	}

	// Rebuild Convience references
	start_it = find( xml_utils::xml_translate_wide( BMLDefs::ATTR_START ) );
	ready_it = find( xml_utils::xml_translate_wide( BMLDefs::ATTR_READY ) );
	stroke_start_it = find( xml_utils::xml_translate_wide( BMLDefs::ATTR_STROKE_START ) );
	stroke_it = find( xml_utils::xml_translate_wide( BMLDefs::ATTR_STROKE ) );
	stroke_end_it = find( xml_utils::xml_translate_wide( BMLDefs::ATTR_STROKE_END ) );
	relax_it = find( xml_utils::xml_translate_wide( BMLDefs::ATTR_RELAX ) );
	end_it = find( xml_utils::xml_translate_wide( BMLDefs::ATTR_END ) );

	
	const std::map<std::wstring, std::wstring>& nameMap = other.getBehaviorToSyncNames();
	for (std::map<std::wstring, std::wstring>::const_iterator iter = nameMap.begin();
		iter != nameMap.end();
		iter++)
	{
		behaviorToSyncName.insert(std::pair<std::wstring, std::wstring>((*iter).first, (*iter).second));
	}
}

BehaviorSyncPoints::iterator BehaviorSyncPoints::insert( const wstring& name, SyncPointPtr sync, BehaviorSyncPoints::iterator pos ) {
	// Is there a way to test is pos is a valid iterator for named_syncs?

	iterator insert_pos;

	MapNameToPos::iterator map_it = name_to_pos.find( name );
	if( map_it != name_to_pos.end() ) {
		// Name already exists

		insert_pos = map_it->second;
		// TODO: check position relative to requested position

		if( insert_pos->sync() ) {
			cout << "ERROR: BehaviorSyncPoints::insert(..): SyncPoint already exists." << endl;
			return end();
		}

		// Update sync point
		insert_pos->set_sync( sync );
	} else {
		insert_pos = named_syncs.insert( pos, NamedSyncPointPtr( name, sync ) );
		if( insert_pos != named_syncs.end() )
			name_to_pos.insert( make_pair( name, insert_pos ) );
	}

	return insert_pos;
}

//BehaviorSyncPoints::iterator BehaviorSyncPoints::pos_of( SyncPointPtr sync ) {
//	return find( begin(), end(), sync );
//}

SetOfWstring BehaviorSyncPoints::get_sync_names() {
	SetOfWstring names;

	MapNameToPos::iterator it  = name_to_pos.begin();
	MapNameToPos::iterator end = name_to_pos.end();
	for( ; it!=end; ++it ) {
		const wstring& name = it->first;
		if( !( names.insert( name ).second ) )
		{
			std::wstringstream wstrstr;
			wstrstr << "ERROR: BehaviorSyncPoints::get_sync_names(): Failed to insert SyncPoint name \""<<name<<"\".";
			std::string str = convertWStringToString(wstrstr.str());
			LOG(str.c_str());
		}
	}

	return names;
}

BehaviorSyncPoints::iterator BehaviorSyncPoints::find( const std::wstring& name ) {
	MapNameToPos::iterator map_it = name_to_pos.find( name );
	bool found_name = map_it != name_to_pos.end();

	return found_name? map_it->second : end();  // return iterator is found, else end()
}

BehaviorSyncPoints::iterator BehaviorSyncPoints::first_scheduled() {
	// Find first sync point that is set
	iterator it = begin();
	iterator it_end = end();
	while( !( it==it_end || it->is_set() ) ) {
		++it;
	}
	return it;
}

BehaviorSpan BehaviorSyncPoints::getBehaviorSpan( time_sec persistent_threshold ) {
	time_sec start_time = TIME_UNSET;
	time_sec end_time   = TIME_UNSET;
	bool     persistent = false;

	iterator it     = begin();
	iterator it_end = end();
	for( ; it != it_end; ++it ) {
		time_sec time = it->time();

		if( isTimeSet( time ) ) {
			if( time < persistent_threshold ) {
				if( isTimeSet( start_time ) ) {
					start_time = min( start_time, time );
					end_time   = max( end_time,   time );
				} else {
					start_time = end_time = time;
				}
			} else {
				persistent = true;
			}
		}
	}

	return BehaviorSpan( start_time, end_time, persistent );
}

void BehaviorSyncPoints::parseStandardSyncPoints( DOMElement* elem, BmlRequestPtr request, const string& behavior_id ) {
	

	// DOM functions never return NULL
//	const wstring tag = elem->getTagName();
	const wstring id  = xml_utils::xml_translate_wide( BMLDefs::ATTR_ID );

	/////////////////////// Create fake ID value?
	//if( !id ) {
	//	// Create fake id
	//	wcerr<<"WARNING: BodyPlannerImpl::parseBML(): <"<<tag<<"> BML tag refers to unknown "<<TM_START<<" point \""<<str<<"\".  Ignoring..."<<endl;
	//	
	//	// count preceeding element siblings
	//	int count = 0;
	//	DOMNode* sibling = elem->getPreviousSibling();
	//	while( sibling != NULL ) {
	//		++count;
	//		sibling = sibling->getPreviousSibling();
	//	}

	//	wstringstream id_buff();
	//	id_buff << 'b';
	//	id_buff.width( 3 );
	//	id_fill( 0 );
	//	id_buff << count;
	//	id_buff.width( 0 );
	//	id_buff << '_' << elem->getTagName(); // TODO: Replace possible namespace colon
	//}

	// Load SyncPoint references
	start_it        = parseSyncPointAttr( elem, id, xml_utils::xml_translate_wide( BMLDefs::ATTR_START ),        request, behavior_id );
	ready_it        = parseSyncPointAttr( elem, id, xml_utils::xml_translate_wide( BMLDefs::ATTR_READY ),        request, behavior_id );
	stroke_start_it = parseSyncPointAttr( elem, id, xml_utils::xml_translate_wide( BMLDefs::ATTR_STROKE_START ), request, behavior_id );
	stroke_it       = parseSyncPointAttr( elem, id, xml_utils::xml_translate_wide( BMLDefs::ATTR_STROKE ),       request, behavior_id );
	stroke_end_it   = parseSyncPointAttr( elem, id, xml_utils::xml_translate_wide( BMLDefs::ATTR_STROKE_END ),   request, behavior_id );
	relax_it        = parseSyncPointAttr( elem, id, xml_utils::xml_translate_wide( BMLDefs::ATTR_RELAX ),        request, behavior_id );
	end_it          = parseSyncPointAttr( elem, id, xml_utils::xml_translate_wide( BMLDefs::ATTR_END ),          request, behavior_id );
}


BehaviorSyncPoints::iterator BehaviorSyncPoints::parseSyncPointAttr( DOMElement* elem, const std::wstring& elem_id, const std::wstring& sync_attr, const BmlRequestPtr request, const string& behavior_id ) {
	
	

	// Does the sync point already exist?
	MapNameToPos::iterator map_it = name_to_pos.find( sync_attr );
	if( map_it != name_to_pos.end() && map_it->second->sync() ) {
		// SyncPoint of this name already exists

		std::string e = xml_utils::xml_w2s( elem_id );
		std::string a = xml_utils::xml_w2s( sync_attr );
		LOG( "BehaviorSyncPoints::parseSyncPointAttr ERR: >>SyncPoint of this name already exists<< %s %s", e.c_str(), a.c_str() );
#if 0
		// TODO: Throw BML ParsingException
		std::stringstream strm;
		strm << "ERROR: Behavior \""<<behavior_id<<"\": BehaviorSyncPoints contains SyncPoint with id \"" << sync_attr << "\".  Ignoring attribute in <"<<elem->getTagName();
		if( !elem_id.empty() )
			strm<<"> id=\""<<elem_id<<"\"";
		else
			strm<<">";
		strm << " and returning existing SyncPoint.";
		LOG(strm.str().c_str());
#endif
		return map_it->second;
	}

	// Get the sync refernce string

	XMLCh* xml_ch_p = xml_utils::xmlch_translate( xml_utils::xml_w2s( sync_attr ) );
	const XMLCh* sync_ref = elem->getAttribute( xml_ch_p );
	xml_utils::xmlch_release( & xml_ch_p );

	SyncPointPtr sync;  // unset by default
	if( sync_ref!=NULL && sync_ref[0]!=0 ) {
		// Has sync reference.
		sync = request->getSyncByReference( xml_utils::xml_translate_wide( sync_ref ) );  // Parses the sync reference notation

		// add to the map of beahvior -> sync point names (ashapiro 5/24/10)
		
		behaviorToSyncName.insert(std::pair<std::wstring, std::wstring>(sync_attr, xml_utils::xml_translate_wide(sync_ref)));

		if( !sync ) {
			// 8-bit C-string references
			string temp_sync_att = xml_utils::xml_w2s( sync_attr );
			string temp_sync_ref = xml_utils::xml_translate_string( sync_ref );

			std::stringstream strstr;
			strstr << "BehaviorSyncPoint:parseSyncPointAttr(..): Invalid SyncPoint reference \"" << temp_sync_ref << "\" in behavior \"" << behavior_id << "\" " << temp_sync_att << "; generated unconstrained sync point.";		
#if HACK_BAD_SYNC_POINT_REF_WARN_ONLY
			LOG(strstr.str().c_str());

			// TEMPORARY HACK: Warn but don't error out, generate blank sync point
			sync = request->start_trigger->addSyncPoint();
#else
			throw BML::ParsingException( oss.str().c_str() );
#endif
		}
	} else {
		sync = request->start_trigger->addSyncPoint();
	}

	
	return insert( sync_attr, sync, end() );
}

#if VALIDATE_BEHAVIOR_SYNCS

string BehaviorSyncPoints::debug_label( SyncPointPtr& sync ) {
	ostringstream out;

	wstring id = idForSyncPoint( sync );

	int count = 0;
	iterator it  = named_syncs.begin();
	iterator end = named_syncs.end();
	while( it!=end && it->sync()!=sync ) {
		++it;
		++count;
	}

	if( it==end ) {
		out << "Invalid SyncPoint";
	} else {
		out << "SyncPoint #" << count;
	}
	if( !id.empty() ) {

		string id_str = xml_utils::xml_w2s( id );
		out << " \"" << id_str << "\"";
	}

	return out.str();
}

void BehaviorSyncPoints::validate() {
	ostringstream out;
	out << "BehaviorSyncPoints validation errors:";

	bool valid = true;

	iterator it  = named_syncs.begin();
	iterator end = named_syncs.end();
	if( it != end ) {
		SyncPointPtr sync = it->sync();

		if( it != start_it ) {
			out << endl << "\t" << debug_label(sync) << " is not sp_start";
				valid = false;
		}
		if( !isTimeSet( sync->time ) ) {
			out << endl << "\t" << debug_label(sync) <<" time is not set";

			// Find first set time
			do {
				++it;
			} while( it!=end && !isTimeSet( it->time() ) );

			if( it == end ) {
				out << endl << "\tAll SyncPoint::time are unset";
				valid = false;
				return;
			}
		}

		SyncPointPtr prev( sync );
		for( ++it; it != end; ++it ) {
			sync = it->sync();

			if( isTimeSet( sync->time ) ) {
				if( sync->time < prev->time ) {
					out << endl << "\t" << debug_label( sync ) << " time " << sync->time << " less than prev->time " << prev->time;
				}

				prev = sync;
			}
		}
	} else {
		out << endl << "\tNo SyncPoints";
		valid = false;
	}

	if( !valid ) {
		throw SchedulingException( out.str().c_str() );
	}
}

#endif // INCOMPLETE_SYNCS_VALIDATION

std::wstring BehaviorSyncPoints::idForSyncPoint( SyncPointPtr sync ) {
	iterator it = named_syncs.begin();
	iterator end = named_syncs.end();

	for( ; it!=end; ++it ) {
		if( it->sync() == sync ) {
			return it->name();  // early return
		}
	}

	// No match
	return wstring();
}

void BehaviorSyncPoints::applyParentTimes( std::string& warning_context ) {
	iterator it  = named_syncs.begin();
	iterator end = named_syncs.end();

	for( ; it!=end; ++it ) {
		SyncPointPtr sync = it->sync();
		if( sync ) {
			SyncPointPtr parent = sync->parent;
			if( parent ) {
				if( parent->is_set() ) {
					if( sync->is_set() ) {
						string ascii_sync_id = xml_utils::xml_w2s( it->name() );

						std::stringstream strstr;
						strstr << "WARNING: ";
						if( !warning_context.empty() )
							strstr << warning_context << ": ";
						strstr << "SyncPoint \""<<ascii_sync_id<<"\" time set before applyParentTimes().";
						LOG(strstr.str().c_str());
					}
					sync->time = parent->time + sync->offset;
				} else {
					string ascii_sync_id = xml_utils::xml_w2s( it->name() );

					// Parent not set!!
					std::stringstream strstr;
					strstr << "WARNING: ";
					if( !warning_context.empty() )
						strstr << warning_context << ": ";
					strstr << "SyncPoint \""<<ascii_sync_id<<"\" parent unset.";
					LOG(strstr.str().c_str());
				}
			}
		}
	}
}

void BehaviorSyncPoints::printSyncIds() {
#ifndef __ANDROID__
	wostringstream buffer;
	if( !named_syncs.empty() ) {
		iterator it = named_syncs.begin();
		iterator end = named_syncs.end();
		// First SyncPoint
		buffer << it->name();
		// Remaining
		for( ++it; it!=end; ++it ) {
			buffer << ", " << it->name();
		}
	}

	wcout << buffer.str() << endl;
#endif
}

void BehaviorSyncPoints::printSyncTimes() {
#ifndef __ANDROID__
	wostringstream buffer; // buffer output for single write
	if( !named_syncs.empty() ) {
		iterator it = named_syncs.begin();
		iterator end = named_syncs.end();

		// Iterate Syncs in order
		for( ; it!=end; ++it ) {
			wcout << "\t" << it->name() << ": " << it->sync() << endl;
		}
	}
	wcout << buffer.str();
#endif
}

const std::map<std::wstring, std::wstring>& BehaviorSyncPoints::getBehaviorToSyncNames() const
{
	return behaviorToSyncName;
}

