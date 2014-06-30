/*
 *  bml_sync_point.hpp - part of SmartBody-lib
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
 *      Corne Versloot, while at USC
 *      Ed Fast, USC
 */

#ifndef BML_SYNC_POINT_HPP
#define BML_SYNC_POINT_HPP

#include <sb/SBTypes.h>

#include <string>
#include <list>
#include <map>
//#include <limits>

#include "bml_types.hpp"
#include "bml/behavior_span.hpp"




#define VALIDATE_BEHAVIOR_SYNCS (1)



namespace BML {
	//  Intended to mark an external event by which a behavior,
	//  behavior request, or some subset, could be scheduled against
	//  (such as a wait event, or a speech synth ready event).
	//  Currently unused in this capacity (one TriggerEvent per BehaviorRequest).
	class TriggerEvent {
	public:
		std::wstring        name;     // for logging / debugging
		BmlRequestWeakPtr   request;

	private:
		TriggerEventWeakPtr weak_ptr;  // weak reference to the reference count struct

	protected:
		TriggerEvent( const std::wstring& name, BmlRequestPtr request );
		void init( TriggerEventPtr self );

	public:
		SyncPointPtr addSyncPoint();  // adds SyncPoint before end of trigger
		SyncPointPtr addSyncPoint( SyncPointPtr par, float off );

		friend class BmlRequest;
	};

	class SyncPoint {
	public:
		const TriggerEventWeakPtr trigger;
		time_sec                  time;  // TIME_UNSET implies it has not been set
		SyncPointPtr              parent;
		float			          offset;	

		bool is_set() { return isTimeSet( time ); }
		void set_time( time_sec time ) { this->time = time; }

	private:
		SyncPointWeakPtr         weak_ptr;  // weak reference to the reference count struct

	protected:
		SyncPoint( const TriggerEventPtr trigger );
		SyncPoint( const TriggerEventPtr trigger, SyncPointPtr par, float off );
		void init( SyncPointPtr self );

	public:
		friend class BmlRequest;
		friend class TriggerEvent;
	};

	/**
	 *  A SyncPointPtr with a name,
	 *  not to be confused with a shared_ptr to the name of a SyncPoint.
	 *  (English is confusing when you try to avoid prepositions.)
	 *
	 *  Only a BehaviorSyncPoints can update the sync reference
	 *  once the NamedSyncPointPtr is created.
	 */
	class NamedSyncPointPtr {
	protected:
		std::wstring _name;
		SyncPointPtr _sync;

	public:
		NamedSyncPointPtr( const std::wstring& name, SyncPointPtr sync );
		NamedSyncPointPtr( const NamedSyncPointPtr& other );

		std::wstring name() const { return _name; }
		SyncPointPtr sync() const { return _sync; }

		// Convience Methods
		bool is_set() const
		{	return _sync->is_set(); }

		time_sec time() const
		{	return _sync->time; }

		// Adding this function to enable updating sync points explicitly. Need this for gesture transition. (Yuyu, 09/06/2012)
		// Might find a better way to do this later
		void set_time(double t)
		{ _sync->time = t; }

		float offset() const
		{ return _sync->offset; }

		void set_offset(float t)
		{ _sync->offset = t; }

	protected:
		friend class BML::BehaviorSyncPoints;
		void set_sync( SyncPointPtr new_sync ) { _sync = new_sync; }
	};

	/**
	 *  The ordered, named set of SyncPoints for a single behavior.
	 *
	 *  Includes direct references to the BML standard SyncPoints.
	 */
	class BehaviorSyncPoints {
	public:
		typedef std::list<NamedSyncPointPtr> ListOfNamedSyncs;
		typedef ListOfNamedSyncs::iterator iterator;

	protected:
		ListOfNamedSyncs named_syncs;
		std::map<const std::wstring,iterator> name_to_pos;

		// Convience references to the standard/core sync points for a behavior
		iterator start_it;
		iterator ready_it;
		iterator stroke_start_it;
		iterator stroke_it;
		iterator stroke_end_it;
		iterator relax_it;
		iterator end_it;

		std::map<std::wstring, std::wstring> behaviorToSyncName;

	public:
		/**
		 * Default constructor.  Does not initialize standard SyncPoint fields.
		 */
		SBAPI BehaviorSyncPoints();

