//
// C++ Interface: WidgetPool
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
#ifndef EMBEROGRE_GUIWIDGETPOOL_H
#define EMBEROGRE_GUIWIDGETPOOL_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include <vector>
#include <stack>
#include <algorithm>

namespace EmberOgre {

namespace Gui {

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
template <typename T>
class WidgetPool
{
	public:
		typedef std::vector<T*> WidgetStore;
		typedef std::stack<T*> WidgetStack;
		class WidgetCreator
		{
			public:
			virtual ~WidgetCreator() {}
			virtual T* createWidget(unsigned int currentPoolSize) = 0;
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

		WidgetStore mWidgetsPool;
		WidgetStack mUnusedWidgets;
};

template <typename T>
WidgetPool<T>::~WidgetPool()
{
	for (typename std::vector<T*>::iterator I = mWidgetsPool.begin(); I != mWidgetsPool.end(); ++I) {
		delete *I;
	}
}


template <typename T>
T* WidgetPool<T>::checkoutWidget()
{
	T* widget(0);
	if (!mUnusedWidgets.size()) {
		widget = mCreator.createWidget(mWidgetsPool.size());
		mWidgetsPool.push_back(widget);
	} else {
		widget = mUnusedWidgets.top();
		mUnusedWidgets.pop();
	}
	mUsedWidgets.push_back(widget);
	return widget;class WidgetPool{
public:
    WidgetPool();

    ~WidgetPool();

};

}

template <typename T>
void WidgetPool<T>::returnWidget(T* widget)
{
	mUnusedWidgets.push(widget);
	typename std::vector<T*>::iterator I = std::find(mUsedWidgets.begin(), mUsedWidgets.end(), widget);
	if (I != mUsedWidgets.end()) {
		mUsedWidgets.erase(I);
	}
}

template <typename T>
void WidgetPool<T>::initializePool(unsigned int initialSize)
{
	for (unsigned int i = 0; i < initialSize; ++i) {
		T* widget = mCreator.createWidget(mWidgetsPool.size());
		mWidgetsPool.push_back(widget);
		mUnusedWidgets.push(widget);
	}
}

template <class T>
std::vector<T*>& WidgetPool<T>::getUsedWidgets()
{
	return mUsedWidgets;
}
template <typename T>
std::vector<T*>& WidgetPool<T>::getWidgetPool()
{
	return mWidgetsPool;
}
template <typename T>
std::stack<T*>& WidgetPool<T>::getUnusedWidgets()
{
	return mUnusedWidgets;
}
}

}

#endif
