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

	ALfloat listenerPos[]={0.0,0.0,0.0};						// listener position
	ALfloat listenerVel[3]={0.0,0.0,0.0};							// listener velocity
	ALfloat listenerOri[6]={0.0,0.0,1.0,0.0,1.0,0.0};		// listener orientation

	ALfloat sourcePos[3]={ 0.0, 0.0, 1.0};						// source position
	ALfloat sourceVel[3]={ 0.0, 0.0, 1.0};						// source velocity

	data=NULL;

		// Initialize OpenAL
		alutInit(NULL,0);
		alGenBuffers(NUM_BUFFERS,myBuffers);

		for (int i=0;i<NUM_BUFFERS;i++)
		{
			if (!alIsBuffer(myBuffers[i]))
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error creating buffers" << ENDM;
				return Service::FAILURE;
			}
		}
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "trace1" << ENDM;



		TestPlatform();
					LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "trace2" << ENDM;

		alutLoadWAV("boomboom.wav",&data,&format,&size,&bits,&freq);		// Load WAV file
					LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "trace3" << ENDM;


		//alBufferData(myBuffers[0],format,data,size,freq);				// Connect WAV to buffer
		/*if(alGetError() != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error reading wav" << ENDM;
			return Service::FAILURE;
		}
*/
	//	 UnloadWAV();

		// set listener initial parameters
		alListenerfv(AL_POSITION,listenerPos); //posicion
		alListenerfv(AL_VELOCITY,listenerVel); //velocidad
		alListenerfv(AL_ORIENTATION,listenerOri); //orientación

alSourcef(mySources[0],AL_PITCH,1.0f);  // Con esto podemos disminuir la frecuencia
alSourcef(mySources[0],AL_GAIN,1.0f); // Con esto definimos la ganancia en amplitud ( intensidad de sonido)
alSourcefv(mySources[0],AL_POSITION,sourcePos); // posición
alSourcefv(mySources[0],AL_VELOCITY,sourceVel); // velocidad
alSourcei(mySources[0],AL_BUFFER,myBuffers[0]); // Con esto pasamos el archivo wav del buffer a la fuente
alSourcei(mySources[0],AL_LOOPING,AL_FALSE); // Con esto le indicamos que la fuente no se reproduzca una y otra vez

		alSourcePlay(mySources[0]);

		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Sound Service initialized" << ENDM;

		setStatus(Service::OK);
		setRunning( true );

		return Service::OK;

  }

  /* Interface method for stopping this service 	*/
  void SoundService::stop(int code)
  {
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
