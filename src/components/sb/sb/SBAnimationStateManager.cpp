#include "SBAnimationStateManager.h"

#include <sb/SBAnimationState.h>
#include <sb/SBMotionBlendBase.h>
#include <sb/SBAnimationTransition.h>
#include <sb/SBCharacter.h>
#include <sb/SBScene.h>
#include <sb/SBSceneListener.h>
#include <controllers/me_ct_param_animation.h>

namespace SmartBody {

SBAnimationBlendManager::SBAnimationBlendManager()
{
}

SBAnimationBlendManager::~SBAnimationBlendManager()
{
}

std::vector<std::string> SBAnimationBlendManager::getAutoBlendTransitions( const std::string& characterName, const std::string& targetBlend )
{
	std::vector<std::string> pathVec;
	

	return pathVec;		
}

bool SBAnimationBlendManager::addBlendToGraph( const std::string& name )
{
	BoostGraph::vertex_descriptor v = stateGraph.vertex(name);
	if (v == BoostGraph::null_vertex()) // the state does not exist in the graph
	{		
		stateGraph.add_vertex(name);	
		//boost::put(boost::get(boost::vertex_name_t,stateGraph),stateGraph,name,name);
		//boost::put(boost::vertex_name_t,)
		
	}
	return true;
}

bool SBAnimationBlendManager::addTransitionEdgeToGraph( const std::string& source, const std::string& dest )
{
	BoostGraph::vertex_descriptor vs = stateGraph.vertex(source), vd = stateGraph.vertex(dest);
	
	if (vs == BoostGraph::null_vertex() || vd == BoostGraph::null_vertex()) return false;
	std::pair<BoostGraph::edge_descriptor,bool> transitionEdge = boost::edge_by_label(source,dest,stateGraph);
	// edge already exist
	if (transitionEdge.second) return false;

	// otherwise add transition edge
	boost::add_edge_by_label(source,dest,stateGraph); return true;
}

SBAnimationBlend0D* SBAnimationBlendManager::createBlend0D(const std::string& name)
{
#if __FLASHPLAYER__
	return NULL;
#endif

	SBAnimationBlend0D* blend = new SBAnimationBlend0D(name);

	addBlendToGraph(name);
	_blends.push_back(blend);

	std::vector<SBSceneListener*>& listeners = SmartBody::SBScene::getScene()->getSceneListeners();
	for (size_t l = 0; l < listeners.size(); l++)
	{
		listeners[l]->OnObjectCreate(blend);
	}
	return blend;
}

SBAnimationBlend1D* SBAnimationBlendManager::createBlend1D(const std::string& name)
{
#if __FLASHPLAYER__
	return NULL;
#endif

	SBAnimationBlend1D* blend = new SBAnimationBlend1D(name);
	
	addBlendToGraph(name);
	_blends.push_back(blend);
	
	std::vector<SBSceneListener*>& listeners = SmartBody::SBScene::getScene()->getSceneListeners();
	for (size_t l = 0; l < listeners.size(); l++)
	{
		listeners[l]->OnObjectCreate(blend);
	}
	return blend;
}

SBAnimationBlend2D* SBAnimationBlendManager::createBlend2D(const std::string& name)
{
#if __FLASHPLAYER__
	return NULL;
#endif

	SBAnimationBlend2D* blend = new SBAnimationBlend2D(name);
	
	addBlendToGraph(name);
	_blends.push_back(blend);

	std::vector<SBSceneListener*>& listeners = SmartBody::SBScene::getScene()->getSceneListeners();
	for (size_t l = 0; l < listeners.size(); l++)
	{
		listeners[l]->OnObjectCreate(blend);
	}

	return blend;
}

SBAnimationBlend3D* SBAnimationBlendManager::createBlend3D(const std::string& name)
{
#if __FLASHPLAYER__
	return NULL;
#endif

	SBAnimationBlend3D* blend = new SBAnimationBlend3D(name);
	
	addBlendToGraph(name);
	_blends.push_back(blend);

	std::vector<SBSceneListener*>& listeners = SmartBody::SBScene::getScene()->getSceneListeners();
	for (size_t l = 0; l < listeners.size(); l++)
	{
		listeners[l]->OnObjectCreate(blend);
	}
	return blend;
}

SBMotionBlendBase* SBAnimationBlendManager::createMotionBlendBase( const std::string& name, const std::string& skelName, int dimension )
{
	SBMotionBlendBase* blend = new SBMotionBlendBase(name,skelName, dimension);
	
	//	addBlendToGraph(name);
	_blends.push_back(blend);

	std::vector<SBSceneListener*>& listeners = SmartBody::SBScene::getScene()->getSceneListeners();
	for (size_t l = 0; l < listeners.size(); l++)
	{
		listeners[l]->OnObjectCreate(blend);
	}
	return blend;
}

SBAnimationTransition* SBAnimationBlendManager::createTransition(const std::string& source, const std::string& dest)
{	
	
	
	SBAnimationBlend* sourceBlend = getBlend(source);
	if (!sourceBlend)
	{
		LOG("Source state %s does not exist. No transition created.", source.c_str());
		return NULL;
	}
	SBAnimationBlend* destBlend = getBlend(dest);
	if (!destBlend)
	{
		LOG("Destination state %s does not exist. No transition created.", dest.c_str());
		return NULL;
	}
	SBAnimationTransition* transition = new SBAnimationTransition(source + "/" + dest);
	transition->set(sourceBlend, destBlend);

	_transitions.push_back(transition);

	std::vector<SBSceneListener*>& listeners = SmartBody::SBScene::getScene()->getSceneListeners();
	for (size_t l = 0; l < listeners.size(); l++)
	{
		listeners[l]->OnObjectCreate(transition);
	}
	return transition;
}

SBAnimationBlend* SBAnimationBlendManager::getBlend(const std::string& name)
{
	for (std::vector<SBAnimationBlend*>::iterator iter = _blends.begin();
		 iter != _blends.end();
		 iter++)
	{
		if ((*iter)->stateName == name)
			return (*iter);
	}
	return NULL;
}

int SBAnimationBlendManager::getNumBlends()
{
	return _blends.size();
}

std::vector<std::string> SBAnimationBlendManager::getBlendNames()
{
	std::vector<std::string> states;
	for (size_t i = 0; i < _blends.size(); i++)
	{
		states.push_back(_blends[i]->stateName);
	}
	return states;
}

std::vector<std::string> SBAnimationBlendManager::getTransitionBlends(const std::string& source)
{
	std::vector<std::string> blends;
	
	for (size_t i = 0; i < _transitions.size(); i++)
	{
		if (_transitions[i]->getSourceBlend()->stateName == source)
			blends.push_back(_transitions[i]->getDestinationBlend()->stateName);
	}

	return blends;
}

SBAnimationTransition* SBAnimationBlendManager::getTransition(const std::string& source, const std::string& dest)
{
	for (size_t i = 0; i < _transitions.size(); i++)
	{
		if (_transitions[i]->getSourceBlend()->stateName == source &&
			_transitions[i]->getDestinationBlend()->stateName == dest)
			return _transitions[i];
	}

	return NULL;
}

SBAnimationTransition* SBAnimationBlendManager::getTransitionByName( const std::string& transitionName )
{
	for (size_t i = 0; i < _transitions.size(); i++)
	{
		if (_transitions[i]->getTransitionName() == transitionName)
			return _transitions[i];
	}

	return NULL;
}

SBAnimationTransition* SBAnimationBlendManager::getTransitionByIndex(int id)
{
	if (id >= 0 && id < (int) _transitions.size())
	{
		SBAnimationTransition* animTransition = _transitions[id];
		return animTransition;
	}
	
	return NULL;
}


int SBAnimationBlendManager::getNumTransitions()
{
	return _transitions.size();
}

std::vector<std::string> SBAnimationBlendManager::getTransitionNames()
{
	std::vector<string> transitionNames;
	for (size_t i = 0; i < _transitions.size(); i++)
	{
		transitionNames.push_back( _transitions[i]->getTransitionName() );
	}
	return transitionNames;
}

std::string SBAnimationBlendManager::getCurrentBlend(const std::string& characterName)
{
	SBCharacter* character = SmartBody::SBScene::getScene()->getCharacter(characterName);
	if (!character)
		return "";

	if (!character->param_animation_ct)
		return "";

	PABlendData* blendData = character->param_animation_ct->getCurrentPABlendData();
	if (!blendData)
		return "";
	return blendData->state->stateName;
}

SrVec SBAnimationBlendManager::getCurrentBlendParameters(const std::string& characterName)
{
	SrVec params;
	SBCharacter* character = SmartBody::SBScene::getScene()->getCharacter(characterName);
	if (!character)
		return params;

	if (!character->param_animation_ct)
		return params;

	
	PABlendData* blendData = character->param_animation_ct->getCurrentPABlendData();
	if (!blendData)
		return params;

	SmartBody::SBAnimationBlend0D* state0D = dynamic_cast<SmartBody::SBAnimationBlend0D*>(blendData->state);
	if (state0D)
		return params;

	SmartBody::SBAnimationBlend1D* state1D = dynamic_cast<SmartBody::SBAnimationBlend1D*>(blendData->state);
	SmartBody::SBAnimationBlend2D* state2D = dynamic_cast<SmartBody::SBAnimationBlend2D*>(blendData->state);
	SmartBody::SBAnimationBlend3D* state3D = dynamic_cast<SmartBody::SBAnimationBlend3D*>(blendData->state);
	SmartBody::SBMotionBlendBase* blendBase = dynamic_cast<SmartBody::SBMotionBlendBase*>(blendData->state);
	if (state1D)
	{
		blendData->state->getParametersFromWeights(params[0], blendData->weights);
		return params;
	}
	if (state2D)
	{
		blendData->state->getParametersFromWeights(params[0], params[1], blendData->weights);
		return params;
	}
	if (state3D || blendBase)
	{
		blendData->state->getParametersFromWeights(params[0], params[1], params[2], blendData->weights);
		return params;
	}
	return params;
}

bool SBAnimationBlendManager::isBlendScheduled(const std::string& characterName, const std::string& stateName)
{
	SBCharacter* character = SmartBody::SBScene::getScene()->getCharacter(characterName);
	if (!character)
		return false;

	if (!character->param_animation_ct)
		return false;

	return character->param_animation_ct->hasPABlend(stateName);
}

void SBAnimationBlendManager::removeAllBlends()
{
	// remove the transitions, too
	removeAllTransitions();

	for (size_t i = 0; i < _blends.size(); i++)
	{
		delete _blends[i];
	}
	_blends.clear();

	stateGraph = BoostGraph();

}

void SBAnimationBlendManager::removeAllTransitions()
{
	for (size_t i = 0; i < _transitions.size(); i++)
	{
		delete _transitions[i];
	}
	_transitions.clear();
}

}