#include "SBScript.h"
#include <sb/SBAttribute.h>
#include <sb/SBScene.h>

namespace SmartBody {

SBScript::SBScript() : SBAsset()
{
	this->createBoolAttribute("enable", true, true, "Script", 10, false, false, false, "Whether this event is enabled.");
	_enabled = true;
}

SBScript::~SBScript()
{
}

void SBScript::setEnable(bool val)
{
	_enabled = val;
	BoolAttribute* boolAttribute = dynamic_cast<BoolAttribute*>(this->getAttribute("enable"));
	boolAttribute->setValueFast(val);
}

bool SBScript::isEnable()
{
	return _enabled;
}

void SBScript::notify(SBSubject* subject)
{
	SBAttribute* attribute = dynamic_cast<SBAttribute*>(subject);
	if (attribute)
	{
		if (attribute->getName() == "enable")
		{
			BoolAttribute* boolAttr = dynamic_cast<BoolAttribute*>(attribute);
			if (boolAttr)
			{
				this->setEnable(boolAttr->getValue());
			}
		}
	}
}
}
