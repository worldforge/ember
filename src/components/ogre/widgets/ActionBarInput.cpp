#include "ActionBarInput.h"

using namespace Ember;
namespace EmberOgre
{
namespace Gui
{

ActionBarInput::ActionBarInput() : ActionBarButton1("actionbar_1", this, "Hotkey for the first ActionBar item.")
{

}

void ActionBarInput::runCommand(const std::string &command, const std::string &args)
{
	if (ActionBarButton1 == command)
		EventGotHotkeyInput.emit("1");
}

}
}
