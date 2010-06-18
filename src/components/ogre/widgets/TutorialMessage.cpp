#include "TutorialMessage.h"

using namespace EmberOgre;

namespace EmberOgre
{
namespace Gui
{
TutorialMessage::TutorialMessage(const std::string &message, const std::string &caption, const std::string& Id) : mMessage(message), mCaption(caption), mId(Id)
{
}

TutorialMessage::~TutorialMessage()
{
}

void TutorialMessage::setHelp(const std::string &message)
{
	mMessage = message;
}

const std::string& TutorialMessage::getHelp() const
{
	return mMessage;
}

const std::string& TutorialMessage::getId() const
{
	return mId;
}

}
}

