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

#include <framework/Service.h>
#include <framework/ConsoleObject.h>	// so this object is able to listen to console commands

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

/*
#ifdef _WIN32  // include para Windows
#include <al\al.h>
#include <al\alut.h>
#endif

#ifdef _LINUX // include para Linux
#include <AL/al.h>
#include <AL/alut.h>
#endif
*/

// #include <sigc++/object.h>

namespace dime {

/**
 * Dime Sound Service
 *
 * @author Miguel Guzman Miranda (Aglanor)
 *
 * @see dime::Service
 * @see dime::ConsoleObject
 */
class SoundService: public Service, public ConsoleObject
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:

	ALCdevice *device;  // device for the Audio Layer Context



	/** System source - this source will play system sounds, like user input request or program error. Will always remain in the same relative position to the listener. */
	static ALuint systemSource;
	/** System buffer - buffer used to load system sounds files */
	static ALuint systemBuffer;
	/** Music source - this source will play background music. Will always remain in the same relative position to the listener. */
	static ALuint musicSource;
	/** Music buffer - buffer used to load background music files */
	static ALuint musicBuffer;
	/** World sources - array of sources to play world sounds. They will be placed in 3D space. This field may change depending on the data model */
	ALuint worldSources[NUM_WORLD_SOURCES];
	/** Wold buffers - array of buffers for loading world sounds */
	ALuint worldBuffers[NUM_WORLD_BUFFERS];


	ALsizei size,freq,bits,format;
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

	ALboolean LoadWAV(const char *fname,int buffer);

	ALboolean UnloadWAV(void);

	void TestPlatform(void);

	// List of SoundService's console commands
	static const char * const PLAYSOUND = "playsound";
	static const char * const PLAYMUSIC = "playmusic";


}; //SoundService

} // namespace dime

#endif
