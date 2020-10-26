//
// C++ Interface: WidgetPool
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#ifndef EMBEROGRE_GUIWIDGETPOOL_H
#define EMBEROGRE_GUIWIDGETPOOL_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include <vector>
#include <stack>
#include <algorithm>
#include <memory>

namespace Ember {
namespace OgreView {

namespace Gui {

/**
	@author Erik Ogenvik <erik@ogenvik.org>
*/
template<typename T>
class WidgetPool {
public:

	struct WidgetCreator {
	public:
		virtual ~WidgetCreator() = default;

		virtual std::unique_ptr<T> createWidget(unsigned int currentPoolSize) = 0;
	};

	explicit WidgetPool(WidgetCreator& creator) :
			mCreator(creator) {}

	virtual ~WidgetPool();

	std::unique_ptr<T> checkoutWidget();

	void returnWidget(std::unique_ptr<T> widget);

	void initializePool(size_t initialSize);

protected:
	WidgetCreator& mCreator;

	std::vector<std::unique_ptr<T>> mWidgetsPool;
};

template<typename T>
WidgetPool<T>::~WidgetPool() = default;

template<typename T>
std::unique_ptr<T> WidgetPool<T>::checkoutWidget() {
	if (!mWidgetsPool.size()) {
		return mCreator.createWidget(mWidgetsPool.size());
	} else {
		auto widget = std::move(mWidgetsPool.back());
		mWidgetsPool.pop_back();
		return widget;
	}
}

template<typename T>
void WidgetPool<T>::returnWidget(std::unique_ptr<T> widget) {
	mWidgetsPool.emplace_back(std::move(widget));
}

template<typename T>
void WidgetPool<T>::initializePool(size_t initialSize) {
	for (size_t i = 0; i < initialSize; ++i) {
		auto newWidget = mCreator.createWidget(mWidgetsPool.size());
		mWidgetsPool.emplace_back(std::move(newWidget));
	}
}

}

}

}

#endif
