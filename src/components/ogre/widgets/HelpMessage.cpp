#include "HelpMessage.h"
#include <iterator>
#include <boost/algorithm/string.hpp>

using namespace EmberOgre;

namespace EmberOgre
{
namespace Gui
{
HelpMessage::HelpMessage(const std::string &message, const std::string &caption, const std::string& tags, const std::string& id) : mMessage(message), mCaption(caption), mId(id)
{
	boost::split(mTags, tags, boost::is_any_of(" "));
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

const std::string HelpMessage::getTags() const
{
	std::string tags;
	for (std::list<std::string>::const_iterator list_iterator = mTags.begin(); list_iterator != mTags.end(); list_iterator++ )
		tags += (*list_iterator);
	return tags;
}

}
}

