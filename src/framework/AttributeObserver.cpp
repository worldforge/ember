//
// C++ Implementation: AttributeObserver
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "AttributeObserver.h"
#include "DirectAttributeObserver.h"
#include "DeepAttributeObserver.h"
#include "Tokeniser.h"

namespace Ember
{

AttributeObserver::AttributeObserver(Eris::Entity& entity, const std::string& attributeName)
{
	mDirectAttributeObserver.reset(new DirectAttributeObserver(entity, EventChanged, attributeName));
}

AttributeObserver::AttributeObserver(Eris::Entity& entity, const std::vector<std::string>& attributePath)
{
	if (attributePath.size() > 1) {
		mDeepAttributeObserver.reset(new DeepAttributeObserver(entity, EventChanged, attributePath));
	} else if (attributePath.size() > 0) {
		mDirectAttributeObserver.reset(new DirectAttributeObserver(entity, EventChanged, attributePath.front()));
	}
}

AttributeObserver::AttributeObserver(Eris::Entity& entity, const std::string& attributePath, const std::string& delimitor)
{
	std::vector<std::string> path = Tokeniser::split(attributePath, delimitor);
	if (path.size() > 1) {
		mDeepAttributeObserver.reset(new DeepAttributeObserver(entity, EventChanged, path));
	} else if (path.size() > 0) {
		mDirectAttributeObserver.reset(new DirectAttributeObserver(entity, EventChanged, path.front()));
	}
}

AttributeObserver::~AttributeObserver()
{
}

void AttributeObserver::forceEvaluation()
{
	if (mDeepAttributeObserver.get()) {
		mDeepAttributeObserver->forceEvaluation();
	} else if (mDirectAttributeObserver.get()) {
		mDirectAttributeObserver->forceEvaluation();
	}
}
}
