/*
 Copyright (C) 2009 Erik Hjortsberg

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

#ifndef MODELPARTREACTIVATORVISITOR_H_
#define MODELPARTREACTIVATORVISITOR_H_

#include "components/entitymapping/ChangeContext.h"
#include "components/entitymapping/IVisitor.h"

namespace EmberOgre
{
namespace Model
{

/**
 *
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Used to reactivate parts on a model which have already been shown.
 * This is done by visiting all the actions of the mapping and reactivating those part actions which already are active.
 *
 */
class ModelPartReactivatorVisitor: public Ember::EntityMapping::IVisitor
{
public:
	virtual void visit(Ember::EntityMapping::Actions::Action& action);

	virtual void visit(Ember::EntityMapping::Matches::MatchBase& match);

	virtual void visit(Ember::EntityMapping::Cases::CaseBase& caseBase);
private:
	Ember::EntityMapping::ChangeContext mChangeContext;

};

}
}
#endif /* MODELPARTREACTIVATORVISITOR_H_ */
