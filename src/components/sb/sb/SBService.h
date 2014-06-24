#ifndef _SBSERVICE_H_
#define _SBSERVICE_H_

#include "SBTypes.h"
#include "SBObject.h"
#ifdef __native_client__
#include "SBCharacter.h"
#endif
namespace SmartBody {

class SBSubject;
class SBPawn;

#ifndef __native_client__
class SBCharacter;
#endif

class SBService : public SBObject
{
	public:
		SBAPI SBService();
		SBAPI ~SBService();

		SBAPI virtual void setEnable(bool val);
		SBAPI virtual bool isEnable();

		SBAPI virtual void onPawnCreate(SBPawn* pawn);
		SBAPI virtual void onPawnDelete(SBPawn* pawn);
		SBAPI virtual void onCharacterCreate(SBCharacter* character);
		SBAPI virtual void onCharacterDelete(SBCharacter* character);

		SBAPI virtual void notify(SBSubject* subject);

	protected:
		bool _enabled;
};

}


#endif