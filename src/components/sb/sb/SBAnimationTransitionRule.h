#ifndef SBANIMATIONTRANSITIONRULE_H
#define SBANIMATIONTRANSITIONRULE_H

#include <sb/SBTypes.h>

namespace SmartBody {

class SBCharacter;
class SBAnimationBlend;

class SBAnimationTransitionRule
{
	public:
		SBAPI SBAnimationTransitionRule();
		SBAPI ~SBAnimationTransitionRule();

		SBAPI virtual bool check(SBCharacter* character, SBAnimationBlend* blend);

};


}

#endif