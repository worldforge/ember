/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)
    Copyright (C) 2008 Erik Hjortsberg <erik.hjortsberg@iteam.se>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SoundEntity.h"

#include "services/logging/LoggingService.h"

#include "components/ogre/EmberPhysicalEntity.h"

#include "services/sound/SoundInstance.h"
#include "services/sound/SoundSource.h"
#include "SoundAction.h"

#include "components/ogre/model/ModelDefinition.h"
#include "components/ogre/model/Model.h"

namespace EmberOgre
{
	SoundEntity::SoundEntity(EmberPhysicalEntity& parentEntity)
	: mParentEntity(parentEntity), mCurrentMovementAction(0)
	{
		parentEntity.Acted.connect(sigc::mem_fun(*this, &SoundEntity::Entity_Action));
		createActions();
	}

	SoundEntity::~SoundEntity()
	{
		for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
			delete I->second;
		}
		for (ActionStore::iterator I = mMovementActions.begin(); I != mMovementActions.end(); ++I) {
			delete I->second;
		}
		///Note that we shouldn't delete the mCurrentMovementAction since that's a pointer to an object held in the mMovementActions store.
	}

	WFMath::Point<3> SoundEntity::getPosition() const
	{
		return mParentEntity.getPredictedPos();
	}

	WFMath::Vector<3> SoundEntity::getVelocity() const
	{
		return mParentEntity.getPredictedVelocity();
	}

	const SoundAction* SoundEntity::playAction(const std::string& name)
	{
		ActionStore::iterator I = mActions.find(name);
		if (I != mActions.end()) {
			SoundAction* action = I->second;
			action->play();
			return action;
		}
		return 0;
	}

	void SoundEntity::Entity_Action(const Atlas::Objects::Operation::RootOperation& act)
	{
		const std::list<std::string> &p = act->getParents();
		std::list<std::string>::const_iterator I = p.begin();
		
		if (I != p.end()) {
			const std::string& name = *I;
			playAction(name);
		}
	}
	
	const SoundAction* SoundEntity::playMovementSound(const std::string& actionName)
	{
		ActionStore::iterator I = mMovementActions.find(actionName);
		if (I != mMovementActions.end()) {
			SoundAction* action = I->second;
			if (mCurrentMovementAction != action) {
				if (mCurrentMovementAction) {
					mCurrentMovementAction->stop();
				}
				action->play();
				mCurrentMovementAction = action;
			}
		} else {
			if (mCurrentMovementAction) {
				mCurrentMovementAction->stop();
			}
			mCurrentMovementAction = 0;
		}
		return mCurrentMovementAction;
	}
	
	
	void SoundEntity::createActions()
	{
		Model::Model* model = mParentEntity.getModel();
		if (!model) {
			S_LOG_FAILURE("Tried to create actions for a entity which has no model specified.");
			return;
		}
		const Model::ActionDefinitionsStore& store = model->getDefinition()->getActionDefinitions();
		for (ActionDefinitionsStore::const_iterator I = store.begin(); I != store.end(); ++I)
		{
			// Setup All Sound Actions
			SoundDefinitionsStore::const_iterator J = (*I)->getSoundDefinitions().begin();
			SoundDefinitionsStore::const_iterator J_end = (*I)->getSoundDefinitions().end();
			for (; J != J_end; ++J)
			{
				Model::SoundDefinition* soundDef(*J);
				if (!soundDef)
				{
					continue;
				}

				SoundAction* newAction = new SoundAction(*this);

				SoundGroup* newGroup = newAction->setGroup(soundDef->groupName);
				if ((*I)->getName().find("__movement_") == 0) {
					newAction->setIsLooping(true);
					mMovementActions.insert(ActionStore::value_type((*I)->getName(), newAction));
				} else {
					newAction->setIsLooping(false);
					mActions.insert(ActionStore::value_type((*I)->getName(), newAction));
				}
				break; //for now we'll only allow one group per action
/*				if (newGroup)
				{
					newGroup->setPlayOrder(sound->playOrder);
					S_LOG_INFO("Sound Group " + sound->groupName
							+ " registered within entity");
				}*/
			}
		}	
	}

} // namespace Ember

