#include "HelpMessage.h"

using namespace EmberOgre;

namespace EmberOgre
{
namespace Gui
{
HelpMessage::HelpMessage(const std::string &message, const std::string &caption, const std::string& Id) : mMessage(message), mCaption(caption), mId(Id)
{
}

HelpMessage::~HelpMessage()
{
}

void HelpMessage::setHelp(const std::string &message)
{
	mMessage = message;
}

const std::string& HelpMessage::getHelp() const
{
	return mMessage;
}

const std::string& HelpMessage::getId() const
{
	return mId;
}

}
}

