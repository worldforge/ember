#ifndef EMBEROGRE_QUICKHELP_H
#define EMBEROGRE_QUICKHELP_H

#include "framework/Singleton.h"
#include <sigc++/signal.h>
#include <string>

namespace EmberOgre {

namespace Gui {

class QuickHelp : public Ember::Singleton<QuickHelp>
{
public:
	QuickHelp();
	virtual ~QuickHelp();
	void updateText(const std::string& text);

	sigc::signal <void,const std::string&> EventUpdateText;
private:


};

}
}


#endif
