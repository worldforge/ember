#include "TutorialMessage.h"

using namespace EmberOgre;

namespace EmberOgre
{
namespace Gui
{
TutorialMessage::TutorialMessage(const std::string &message, const std::string &caption, short int Id) : mMessage(message), mCaption(caption), mId(Id)
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

const short int TutorialMessage::getId() const
{
	return mId;
}

}
}

