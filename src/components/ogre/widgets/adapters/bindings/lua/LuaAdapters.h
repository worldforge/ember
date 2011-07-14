/*
 * This file contains several specialisations needed to workaround tolua++ issues with templated classes
 *
 * TODO: Write the specific issues so that this can be removed when these are fixed in tolua++ 
 */

#include "../../ComboboxAdapter.h"
#include <Atlas/Message/Element.h>
#include <string>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

template <typename ValueType>
class StringComboboxAdapter : public ComboboxAdapter<ValueType, std::string>
{
public:
	StringComboboxAdapter(const ValueType& value, CEGUI::Window* widget);
	virtual ~StringComboboxAdapter();
};

template <typename ValueType>
inline StringComboboxAdapter<ValueType>::StringComboboxAdapter(const ValueType& value, CEGUI::Window* widget) : ComboboxAdapter<ValueType, std::string>(value, widget)
{}

template <typename ValueType>
inline StringComboboxAdapter<ValueType>::~StringComboboxAdapter() {}

}

}
}
} 
