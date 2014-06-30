#ifndef _SBSTATEMANAGER_H
#define _SBSTATEMANAGER_H

#include <sb/SBTypes.h>
#include <string>
#include <vector>
#include <sr/sr_vec.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#undef INTMAX_C
#undef UINTMAX_C


namespace SmartBody {

class SBAnimationBlend;
class SBAnimationBlend0D;
class SBAnimationBlend1D;
class SBAnimationBlend2D;
class SBAnimationBlend3D;
class SBMotionBlendBase;
class SBAnimationTransition;

typedef boost::property<boost::vertex_name_t,std::string> StateVertexProperty;
typedef boost::labeled_graph<boost::adjacency_list<boost::listS,boost::listS, boost::directedS, StateVertexProperty>,std::string> BoostGraph;

class SBAnimationBlendManager
{
	public:
		SBAPI SBAnimationBlendManager();
		SBAPI ~SBAnimationBlendManager();

		SBAPI SBAnimationBlend0D* createBlend0D(const std::string& name);
		SBAPI SBAnimationBlend1D* createBlend1D(const std::string& name);
		SBAPI SBAnimationBlend2D* createBlend2D(const std::string& name);
		SBAPI SBAnimationBlend3D* createBlend3D(const std::string& name);
		SBAPI SBMotionBlendBase*  createMotionBlendBase(const std::string& name, const std::string& skelName, int dimension);
		SBAPI SBAnimationTransition* createTransition(const std::string& source, const std::string& dest);

		SBAPI SBAnimationBlend* getBlend(const std::string&name);
		SBAPI int getNumBlends();
		SBAPI std::vector<std::string> getBlendNames();

		SBAPI SBAnimationTransition* getTransition(const std::string& source, const std::string& dest);
		SBAPI SBAnimationTransition* getTransitionByName(const std::string& transitionName);
		SBAPI SBAnimationTransition* getTransitionByIndex(int id);
		SBAPI int getNumTransitions();
		SBAPI std::vector<std::string> getTransitionNames();
		SBAPI std::vector<std::string> getTransitionBlends(const std::string& source);

		SBAPI std::string getCurrentBlend(const std::string& characterName);
		SBAPI SrVec getCurrentBlendParameters(const std::string& characterName);
		SBAPI std::vector<std::string> getAutoBlendTransitions(const std::string& characterName, const std::string& targetBlend);
		SBAPI bool isBlendScheduled(const std::string& characterName, const std::string& blendName);

		SBAPI void removeAllBlends();
		SBAPI void removeAllTransitions();

protected:
		bool addBlendToGraph(const std::string& name);
		bool addTransitionEdgeToGraph(const std::string& source, const std::string& dest);

		BoostGraph stateGraph;		

		std::vector<SBAnimationBlend*> _blends;
		std::vector<SmartBody::SBAnimationTransition*> _transitions;
};

}

#endif