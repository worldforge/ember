#include "QuickHelp.h"

template<> EmberOgre::Gui::QuickHelp* Ember::Singleton<EmberOgre::Gui::QuickHelp>::ms_Singleton = 0;

using namespace Ember;
namespace EmberOgre
{

namespace Gui
{

QuickHelp::QuickHelp()
{
}

QuickHelp::~QuickHelp()
{
}

void QuickHelp::updateText(const std::string& text)
{
	EventUpdateText.emit(text);
}

}
}
