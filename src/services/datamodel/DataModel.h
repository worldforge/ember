/*
    Copyright (C) 2002  Tim Enderling

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

#ifndef DATAMODEL_H
#define DATAMODEL_H

// Include other headers of the current program here

// Include library headers here

#include <boost/shared_ptr.hpp>
#include <sigc++/signal_system.h>

// Include system headers here

namespace dime {

class DataObject;
typedef boost::shared_ptr<DataObject> PDataObject;
class DataKey;
typedef boost::shared_ptr<DataKey> PDataKey;

/**
 * Data object types/flags
 */

enum DataType
{  
  /**
   * is called before the value is changed (for data model users needing the old value).
   */
  PRE_VALUE_CHANGE = 0x0001,

  /**
   * is called after the value has changed.
   */
  POST_VALUE_CHANGE = 0x0002,

  /**
   * is called after a data object was added.
   */
  POST_ADDITION = 0x0004,

  /**
   * is called before a data object will be deleted.
   */
  PRE_DELETION = 0x0008,

  /**
   * is called before a child node get's unlinked.
   */
  PRE_LINK_CHANGE = 0x0010,

  /**
   * is called after a child node got linked.
   */
  POST_LINK_CHANGE = 0x0020,
  
  /**
   * is used to indicate, that a signal is fired on events concerning a direct/indirect
   * child.
   */
  FIRE_ON_CHILD_EVENT = 0x0040,

  /**
   * Void nodes are also used for unspecified linking nodes.
   */
  VOID = 0x0080,
  FOLDER = 0x0100,
  LIST = 0x0100,
  STREAM = 0x0100,
  BOOLEAN = 0x0200,
  INTEGER = 0x0400,
  FLOAT = 0x0800,
  STRING = 0x1000,
  CUSTOM_TYPE = 0x2000,
  
  /**
   * See http://www.worldforge.org/dev/eng/clients/dime/developer_doc/DataModelFW
   * for more info on certain flags.
   */

  HAS_CHILDS = 0x4000,
  ADOPTS_CHILDS = 0x8000,
  CAN_SET_VALUE = 0x10000,
  CAN_ADD_CHILDS = 0x20000, 
  CAN_REMOVE = 0x40000,
  IS_LINK = 0x80000
};


/**
 * Signal type for data model change signals.
 * The form is void DataObjectChanged(PDataObject subject, type event).
 */
typedef SigC::Signal2<void, PDataObject, DataType, SigC::Marshal<void> > DataSignal;

/**
 * Slot type for data model change signals.
 */
typedef DataSignal::InSlotType DataSlot;

} // namespace dime

#endif
