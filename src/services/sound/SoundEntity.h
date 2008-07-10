/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)

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

#ifndef SOUND_ENTITY_H
#define SOUND_ENTITY_H

#include "SoundGeneral.h"
#include "SoundService.h"

namespace Ember
{
	class SoundEntity
	{
		private:
			std::string mName;
			WFMath::Point<3> mPosition;
			WFMath::Vector<3> mVelocity;

			std::map<std::string, SoundAction*> mActions;

		public:
			SoundEntity(const std::string& name);
			~SoundEntity();

			void setPosition(const WFMath::Point<3> &pos);
			void setVelocity(const WFMath::Vector<3> &vel);

			const WFMath::Point<3> getPosition();
			const WFMath::Vector<3> getVelocity();

			SoundAction* registerAction(const std::string& name, const orderOfPlay playorder, 
					const float timestep);

			void onAction(const Atlas::Objects::Operation::RootOperation& act);
			void update(const double timeSinceLastFrame);
	};
}

#endif

