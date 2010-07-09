#include "ActionBarInput.h"

using namespace Ember;
namespace EmberOgre
{
namespace Gui
{

ActionBarInput::ActionBarInput(const std::string& actionBarNum) : ActionBarButton("actionbar_" + actionBarNum, this, "Hotkey for ActionBar item number: " + actionBarNum + "."), mHotkey(actionBarNum)
{
}

void ActionBarInput::runCommand(const std::string &command, const std::string &args)
{
	if (ActionBarButton == command)
		EventGotHotkeyInput.emit(mHotkey);
}

}
}
