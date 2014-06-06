#include "SBAnimationTransitionRule.h"
#include <sb/SBCharacter.h>

namespace SmartBody {

SBAnimationTransitionRule::SBAnimationTransitionRule()
{
}

SBAnimationTransitionRule::~SBAnimationTransitionRule()
{
}

bool SBAnimationTransitionRule::check(SBCharacter* character, SBAnimationBlend* blend)
{
	return false;
}

}

