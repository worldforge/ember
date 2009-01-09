//
// C++ Interface: JesusEdit
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#ifndef EMBEROGREJESUSEDIT_H
#define EMBEROGREJESUSEDIT_H

#include "Widget.h"
#include "../jesus/JesusMousePicker.h"

// #include <elements/CEGUIListbox.h>
// #include <elements/CEGUIListboxItem.h> 
// #include <elements/CEGUIListboxTextItem.h> 
// #include <elements/CEGUIEditbox.h>
#include <CEGUIMouseCursor.h>

#include "../GUIManager.h"
#include "framework/ConsoleObject.h"




namespace Carpenter
{
class AttachPair;
class AttachPoint;
class BlockSpec;
class BuildingBlock;
class BuildingBlockSpec;
class BuildingBlockSpecDefinition;
class BuildingBlockBinding;
class BuildingBlockBindingDefinition;
class BuildingBlockDefinition;
class BluePrint;
class Carpenter;

};

namespace EmberOgre {

namespace Model {
	class Model;
}

class JesusMousePicker;
class ModelBlock;
class AttachPointNode;
class Construction;
class Jesus;
namespace Gui {

class JesusEdit;

class EntityCEGUITexture;

/**
@author Erik Hjortsberg
*/


class JesusEditPreview
{
public:
	JesusEditPreview(GUIManager* guiManager, Jesus* jesus);
	virtual ~JesusEditPreview();
	
	/**
	 *    shows a preview of the BuildingBlockSpec
	 * @param spec 
	 */
	void showBuildingBlock(const std::string & spec);
	
	
	/**
	 *    selects the attach point (makes it begin to blink etc.)
	 * @param point 
	 */
	void selectAttachPoint(const Carpenter::AttachPoint* point);
	
	
	/**
	 *    zooms the camera to the specified value
	 * @param value a value between 0.0 and 1.0
	 */
	void setZoom(float value);

	void setVisible(bool visible);
	
	
protected:
	CEGUI::Window* mPreviewWindow;
	Jesus* mJesus;
	Construction* mConstruction;
	GUIManager* mGuiManager;
	void createPreviewTexture();
	void createCamera();
	Ogre::SceneNode* mCameraNode;
	Ogre::SceneNode* mEntityNode;
	Ogre::Camera* mCamera;
	//Model* mModel;
	Carpenter::BluePrint* mBlueprint;
	
	Carpenter::BuildingBlock* mBlock;
	ModelBlock* mModelBlock;
	
	bool Zoom_ValueChanged(const CEGUI::EventArgs& args);
	
	CEGUI::Slider* mZoomSlider;
	
	/**
	the minimum and maximum camera distance, as used by zoom()
	*/
	Ogre::Real mMinCameraDistance, mMaxCameraDistance;

	
	/**
	The currently selected AttachPointNode
	*/
	AttachPointNode* mSelectedAttachPointNode;
	
	Ogre::RenderTexture* mRenderTexture;
	
	EntityCEGUITexture* mTexture;
};

class JesusEditFile
{
public:
	JesusEditFile(GUIManager* guiManager, JesusEdit* jesusEdit, Jesus* jesus);
	~JesusEditFile();
	
	void show();
	void hide();
	void switchVisibility();
	
protected:
	CEGUI::Window* mWindow;
	JesusEdit* mJesusEdit;
	Jesus* mJesus;
	
	bool Load_Click(const CEGUI::EventArgs& args);
	bool Save_Click(const CEGUI::EventArgs& args);

	CEGUI::PushButton* mLoadButton;
	CEGUI::PushButton* mSaveButton;
	CEGUI::Listbox* mBluePrintList;
	CEGUI::Editbox* mNewNameEditBox;
	
	void fillBluePrintList();
	
	bool saveBluePrint(const std::string& name, Carpenter::BluePrint* blueprint );
		
	

};



class JesusEdit : public Widget
{
public:
    JesusEdit();

    virtual ~JesusEdit();
	
	Jesus* mJesus;
	
	virtual void buildWidget();

	
	Construction* getConstruction() const;
	
	/**
	 *    loads the supplied construction
	 * @param construction 
	 */
	void loadConstruction(Construction* construction);

	
	/**
	 *    creates a new construction from the blueprint and adds it to the world
	 * @param blueprint 
	 * @return 
	 */
	Construction* createNewConstructionFromBlueprint(Carpenter::BluePrint* blueprint);

	
	virtual void show();
	virtual void hide();
	

	
protected:

	/**
	if set to true, we're in edit mode
	*/
	bool mInJesusMode;
	
	/**
	mouse picker which picks building blocks
	*/
	JesusMousePicker mMousePicker; 
	
	
	/**
	 *    method bound to EmberOgre::EventCreatedJesus
	 *    
	 * @param jesus 
	 */
	void createdJesus(Jesus* jesus);
	
	
	/**
	 *    set up the whole system from a Jesus instance
	 * @param jesus 
	 */
	void loadFromJesus(Jesus* jesus);
	
	
	
	///---------start CEGUI callback methods
	
