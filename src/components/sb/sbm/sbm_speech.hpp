/*
 *  sbm_speech.hpp - part of SmartBody-lib
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
 *      Ashok Basawapatna, USC (no longer)
 *      Ed Fast, USC
 *      Corne Versloot, Univ of Twente (while at USC)
 */

# ifndef SBM_AUDIO_HPP
# define SBM_AUDIO_HPP

# include <vector>
# include <iostream>

# include "xercesc_utils.hpp"


// Forward declarations
class SbmCharacter;


namespace SmartBody {
	// typedef
	typedef int RequestId;


	/**
	*  interface for viseme data.
	*/
	class VisemeData {
	private:
		std::string _id;
		float _weight;
		float _time;
		float _duration;
		int		_numKeys;
		std::string	_curveInfo;
		bool _curveMode;
		bool _trapezoidMode;
		bool _floatCurveMode;
		bool _motionMode;
		float _rampin;
		float _rampout;
		std::vector<float> _curveData;
		int _floatsPerKey;

	public:
		VisemeData( const std::string& id)
			: _id( id ), _weight( 1.0f ), _time(0.0f), _duration( 0.0f ), _rampin(0.0f), _rampout(0.0f),  _numKeys(0), _floatsPerKey(0), _curveMode(false), _trapezoidMode(false), _floatCurveMode(false), _motionMode(true)
		{
		}
		VisemeData( const std::string& id, float weight, float time )
			: _id( id ), _weight( weight ), _time( time ), _duration( 0 ),  _rampin(0), _rampout(0), _numKeys( 0 ), _curveInfo( "" ), _curveMode(false), _trapezoidMode(false), _floatCurveMode(false), _motionMode(false)
		{
		}
		
		VisemeData( const std::string& id, float weight, float time, float duration )
			: _id( id ), _weight( weight ), _time( time ), _duration( duration ), _rampin(0), _rampout(0), _numKeys( 0 ), _curveInfo( "" ), _curveMode(false), _trapezoidMode(false), _floatCurveMode(false), _motionMode(false)
		{
		}

		VisemeData( const std::string& id, int numKeys, const std::string& curveInfo );

		VisemeData( const std::string& id, float startTime)
			: _id( id ), _weight( 1.0f ), _time( startTime ), _duration( 0.0f ), _rampin(0.0f), _rampout(0.0f),  _numKeys(0), _floatsPerKey(0), _curveMode(false), _trapezoidMode(false), _floatCurveMode(true), _motionMode(false)
		{
		}

		VisemeData( const std::string& id, float weight, float time, float duration, float rampin, float rampout)
			: _id( id ), _weight( weight ), _time( time ), _duration( duration ), _rampin(rampin), _rampout(rampout), _numKeys( 0 ), _curveInfo( "" ), _trapezoidMode(true), _curveMode(false), _floatCurveMode(false), _motionMode(false)
		{
		}

		virtual ~VisemeData()
		{
		}


		/** Return the viseme identifier/name.  */
		const char * id() const { return _id.c_str(); }

		/** Return the weight of the viseme. */
		float weight() const { return _weight; }

		/** Return the audio relative time to trigger this viseme. */
		float time() const { return _time; }
		
		/** Return the blend-in duration of the viseme. */
		float duration() const { return _duration; }

		/** Return the ramp-in of the viseme. */
		float rampin() const { return _rampin; }

		/** Return the ramp-out duration of the viseme. */
		float rampout() const { return _rampout; }

		/** Sets the ramp-in of the viseme. */
		void rampin(float val) { _rampin = val; }

		/** Sets the ramp-out duration of the viseme. */
		void rampout(float val) { _rampout = val; }

		/** Set the audio relative time to trigger this viseme. */
		void setTime( float time ) { _time = time; }

		/** Set the blend-in duration of the viseme. */
		void setDuration( float duration ) { _duration = duration; }

		/** Set the blend-in duration of the viseme. */
		void setRampout( float rampout ) { _rampout = rampout; }

		/** Set the blend-in duration of the viseme. */
		void setRampin( float rampin ) { _rampin = rampin; }

		/** Get the number of keys. */
		int getNumKeys() {return _numKeys;}

		/** Get the curve information. */
		const char* getCurveInfo() {return _curveInfo.c_str();}

		/** Set the curve information. */
		void setCurveInfo(const char* info) { _curveInfo = info;}

		/** Get the viseme Mode. */
		bool isMotionMode()	{return _motionMode;}

		/** Set the viseme Mode. */
		void setMotionMode(bool val) {_motionMode = val;}

		/** Get the viseme Mode. */
		bool isCurveMode() {return _curveMode;}

		/** Set the viseme Mode. */
		void setCurveMode(bool val) {_curveMode = val; _trapezoidMode = !val; _floatCurveMode = !val;}

		/** Get the viseme Mode. */
		bool isTrapezoidMode() {return _trapezoidMode;}

		/** Set the viseme Mode. */
		void setTrapezoidMode(bool val) {_curveMode = !val; _trapezoidMode = val; _floatCurveMode = !val;}

