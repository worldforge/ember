#ifndef EMBEROGRE_GUIGENERICICONUSERDATA_H
#define EMBEROGRE_GUIGENERICICONUSERDATA_H

namespace Ember
{
namespace OgreView
{
namespace Gui
{
template <class IconType>
class GenericIconUserData
{
public:
	/**
	 * @brief Ctor.
	 * @param entityIcon The entity icon to which to which this user data belongs.
	 */
	GenericIconUserData(IconType& mGenericIcon);

	/**
	 * @brief Gets the entity icon instance to which this user data instance belongs.
	 * @return The entity icon to which to which this user data belongs.
	 */
	IconType& getIcon() const;
private:
	/**
	@brief The entity icon to which to which this user data belongs.
	*/
	IconType& mGenericIcon;
};

template <class IconType>
GenericIconUserData<IconType>::GenericIconUserData(IconType& icon)
: mGenericIcon(icon)
{
}

template <class IconType>
IconType& GenericIconUserData<IconType>::getIcon() const
{
	return mGenericIcon;
}

}
}

}

#endif
