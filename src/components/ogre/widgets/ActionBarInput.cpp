#include "ActionBarInput.h"

using namespace Ember;
namespace EmberOgre
{
namespace Gui
{

ActionBarInput::ActionBarInput(const std::string& actionBarKey) : ActionBarButton("actionbar_" + actionBarKey, this, "Hotkey for ActionBar key: " + actionBarKey + "."), mHotkey(actionBarKey)
{
}

void ActionBarInput::runCommand(const std::string &command, const std::string &args)
{
	if (ActionBarButton == command)
		EventGotHotkeyInput.emit(mHotkey);
}

ActionBarInput::~ActionBarInput()
{
}

}
}