	/**
	 *    switches between normal gui mode and JesusEdit mode
	 * @param args 
	 * @return 
	 */
	bool SwitchMode_Click(const CEGUI::EventArgs& args);
	
	
	/**
	 *    tries to bind two attach points
	 * @param args 
	 * @return 
	 */
	bool Bind_Click(const CEGUI::EventArgs& args);
	
	
	/**
	 *    tries to create a new bblock
	 * @param args 
	 * @return 
	 */
	bool Create_Click(const CEGUI::EventArgs& args);
	
	
	/**
	 *    creates a new Construction
	 * @param args 
	 * @return 
	 */
	bool CreateNew_Click(const CEGUI::EventArgs& args);
	
	/**
	 *    removes a BuildingBlock
	 * @param args 
	 * @return 
	 */
	bool Remove_Click(const CEGUI::EventArgs& args);
	
	

	bool File_Click(const CEGUI::EventArgs& args);
	
	
	bool AvailableBlocksList_SelectionChanged(const CEGUI::EventArgs& args);
	bool CurrentBlocksList_SelectionChanged(const CEGUI::EventArgs& args);
	bool CurrentPointsList_SelectionChanged(const CEGUI::EventArgs& args);
	bool NewPointsList_SelectionChanged(const CEGUI::EventArgs& args);
	
	///-----------end CEGUI callback methods
	
	
	/**
	 *    checks whether the Bind button should be enabled
	 */
	void updateBindingButton();
	/**
	 *    checks whether the Create button should be enabled
	 */
	void updateCreateButton();
	
	/**
	 *    checks whether the Remove button should be enabled
	 */
	void updateRemoveButton( );

	/**
	 *    gets the selected AttachPoint for the current block
	 * @return null if no block selected
	 */
	const Carpenter::AttachPoint* getSelectedPointForCurrentBlock() const;
	/**
	 *    gets the selected AttachPoint for the new
	 * @return null if no block selected
	 */
	const Carpenter::AttachPoint* getSelectedPointForNewBlock() const;
	
	
	/**
	 *    bound to JesusMousePicker::EventPickedModelBlock
	 * @param modelBlock 
	 * @param  
	 */
	void pickedModelBlock(ModelBlock* modelBlock, const MousePickerArgs&);
	
	/**
	 *    bound to JesusMousePicker::EventPickedAttachPointNode
	 * @param pointNode 
	 * @param  
	 */
	void pickedAttachPointNode(AttachPointNode* pointNode, const MousePickerArgs&);

	
	/**
	 *    fill the attach point list with values taken from the supplied ModelBlock
	 * @param block 
	 */
	void fillAttachPointList(ModelBlock* block);
	
	
	/**
	 *    fill the "new attach point" list with values from the supplied BlockSpec
	 * @param blockspec 
	 */
	void fillNewAttachPointList(const Carpenter::BlockSpec* blockspec);
	
	

	
	///---------start CEGUI elements
	CEGUI::PushButton* mBind;
	CEGUI::PushButton* mCreate;
	CEGUI::PushButton* mCreateNew;
	CEGUI::PushButton* mRemove;
	
	CEGUI::Editbox* mNewName;
		
	CEGUI::Listbox* mAvailableBlocksList;
	CEGUI::Listbox* mCurrentBlocksList;
	CEGUI::Listbox* mCurrentPointsList;
	CEGUI::Listbox* mNewPointsList;
	
	///----------end CEGUI elements
	
	
	/**
	 *    creates and returns a vector of BuildingBlockBinding for the supplied BuildingBlock
	 * @param newBlock 
	 * @return 
	 */
	std::vector<Carpenter::BuildingBlockBinding*> createBindingsForNewBlock(Carpenter::BuildingBlock* newBlock);
	
	/**
	 * removes all current bindings
	 */
	void removeBindings();

	
	Construction* mCurrentConstruction;
	ModelBlock* getSelectedBlock();
	ModelBlock* mCurrentlySelectedBlock;
// 	const Carpenter::BuildingBlockSpec* mCurrentlySelectedBuildingBlockSpec;
	
	/**
	The selected attach point node of the existing building block.
	*/
	AttachPointNode* mCurrentlySelectedAttachPointNode;
	
// 	Carpenter::BlockSpec* getNewBlockSpec();
	const Carpenter::BuildingBlockSpec* getNewBuildingBlockSpec() const;
	/**
	used for lookups of ListboxItems from ModelBlocks
	*/
	std::map<ModelBlock*, CEGUI::ListboxItem*> mCurrentBlocksListLookup;
	
	/**
	used for lookups of ListboxItems from AttachPoints
	*/
	std::map<AttachPointNode*, CEGUI::ListboxItem*> mCurrentPointsListLookup;
	
	
	std::map<AttachPointNode*, const Carpenter::AttachPoint*> mBindings;
	
//	void createPreviewTexture();
	JesusEditPreview* mPreview;
	JesusEditFile* mFile;
};

inline ModelBlock* JesusEdit::getSelectedBlock() { return mCurrentlySelectedBlock;};
inline Construction* JesusEdit::getConstruction() const {return mCurrentConstruction;}



// class AttachPointHolder
// {
// public:
// 	AttachPointHolder(const Carpenter::AttachPoint* attachPoint);
// 	const Carpenter::AttachPoint* mAttachPoint;
// };
}
}

#endif
