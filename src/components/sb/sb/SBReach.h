#ifndef _SBREACH_H_
#define _SBREACH_H_

#include <sb/SBTypes.h>
#include <string>
#include <vector>
#include <set>

namespace SmartBody {

class SBCharacter;
class SBMotion;

typedef std::pair<int,SmartBody::SBMotion*> TagMotion; // pair the motion with a tag
typedef std::set<TagMotion> MotionDataSet;

class SBReach
{
	public:
		SBAPI SBReach();
		SBAPI SBReach(SBCharacter* character);
		SBAPI ~SBReach();

		SBAPI SBCharacter* getCharacter();
		SBAPI SBReach* copy();

		SBAPI void setInterpolatorType(std::string type);
		SBAPI std::string& getInterpolatorType();
		SBAPI void addMotion(std::string type, SBMotion* motion);
		SBAPI void removeMotion(std::string type, SBMotion* motion);
		SBAPI int getNumMotions();
		SBAPI bool isPawnAttached(std::string pawnName);

		SBAPI std::vector<std::string> getMotionNames(std::string type);
		SBAPI void build(SBCharacter* character);
		
		SBAPI void setPointHandMotion(std::string type, SBMotion* pointMotion);
		SBAPI SBMotion* getPointHandMotion(std::string type);
		SBAPI void setGrabHandMotion(std::string type, SBMotion* grabMotion);
		SBAPI SBMotion* getGrabHandMotion(std::string type);
		SBAPI void setReleaseHandMotion(std::string type,SBMotion* releasebMotion);
		SBAPI SBMotion* getReleaseHandMotion(std::string type);
		SBAPI void setReachHandMotion(std::string type,SBMotion* reachMotion);
		SBAPI SBMotion* getReachHandMotion(std::string type);

		const std::set<std::pair<int,SBMotion*> >& getReachMotionDataSet() const { return reachMotionData;}

		const std::set<std::pair<int,SBMotion*> >& getPointHandData() const { return pointHandData;}
		const std::set<std::pair<int,SBMotion*> >& getGrabHandData() const { return grabHandData;}
		const std::set<std::pair<int,SBMotion*> >& getReachHandData() const { return reachHandData;}
		const std::set<std::pair<int,SBMotion*> >& getReleaseHandData() const { return releaseHandData;}
		static SBMotion* findTagMotion(int tag, const MotionDataSet& motionSet);

	protected:

		SBCharacter* _character;
		std::string interpolatorType;

		std::set<std::pair<int,SBMotion*> >      reachMotionData;
		std::set<std::pair<int,SBMotion*> >      reachHandData;
		std::set<std::pair<int,SBMotion*> >      grabHandData;
		std::set<std::pair<int,SBMotion*> >      releaseHandData;
		std::set<std::pair<int,SBMotion*> >      pointHandData;
};

}


#endif