		/**
		 * Copy constructor.
		 */
		SBAPI BehaviorSyncPoints( const BehaviorSyncPoints& other );

		/**
		 *  Returns the position of the first SyncPointPtr, or end() if empty.
		 */
		BehaviorSyncPoints::iterator begin()
		{	return named_syncs.begin(); }

		/**
		 *  Returns the position after the last SyncPointPtr.
		 */
		BehaviorSyncPoints::iterator end()
		{	return named_syncs.end(); }

		size_t size() const
		{	return named_syncs.size(); }

		// Accessors for convience refences to standard/core SyncPoints
		// Returns end() is called before parseStandardSyncPoints(..).
		BehaviorSyncPoints::iterator sync_start()
		{	return BehaviorSyncPoints::iterator( start_it ); }
		BehaviorSyncPoints::iterator sync_ready()
		{	return BehaviorSyncPoints::iterator( ready_it ); }
		BehaviorSyncPoints::iterator sync_stroke_start()
		{	return BehaviorSyncPoints::iterator( stroke_start_it ); }
		BehaviorSyncPoints::iterator sync_stroke()
		{	return BehaviorSyncPoints::iterator( stroke_it ); }
		BehaviorSyncPoints::iterator sync_stroke_end()
		{	return BehaviorSyncPoints::iterator( stroke_end_it ); }
		BehaviorSyncPoints::iterator sync_relax()
		{	return BehaviorSyncPoints::iterator( relax_it ); }
		BehaviorSyncPoints::iterator sync_end()
		{	return BehaviorSyncPoints::iterator( end_it ); }

		iterator insert( const std::wstring& id, SyncPointPtr sync, BehaviorSyncPoints::iterator pos ); 

		SetOfWstring get_sync_names();

		/**
		 *  Returns the iterator refering to this SyncPoint by this name.
		 *  Returns end() if not found.
		 */
		iterator find( const std::wstring& name );

		/**
		 *  Returns the iterator refering to this SyncPoint.
		 *  Returns end() if not found.
		 */
		iterator pos_of( SyncPointPtr sync );

		/**
		 *  Returns the position of the first scheduled SyncPointPtr, or end() is none are scheduled.
		 */
		iterator first_scheduled();

		void parseStandardSyncPoints( DOMElement* elem, BmlRequestPtr request, const std::string& behavior_id );

		/**
		 *  Gets the BehaviorSpan for all scheduled SyncPoints.
		 */
		BehaviorSpan getBehaviorSpan( time_sec persistent_threshold );

#if VALIDATE_BEHAVIOR_SYNCS
		std::string debug_label( SyncPointPtr& sync );

		/** Validates the SyncPoint scheduled times are in order, if set.  Throws SchedulingException if out of order.  */
		void validate();
#endif // INCOMPLETE_SYNCS_VALIDATION

		std::wstring idForSyncPoint( SyncPointPtr sync );

		/** For each SyncPoint, if parent is set, applies the parent time and offset. */
		// Called by BehaviorRequest::schedule, not BehaviorScheduler
		void applyParentTimes( std::string& warning_context );

		/** Prints SyncPoint ids in order, separated by commas. */
		void printSyncIds();

		/** Prints SyncPoints in order, one per line, prefixed with a tab. */
		void printSyncTimes();

		SBAPI const std::map<std::wstring, std::wstring>& getBehaviorToSyncNames() const;

	protected:
		BehaviorSyncPoints::iterator parseSyncPointAttr( DOMElement* elem, const std::wstring& elem_id, const std::wstring& sync_attr, const BmlRequestPtr request, const std::string& behavior_id );
	};


	//  ostream Output Operators
	template < typename charT, typename traits >
	inline
	std::basic_ostream<charT,traits>& 
	operator << ( std::basic_ostream<charT,traits>& out, const BML::SyncPointPtr& sync ) {
		out << *(sync.get());
		return out;
	}

	template < typename charT, typename traits >
	inline
	std::basic_ostream<charT,traits>& 
	operator << ( std::basic_ostream<charT,traits>& out, const BML::SyncPoint& sync ) {
		if( sync.parent ) {
			out << "time=" << sync.time << "; parent="<< sync.parent.get() << "; offset="<<sync.offset;
		} else {
			out << "time="<<sync.time;
		}
		return out;
	}
} // end namespace BML


#endif  // BML_SYNC_POINT_HPP
