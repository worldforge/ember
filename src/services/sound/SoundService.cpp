/*
  Copyright (C) 2002 Miguel Guzman Miranda (Aglanor)
  Code based on the Woldspace OpenAL tutorials by Lord Loki
  	at http://worldspace.berlios.de/openal/

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

#include "SoundService.h"
#include "services/logging/LoggingService.h"
#include "services/gui/Console.h"
#include "services/gui/Tokeniser.h"

/*
#include <list>
#include <algorithm>
#include <iostream>
*/

#ifdef _WIN32  // include para Windows
#include <al\al.h>
#include <al\alut.h>
#endif

#ifdef _LINUX // include para Linux
#include <AL/al.h>
#include <AL/alut.h>
#endif






namespace dime
{

  /* ctor */
  SoundService::SoundService()
  {
  }

  /* dtor */
  SoundService::~SoundService()
  {

  }

	/* Method for starting this service 	*/
	Service::Status SoundService::start()
	{

		ALfloat listenerPos[3]={0.0,0.0,0.0};						// listener position
		ALfloat listenerVel[3]={0.0,0.0,0.0};							// listener velocity
		ALfloat listenerOri[6]={0.0,0.0,1.0,0.0,1.0,0.0};		// listener orientation

		ALfloat sourcePos[3]={ 0.0, 10.0, 0.0};						// source position
		ALfloat sourceVel[3]={ 0.0, 0.0, 1.0};						// source velocity

		data=NULL;

		// Initialize OpenAL
		alutInit(NULL,0);

		// Generate buffers
		alGenBuffers(NUM_BUFFERS,myBuffers);

		for (int i=0;i<NUM_BUFFERS;i++)
		{
			if (!alIsBuffer(myBuffers[i]))
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error creating buffers" << ENDM;
				return Service::FAILURE;
			}
		}

		// Generate sources
		alGenSources(NUM_SOURCES,mySources);

		for (int i=0;i<NUM_SOURCES;i++)
		{
			if (!alIsSource(mySources[i]))
			{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error creating sources" << ENDM;
				return Service::FAILURE;
			}
		}


		TestPlatform();  // a test

		alutLoadWAV("boom.wav",&data,&format,&size,&bits,&freq);		// Load WAV file  // Should be LoadWAV, platform independant

		if(alGetError() != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error loading wav" << ENDM;
			return Service::FAILURE;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "WAV file loaded"  << ENDM;
		}


	LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Loaded WAV file? let's check"  << ENDM;

	LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "format: " << format << ENDM;

	LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "size: " << size << ENDM;

	LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "bits: " << bits << ENDM;

	LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "freq: " << freq << ENDM;

		alBufferData(myBuffers[0],format,data,size,freq);				// Connect WAV to buffer

		if(alGetError() != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error connecting wav to buffer" << ENDM;
			return Service::FAILURE;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "WAV connected to buffer"  << ENDM;
		}


	free (data);  // should be UnloadWAV, platform independant

	//	 UnloadWAV();

		// set listener initial parameters
		alListenerfv(AL_POSITION,listenerPos);			// position
		alListenerfv(AL_VELOCITY,listenerVel);			// velocity
		alListenerfv(AL_ORIENTATION,listenerOri);		// orientation

		int error = alGetError();
			if(error != AL_NO_ERROR)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error in listener parameters: " << error << ENDM;
			} else {
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Listener Parameters OK" << ENDM;
			}

		alSourcef(mySources[0],AL_PITCH,1.0f); 					// source Frequency

		error = alGetError();
			if(error != AL_NO_ERROR)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error in source parameter pitch: " << error << ENDM;
			} else {
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Source parameter pitch OK" << ENDM;
			}

		alSourcef(mySources[0],AL_GAIN,1.0f);						// source gain

		error = alGetError();
			if(error != AL_NO_ERROR)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error in source parameter gain: " << error << ENDM;
			} else {
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Source parameter gain OK" << ENDM;
			}



		alSourcefv(mySources[0],AL_POSITION,sourcePos);	// source position

		error = alGetError();
			if(error != AL_NO_ERROR)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error in source parameter position: " << error << ENDM;
			} else {
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Source parameter position OK" << ENDM;
			}



		alSourcefv(mySources[0],AL_VELOCITY,sourceVel);	// source velocity
		error = alGetError();
			if(error != AL_NO_ERROR)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error in source parameter velocity: " << error << ENDM;
			} else {
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Source parameter velocity OK" << ENDM;
			}


		error = alGetError();
			if(error != AL_NO_ERROR)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error in source parameters: " << error << ENDM;
			} else {
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Source Parameters OK" << ENDM;
			}

		alSourcei(mySources[0],AL_LOOPING,AL_FALSE);		// looping play

		error = alGetError();
			if(error != AL_NO_ERROR)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error in source parameter looping: " << error << ENDM;
			} else {
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Source parameter looping OK" << ENDM;
			}



		alSourcei(mySources[0],AL_BUFFER,myBuffers[0]);		// link the source to the buffer

		error = alGetError();
			if(error != AL_NO_ERROR)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error attaching buffer to source: " << error << ENDM;
			} else {
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Buffer attached OK" << ENDM;
			}


		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Press 1 to play sound, 2 to stop, 3 to go on with dime." << ENDM;

		/*
		char a; bool loop = true;
	do {
		cin >> a;
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "a: " << a << ENDM;
 		switch (a)
		{
			case '1':
				alSourcePlay(mySources[0]);
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "playing"<< ENDM;
				break;
			case '2':
				alSourceStop(mySources[0]);
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "stopping" << ENDM;
				break;
			case '3':
				loop = false;
				break;
		}
			error = alGetError();
			if(error != AL_NO_ERROR)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error: " << error << ENDM;
			}

	} while (loop);
*/
		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error playing wav: " << error << ENDM;

			return Service::FAILURE;
		}

	alSourceStop(mySources[0]);




		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Sound Service initialized" << ENDM;

		setStatus(Service::OK);
		setRunning( true );

		return Service::OK;

  }

	/* Interface method for stopping this service 	*/
	void SoundService::stop(int code)
	{
		alutExit();		// Finalize OpenAL
		setStatus(Service::OK);
		setRunning( false );
	}

  void SoundService::runCommand(const std::string &command, const std::string &args)
  {

  }

	ALboolean SoundService::LoadWAV(const char *fname,int buffer)
	{

#ifdef _WIN32 // Windows

		alutLoadWAVFile(fname,&format,&data,&size,&freq); 	// Load WAV file
		alBufferData(buffer,format,data,size,freq); 			// Connect WAV to buffer

#endif

#ifdef _LINUX // Linux

		alutLoadWAV(fname,&data,&format,&size,&bits,&freq);		// Load WAV file
		alBufferData(buffer,format,data,size,freq);				// Connect WAV to buffer
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Loading WAV and stuff" << ENDM;

#endif

	return AL_TRUE;

	}

	ALboolean SoundService::UnloadWAV(void)
	{

#ifdef _WIN32 //Windows

		alutUnloadWAV(format,data,size,freq); // Funtion that unloads the Wav file on windows

#endif

#ifdef _LINUX // Linux

		free (data); // Free the memory used when loading the WAV file

#endif

	return AL_TRUE;

	}

	void SoundService::TestPlatform(void)
	{

		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Testing Platform" << ENDM;

#ifdef _WIN32 //Windows

		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Windows Platform found" << ENDM;

#endif

#ifdef _LINUX // Linux

		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Linux Platform found" << ENDM;

#endif


	}



} // namespace dime
