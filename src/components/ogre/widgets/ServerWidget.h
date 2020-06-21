//
// C++ Interface: ServerWidget
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2004
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
#ifndef EMBEROGRE_SERVERWIDGET_H
#define EMBEROGRE_SERVERWIDGET_H

#include "Widget.h"
#include "WidgetPlugin.h"
#include "components/ogre/authoring/DetachedEntity.h"
#include "services/server/AvatarTransferInfo.h"

#include <Eris/ServerInfo.h>
#include <Eris/Connection.h>
#include <Eris/Avatar.h>
#include <Eris/Account.h>

#include <Atlas/Objects/Entity.h>
#include <Atlas/Message/Element.h>

#include <boost/optional.hpp>

#include <map>
#include <vector>
#include <framework/AutoCloseConnection.h>

namespace Ember {
namespace OgreView {


class GUIManager;
namespace Gui {


class Widget;

class ModelRenderer;

class EntityTextureManipulator;

/**
@author Erik Ogenvik
*/
class ServerWidget : public virtual sigc::trackable {
public:
	static WidgetPluginCallback registerWidget(Ember::OgreView::GUIManager& guiManager);

	ServerWidget(GUIManager& guiManager, Eris::Connection& connection);

	~ServerWidget();

	void buildWidget();

protected:

	Widget* mWidget;

	Eris::Connection& mConnection;

	Eris::Account* mAccount;

	/**
	 * @brief A preview renderer for creating new characters
	 */
	std::unique_ptr<ModelRenderer> mModelPreviewRenderer;

	/**
	 * @brief Handles manipulation of the entity preview.
	 */
	std::unique_ptr<EntityTextureManipulator> mModelPreviewManipulator;

	Atlas::Message::MapType mNewEntity;

	CEGUI::Listbox* mCharacterList;
	CEGUI::PushButton* mCreateChar;
	CEGUI::PushButton* mUseCreator;

	boost::optional<AvatarTransferInfo> mAvatarTransferInfo;

	/**
	 * @brief Keeps track of the character ids of the characters in the mCharacterList listbox.
	 */
	std::vector<std::string> mCharacterModel;

	sigc::connection mTypeServiceConnection;

	/**
	 * The name of the type we'll use for preview. This will either be a Type, or an Archetype.
	 * If it's an Archetype we need to resolve the Archetype and then extract the Type of the
	 * first entity, and use that for lookup.
	 */
	std::string mPreviewTypeName;

	std::vector<AutoCloseConnection> mConnections;

	void createdAccount(Eris::Account* account);

	void gotAvatar(Eris::Avatar* avatar);

	void avatar_EntityDeleted();

	void avatar_Deactivated(const std::string& avatarId);

	bool Login_Click(const CEGUI::EventArgs& args);

	bool UseCreator_Click(const CEGUI::EventArgs& args);

	bool CreateAcc_Click(const CEGUI::EventArgs& args);

	bool OkButton_Click(const CEGUI::EventArgs& args);

	bool EntityDestroyedOkButton_Click(const CEGUI::EventArgs& args);


	bool fetchCredentials(Eris::Connection& connection, std::string& user, std::string& pass);

	bool saveCredentials();

	void loginSuccess(Eris::Account* account);

	void logoutComplete(bool clean);


	/**
	* Shows the error message sent from Eris if the login does not succeed.
	*/
	void showLoginFailure(Eris::Account* account, std::string msg);

	/**
	* Hides the login error message and moves the HelpText to it's original position.
	*/
	bool hideLoginFailure();

	void gotAllCharacters(Eris::Account* account);

	void updateNewCharacter();


	/**
	 *    Fills the drop down list with available character types from the server.
	 * @param account 
	 */
	void fillAllowedCharacterTypes(Eris::Account* account);

	/**
	 *    Set up the preview renderer.
	 */
	void createPreviewTexture();


	/**
	 *    Shows server info.
	 */
	void showServerInfo(Eris::Connection& connection);

	/**
	 * @brief Shows an alert explaining to the user that the server doesn't have any available characters and thus isn't correctly setup.
	 */
	void showNoCharactersAlert();

	void showOutdatedProtocolAlert();

	void displayPanel(const std::string& windowName);

	void server_TransferInfoAvailable(const std::vector<AvatarTransferInfo>& transferInfos);

	void showPreview(Ember::OgreView::Authoring::DetachedEntity& entity);

	void preparePreviewForTypeOrArchetype(std::string typeOrArchetype);

	void typeService_TypeBound(Eris::TypeInfo* type);


};
}
}
}
#ifdef WF_USE_WIDGET_PLUGINS
BOOST_DLL_ALIAS(
		Ember::OgreView::Gui::ServerWidget::registerWidget,
		registerWidget
)
#endif

#endif
