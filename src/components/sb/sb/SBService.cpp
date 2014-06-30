#include "SBService.h"
#include <sb/SBSubject.h>
#include <sb/SBAttribute.h>
#include <sb/SBPawn.h>
#include <sb/SBCharacter.h>

namespace SmartBody {

SBService::SBService() : SBObject()
{
	_enabled = false;

	createBoolAttribute("enable", false, true, "basic", 5, false, false, false, "Enables or disables the service.");
}

SBService::~SBService()
{
}

void SBService::setEnable(bool val)
{
	_enabled = val;
}

bool SBService::isEnable()
{
	return _enabled;
}

void SBService::notify(SBSubject* subject)
{
	SBAttribute* attribute = dynamic_cast<SBAttribute*>(subject);
	if (attribute)
	{
		if (attribute->getName() == "enable")
		{
			BoolAttribute* enableAttr = dynamic_cast<BoolAttribute*>(attribute);
			if (enableAttr)
			{
				setEnable(enableAttr->getValue());
			}
		}
	}
}

void SBService::onPawnCreate(SBPawn* pawn)
{
}

void SBService::onPawnDelete(SBPawn* pawn)
{
}

void SBService::onCharacterCreate(SBCharacter* character)
{
}

void SBService::onCharacterDelete(SBCharacter* character)
{
}


}
