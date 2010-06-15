#ifndef EMBEROGRE_TUTORIALMESSAGE_H
#define EMBEROGRE_TUTORIALMESSAGE_H

#include <string>

namespace EmberOgre
{

namespace Gui
{

class TutorialMessage
{
public:
	TutorialMessage(const std::string &message);
	~TutorialMessage();

	void setHelp(const std::string &message);
	const std::string& getHelp();

private:
	std::string mMessage;
};
}
}

#endif
