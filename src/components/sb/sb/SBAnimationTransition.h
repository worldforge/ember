#ifndef _SBTRANSITION_H_
#define _SBTRANSITION_H_

#include <sb/SBTypes.h>
#include <vector>
#include <sb/PABlend.h>
#include <sb/SBObject.h>

class PABlend;

namespace SmartBody {

class SBAnimationBlend;
class SBAnimationTransitionRule;

class SBAnimationTransition : public SBObject
{
	public:
		SBAPI SBAnimationTransition();
		SBAPI SBAnimationTransition(std::string name);
		SBAPI SBAnimationTransition(SBAnimationTransition* transition, PABlend* from, PABlend* to);
		SBAPI ~SBAnimationTransition();

		SBAPI void setSourceBlend(SBAnimationBlend* source);
		SBAPI void setDestinationBlend(SBAnimationBlend* dest);
		SBAPI SBAnimationBlend* getSourceBlend();
		SBAPI SBAnimationBlend* getDestinationBlend();

		SBAPI void set(SBAnimationBlend* source, SBAnimationBlend* dest);
		SBAPI void setEaseInStart(double val);
		SBAPI void setEaseInEnd(double val);
		SBAPI double getEaseInStart();
		SBAPI double getEaseInEnd();
		SBAPI std::vector<double>& getEaseOutStart();
		SBAPI std::vector<double>& getEaseOutEnd();
		SBAPI void setEaseInInterval(std::string destMotion, float start, float end);
		SBAPI void addEaseOutInterval(std::string sourceMotion, float start, float end);
		SBAPI const std::string& getSourceMotionName();
		SBAPI const std::string& getDestinationMotionName();
		SBAPI void removeEaseOutInterval(int num);
		SBAPI int getNumEaseOutIntervals();
		SBAPI std::vector<double> getEaseOutInterval(int num);
		SBAPI SBAnimationTransitionRule* getTransitionRule();
		SBAPI void setTransitionRule(SBAnimationTransitionRule* rule);
		SBAPI std::string getTransitionName();
		SBAPI std::string saveToString();

	protected:
		SBAnimationTransitionRule* _rule;
		
		PABlend* fromState;
		PABlend* toState;
		std::string fromMotionName;
		std::string toMotionName;
		std::vector<double> easeOutStart;
		std::vector<double> easeOutEnd;
		double easeInStart;
		double easeInEnd;

};

}
#endif