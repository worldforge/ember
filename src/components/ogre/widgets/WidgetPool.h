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
	typedef std::vector<T*> WidgetStore;
	typedef std::stack<T*> WidgetStack;

	class WidgetCreator {
	public:
		virtual ~WidgetCreator() {}

		virtual std::unique_ptr<T> createWidget(unsigned int currentPoolSize) = 0;
	};

	WidgetPool(WidgetCreator& creator) : mCreator(creator) {}

	virtual ~WidgetPool();

	T* checkoutWidget();

	void returnWidget(T* widget);

	void initializePool(unsigned int initialSize);

	std::vector<T*>& getUsedWidgets();

	std::vector<T*>& getWidgetPool();

	std::stack<T*>& getUnusedWidgets();

protected:
	WidgetCreator& mCreator;

	WidgetStore mUsedWidgets;

	std::vector<std::unique_ptr<T>> mWidgetsPool;
	WidgetStack mUnusedWidgets;
};

template<typename T>
WidgetPool<T>::~WidgetPool() = default;

template<typename T>
T* WidgetPool<T>::checkoutWidget() {
	T* widget;
	if (!mUnusedWidgets.size()) {
		auto newWidget = mCreator.createWidget(mWidgetsPool.size());
		widget = newWidget.get();
		mWidgetsPool.push_back(std::move(newWidget));
	} else {
		widget = mUnusedWidgets.top();
		mUnusedWidgets.pop();
	}
	mUsedWidgets.push_back(widget);
	return widget;
}

template<typename T>
void WidgetPool<T>::returnWidget(T* widget) {
	mUnusedWidgets.push(widget);
	typename std::vector<T*>::iterator I = std::find(mUsedWidgets.begin(), mUsedWidgets.end(), widget);
	if (I != mUsedWidgets.end()) {
		mUsedWidgets.erase(I);
	}
}

template<typename T>
void WidgetPool<T>::initializePool(unsigned int initialSize) {
	for (unsigned int i = 0; i < initialSize; ++i) {
		auto newWidget = mCreator.createWidget(mWidgetsPool.size());
		auto widget = newWidget.get();
		mWidgetsPool.push_back(std::move(newWidget));
		mUnusedWidgets.push(widget);
	}
}

template<class T>
std::vector<T*>& WidgetPool<T>::getUsedWidgets() {
	return mUsedWidgets;
}

template<typename T>
std::vector<T*>& WidgetPool<T>::getWidgetPool() {
	return mWidgetsPool;
}

template<typename T>
std::stack<T*>& WidgetPool<T>::getUnusedWidgets() {
	return mUnusedWidgets;
}
}

}

}

#endif
