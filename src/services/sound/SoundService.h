/*
    Copyright (C) 2002  Miguel Guzman Miranda (Aglanor)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H

#define NUM_WORLD_SOURCES 16
#define NUM_WORLD_BUFFERS 16


class Service;
//class ConsoleObject;
//I only break my own rules for very good reasons
#include "framework/ConsoleObject.h"

namespace Ember {

/**
 * Ember Sound Service
 *
 * @author Miguel Guzman Miranda (Aglanor)
 *
 * @see Ember::Service
 * @see Ember::ConsoleObject
 */
class SoundService: public Service, public ConsoleObject
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:

  //ALCdevice *device;  // device for the Audio Layer Context
  //this should be a void* if it is enabled again.  It is not currently used



	/** System source - this source will play system sounds, 
	like user input request or program error. 
	Will always remain in the same relative position to the listener. */
	static unsigned int systemSource;
	/** System buffer - buffer used to load system sounds files */
	static unsigned int systemBuffer;
	/** Music source - this source will play background music. 
	Will always remain in the same relative position to the listener. */
	static unsigned int musicSource;
	/** Music buffer - buffer used to load background music files */
	static unsigned int musicBuffer;
	/** World sources - array of sources to play world sounds. 
	They will be placed in 3D space. 
	This field may change depending on the data model */
	unsigned int worldSources[NUM_WORLD_SOURCES];
	/** Wold buffers - array of buffers for loading world sounds */
	unsigned int worldBuffers[NUM_WORLD_BUFFERS];


	std::string soundsDirPath;
	
	int size,freq,bits,format;
	void *data;


    //----------------------------------------------------------------------
    // Constructors & Destructor

  public:

    /** Creates a new SoundService using default values. */
    SoundService();


    /** Deletes a SoundService instance. */
    ~SoundService();

    //----------------------------------------------------------------------
    // Getters & Setters

    //----------------------------------------------------------------------
    // Methods

	Service::Status start();

	void stop(int code);

	void runCommand(const std::string &command, const std::string &args);

	bool LoadWAV(const char *fname,int buffer);

	bool UnloadWAV(void);

	void TestPlatform(void);

	void playTestGYPH(void);
	
	// List of SoundService's console commands
	static const char * const PLAYSOUND;
	static const char * const PLAYMUSIC;


}; //SoundService

} // namespace Ember

#endif
