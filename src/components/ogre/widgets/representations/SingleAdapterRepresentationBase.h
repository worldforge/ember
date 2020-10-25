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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_GUI_REPRESENTATIONS_SINGLEADAPTERREPRESENTATIONBASE_H
#define EMBEROGRE_GUI_REPRESENTATIONS_SINGLEADAPTERREPRESENTATIONBASE_H

#include "RepresentationBase.h"
#include "../adapters/AdapterBase.h"

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Representations {

/**
 * @brief a representation using just one adapter
 * 
 * Most representations will only use a single adapter, this class serves as a finger saver for the common
 * case. It subscribes to the adapter and takes care of synchronisation of the adapter value and target value.
 * 
 * You only have to construct and destruct the layout and specify which adapter to use.
 */
template<typename ValueType>
class SingleAdapterRepresentationBase : public RepresentationBase<ValueType> {
public:
	/**
	 * @brief Ctor
	 * 
	 * @copydoc RepresentationBase::RepresentationBase
	 */
	SingleAdapterRepresentationBase();

	/**
	 * @brief Dtor
	 */
	virtual ~SingleAdapterRepresentationBase();

	virtual void setEditedValue(const ValueType& v);

	virtual const ValueType& getEditedValue() const;

	virtual sigc::signal<void>& getEventValueChangedSignal();

	virtual const ValueType& getOriginalValue() const;

	virtual void applyChanges();

	virtual bool hasChanges() const;

	virtual bool isRemoved() const;

	virtual void addSuggestion(const std::string& suggestion);

protected:
	std::unique_ptr<Adapters::AdapterBase<ValueType>> mAdapter;

	/**
	 * @brief sets the single adapter to use in this representation
	 *  
	 * @param adapter the single adapter we want to use (this class takes ownership of it and will delete it!)
	 * @note You may only call this once in your inherited implementation (usually in the constructor)
	 */
	void setAdapter(std::unique_ptr<Adapters::AdapterBase<ValueType>> adapter);
};

template<typename ValueType>
SingleAdapterRepresentationBase<ValueType>::SingleAdapterRepresentationBase() {}

template<typename ValueType>
SingleAdapterRepresentationBase<ValueType>::~SingleAdapterRepresentationBase() = default;

template<typename ValueType>
void SingleAdapterRepresentationBase<ValueType>::setEditedValue(const ValueType& v) {
	mAdapter->setValue(v);
}

template<typename ValueType>
const ValueType& SingleAdapterRepresentationBase<ValueType>::getEditedValue() const {
	return mAdapter->getValue();
}

template<typename ValueType>
sigc::signal<void>& SingleAdapterRepresentationBase<ValueType>::getEventValueChangedSignal() {
	return mAdapter->EventValueChanged;
}

template<typename ValueType>
const ValueType& SingleAdapterRepresentationBase<ValueType>::getOriginalValue() const {
	return mAdapter->getOriginalValue();
}

template<typename ValueType>
void SingleAdapterRepresentationBase<ValueType>::applyChanges() {
	mAdapter->applyChanges();
}

template<typename ValueType>
bool SingleAdapterRepresentationBase<ValueType>::hasChanges() const {
	return mAdapter->hasChanges();
}

template<typename ValueType>
bool SingleAdapterRepresentationBase<ValueType>::isRemoved() const {
	return mAdapter->isRemoved();
}

template<typename ValueType>
void SingleAdapterRepresentationBase<ValueType>::addSuggestion(const std::string& suggestion) {
	mAdapter->addSuggestion(suggestion);
}

template<typename ValueType>
void SingleAdapterRepresentationBase<ValueType>::setAdapter(std::unique_ptr<Adapters::AdapterBase<ValueType>> adapter) {
	mAdapter = std::move(adapter);
}

}

}

}

}

#endif
