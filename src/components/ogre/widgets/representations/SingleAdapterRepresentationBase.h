//
// C++ Interface: SingleAdapterRepresentationBase
//
// Description: 
//
// Author: Martin Preisler <preisler.m@gmail.com>
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
#ifndef EMBEROGRE_GUI_REPRESENTATIONS_SINGLEADAPTERREPRESENTATIONBASE_H
#define EMBEROGRE_GUI_REPRESENTATIONS_SINGLEADAPTERREPRESENTATIONBASE_H

#include "RepresentationBase.h"

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

/**
 * @brief a representation using just one adapter
 * 
 * Most representations will only use a single adapter, this class serves as a finger saver for the common
 * case. It subscribes to the adapter and takes care of synchronisation of the adapter value and target value.
 * 
 * You only have to construct and destruct the layout and specify which adapter to use.
 */
template<typename ValueType>
class SingleAdapterRepresentationBase : public RepresentationBase<ValueType>
{
public:
	/**
	 * @brief Ctor
	 * 
	 * @copydoc RepresentationBase::RepresentationBase
	 */
	SingleAdapterRepresentationBase(ValueType& value):
		RepresentationBase<ValueType>(value),
		
		mAdapter(0)
	{}
	
	/**
	 * @brief Dtor
	 */
	virtual ~SingleAdapterRepresentationBase()
	{
		delete mAdapter;
	}
	
protected:
	AdapterBase<ValueType>* mAdapter;
	
	/**
	 * @brief sets the single adapter to use in this representation
	 *  
	 * @param adapter the single adapter we want to use (this class takes ownership of it and will delete it!)
	 * @note You may only call this once in your inherited implementation (usually in the constructor)
	 */
	void setAdapter(AdapterBase<ValueType>* adapter)
	{
		assert(!mAdapter);
		assert(adapter);
		
		mAdapter = adapter;
		mAdapter->EventValueChanged.connect(sigc::mem_fun(*this, &SingleAdapterRepresentationBase::adapterValueChanged));
	}
	
	void adapterValueChanged()
	{
		this->mValue = mAdapter->getValue();
	}
};

}

}

}

}

#endif
