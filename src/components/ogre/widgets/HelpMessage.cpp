#include "HelpMessage.h"

using namespace EmberOgre;

namespace EmberOgre
{
namespace Gui
{
HelpMessage::HelpMessage(const std::string &message, const std::string &caption, const std::string& id) : mMessage(message), mCaption(caption), mId(id)
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

bool HelpMessage::hasId() const
{
	return mId.size() != 0;
}

}
}

