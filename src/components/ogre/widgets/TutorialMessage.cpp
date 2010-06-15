#include "TutorialMessage.h"

using namespace EmberOgre;

namespace EmberOgre
{
namespace Gui
{
TutorialMessage::TutorialMessage(const std::string &message)
{
	mMessage = message;
}

TutorialMessage::~TutorialMessage()
{
}

void TutorialMessage::setHelp(const std::string &message)
{
	mMessage = message;
}

const std::string& TutorialMessage::getHelp()
{
	return mMessage;
}

}
}