		/** Get the viseme Mode. */
		bool isFloatCurveMode() {return _floatCurveMode;}

		/** Set the viseme Mode. */
		void setFloatCurveMode(bool val) {_curveMode = !val; _trapezoidMode = !val; _floatCurveMode = val;}

		void setFloatCurve(std::vector<float>& curve, int numKeys, int floatsPerKey) {
			_curveData.clear();
			_numKeys = numKeys;
			_floatsPerKey = floatsPerKey;
			for (int x = 0; x < numKeys; x++)
			{
				_curveData.push_back(curve[x * floatsPerKey]);
				_curveData.push_back(curve[x * floatsPerKey + 1]);
				for (int y = 2; y < floatsPerKey; y++)
					_curveData.push_back(0.0f);
			}
		}

		void calculateDuration() {
			// find the last key
			if (!isFloatCurveMode())
				return;
			int index = (_numKeys - 1) * _floatsPerKey;
			_duration = _curveData[index];
		}

		/** Get the number of keys. */
		int getFloatsPerKey() {return _floatsPerKey;}

		std::vector<float>& getFloatCurve() { return _curveData; }
	};

	/**
	 *  Write VisemeData to Stream
	 */
	std::ostream& operator << ( std::ostream& strm, const VisemeData& v );

    /**
     *  Abstract interface to speech synthesizers and speech audio.
     *
     *  Speech requests evaluate in four steps:
     *  1. A request is made with one of the given requestSpeechAudio() methods
     *  2. The speech implementation sends out a SBM result message of 
     *     the form: <callbackCmd> <agentId> <requestId> SUCCESS
     *           or:  <callbackCmd> <agentId> <requestId> ERROR <msg>
     *  3. Up reciept of the callbackMsg, the client can query result data via
     *     any of the get*(RequestId) methods.
     *  4. When complete, the client should call requestComplete(RequestId).
     *
     *  Each of the get*(RequestId) methods return a pointer to an object.
     *  This object belongs to the SpeechInterface and should not be deleted by 
     *  SpeechInterface clients.  The SpeechInterface implementation can delete 
     *  the object on a call to requestComplete(..), or in its destructor.
     *
     *  If any get*(RequestId) method has been called on an invalid RequestId
     *  or prior to the availability of the data, the method should return NULL.
     */
    class SpeechInterface {
    public:
        /**
         *  Requests audio for a speech by agentName as specified in XML node.
         *  If node is a DOMElement, use the child nodes.
         *  If node is a text node, is the value a string.
         *  If node is an attribute, use the attribute value as a string.
         *  Returns a unique request identifier.
         */
		// TODO: Define error return value as a constant somewhere (or new exception type).
        virtual RequestId requestSpeechAudio( const char* agentName, const std::string voiceCode, const DOMNode* node, const char* callbackCmd ) = 0;

        /**
         *  Requests audio for a speech char[] text by agentName.
         *  Returns a unique request identifier.
         */
		// TODO: Define error return value as a constant somewhere (or new exception type).
		virtual RequestId requestSpeechAudio( const char* agentName, const std::string voiceCode, std::string text, const char* callbackCmd ) = 0;

        /**
         *  If the request has been processed, returns the time ordered vector 
         *  of VisemeData for the requestId.  Otherwise return NULL.
         *
         *  Visemes in this list are actually morph targets, and multiple
         *  visemes with different weights can be added together.  Because of
         *  this, the returned viseme list should include zero weighted
         *  VisemeData instances of when to cancel previous visemes (change
         *  of viseme, and end of words).
         */
        virtual std::vector<VisemeData *>* getVisemes( RequestId requestId, SbmCharacter* character ) = 0;

		/**
		 *	Returns the emotion curve for given speech request.
		 */
		virtual std::vector<float> getEmotionCurve(RequestId requestId, const std::string& emotionType, SbmCharacter* character = NULL) = 0;

		/**
		 *	Returns number of emotions for given speech request.
		 */
		virtual std::vector<std::string> getEmotionNames(RequestId requestId, SbmCharacter* character = NULL) = 0;

        /**
         *  Returns the sbm command used to play the speech audio.
         */
        virtual char* getSpeechPlayCommand( RequestId requestId, SbmCharacter* character = NULL ) = 0;

        /**
         *  Returns the sbm command used to stop the speech audio.
         */
        virtual char* getSpeechStopCommand( RequestId requestId, SbmCharacter* character = NULL ) = 0;

        /**
         *  Returns the filename of the audio.
         */
        virtual char* getSpeechAudioFilename( RequestId requestId ) = 0;

        /**
         *  Returns the timing for a synthesis bookmark,
         *  or -1 if the markId is not recognized.
         */
        virtual float getMarkTime( RequestId requestId, const XMLCh* markId ) = 0;

        /**
         *  Signals that requestId processing is complete and its data can be 
         *  discarded.  May be called (...?)
         */
        virtual void requestComplete( RequestId requestId ) = 0;

    };
}  // end namespace SmartBody

# endif // SBM_AUDIO_HPP
