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

#define SPECIALISE_COMBOBOX_ADAPTER(prefix, type)\
template <typename ValueType> \
class prefix##ComboboxAdapter : public ComboboxAdapter<ValueType, type> \
{ \
public: \
	prefix##ComboboxAdapter(const ValueType& value, CEGUI::Window* widget); \
	virtual ~prefix##ComboboxAdapter(); \
}; \
template <typename ValueType> \
inline prefix##ComboboxAdapter<ValueType>::prefix##ComboboxAdapter(const ValueType& value, CEGUI::Window* widget) : ComboboxAdapter<ValueType, type>(value, widget) \
{} \
\
template <typename ValueType> \
inline prefix##ComboboxAdapter<ValueType>::~prefix##ComboboxAdapter() \
{}

SPECIALISE_COMBOBOX_ADAPTER(String, std::string)
SPECIALISE_COMBOBOX_ADAPTER(Int, int)
SPECIALISE_COMBOBOX_ADAPTER(Float, float)

}

}
}
} 
