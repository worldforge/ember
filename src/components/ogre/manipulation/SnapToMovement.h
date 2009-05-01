//
// C++ Interface: SnapToMovement
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
#ifndef EMBEROGRE_MANIPULATIONSNAPTOMOVEMENT_H
#define EMBEROGRE_MANIPULATIONSNAPTOMOVEMENT_H

#include <wfmath/point.h>

namespace EmberOgre {

class EmberEntity;

namespace Manipulation {

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class SnapToMovement{
public:
    SnapToMovement(EmberEntity& entity);

    ~SnapToMovement();
    
    
    bool testSnapTo(const WFMath::Point<3>& position, WFMath::Point<3>& newPosition, EmberEntity* snappedToEntity);
    
protected:
	EmberEntity& mEntity;

};

}

}

#endif
