#ifndef _SBSPEECHMANAGER_H_
#define _SBSPEECHMANAGER_H_

#include <sb/SBTypes.h>
#include <sb/SBObject.h>

class remote_speech;
class local_speech;
class text_speech;
class FestivalSpeechRelayLocal;
class CereprocSpeechRelayLocal;

namespace  SmartBody 
{
	class AudioFileSpeech;
}

namespace SmartBody {

class SBSpeechManager : public SBObject
{
	public:
		SBAPI SBSpeechManager();
		SBAPI ~SBSpeechManager();

		SBAPI FestivalSpeechRelayLocal* festivalRelay();
		SBAPI CereprocSpeechRelayLocal* cereprocRelay();
		SBAPI remote_speech* speech_rvoice();
		SBAPI local_speech* speech_localvoice();
		SBAPI SmartBody::AudioFileSpeech* speech_audiofile();
		SBAPI text_speech* speech_text();

	protected:
		remote_speech* _speech_rvoice;
		local_speech* _speech_localvoice;
		SmartBody::AudioFileSpeech* _speech_audiofile;
		text_speech* _speech_text;
		FestivalSpeechRelayLocal* _festivalRelayLocal; 
		CereprocSpeechRelayLocal* _cereprocRelayLocal;
};

}

#endif