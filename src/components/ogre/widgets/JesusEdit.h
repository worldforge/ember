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

#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIEditbox.h>
#include <CEGUIMouseCursor.h>

#include "../GUIManager.h"


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

class JesusMousePicker;
class ModelBlock;
class AttachPointNode;
class Construction;
class Jesus;
class Model;



/**
@author Erik Hjortsberg
*/

class JesusEditPreviewRenderListener : public Ogre::RenderTargetListener
{
protected:
	GUIManager* mGuiManager;

public:

	JesusEditPreviewRenderListener(GUIManager* guiManager) : mGuiManager(guiManager) {}

    void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

    void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
	

};

class JesusEditPreview
{
public:
	JesusEditPreview(GUIManager* guiManager, Jesus* jesus);
	~JesusEditPreview();
	
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

protected:
	Jesus* mJesus;
	Construction* mConstruction;
	GUIManager* mGuiManager;
	void createPreviewTexture();
	void createCamera();
	Ogre::SceneNode* mCameraNode;
	Ogre::SceneNode* mEntityNode;
	Ogre::Camera* mCamera;
	//Model* mModel;
	JesusEditPreviewRenderListener* mListener;
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
};



class JesusEdit : public Widget
{
public:
    JesusEdit();

    ~JesusEdit();
	
	virtual void buildWidget();
	
protected:

	CEGUI::Window* mPreviewWindow;
	
	bool mInJesusMode;
	JesusMousePicker mMousePicker; 
	
	void createdJesus(Jesus* jesus);
	void loadFromJesus(Jesus* jesus);
	
	bool SwitchMode_Click(const CEGUI::EventArgs& args);
	bool Bind_Click(const CEGUI::EventArgs& args);
	bool Create_Click(const CEGUI::EventArgs& args);
	bool CreateNew_Click(const CEGUI::EventArgs& args);

	bool AvailableBlocksList_SelectionChanged(const CEGUI::EventArgs& args);
	bool CurrentBlocksList_SelectionChanged(const CEGUI::EventArgs& args);
	bool CurrentPointsList_SelectionChanged(const CEGUI::EventArgs& args);
	bool NewPointsList_SelectionChanged(const CEGUI::EventArgs& args);
	

	void updateBindingButton();
	void updateCreateButton();
	
	const Carpenter::AttachPoint* getSelectedPointForCurrentBlock() const;
	const Carpenter::AttachPoint* getSelectedPointForNewBlock() const;
	
	void pickedModelBlock(ModelBlock* modelBlock, const CEGUI::MouseEventArgs&);
	void pickedAttachPointNode(AttachPointNode* pointNode, const CEGUI::MouseEventArgs&);

	void fillAttachPointList(ModelBlock* block);
	void fillNewAttachPointList(const Carpenter::BlockSpec* blockspec);
	void loadConstruction(Construction* construction);

	CEGUI::PushButton* mBind;
	CEGUI::PushButton* mCreate;
	CEGUI::PushButton* mCreateNew;
	
	CEGUI::Editbox* mNewName;
		
	CEGUI::Listbox* mAvailableBlocksList;
	CEGUI::Listbox* mCurrentBlocksList;
	CEGUI::Listbox* mCurrentPointsList;
	CEGUI::Listbox* mNewPointsList;
	
	std::vector<Carpenter::BuildingBlockBinding*> createBindingsForNewBlock(Carpenter::BuildingBlock* newBlock);
	
	Construction* mCurrentConstruction;
	inline ModelBlock* getSelectedBlock() { return mCurrentlySelectedBlock;};
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
};


/**
Because CEGUI::Listbox can't hold const objects in its
*/
template<typename T> class ConstWrapper
{
public:
	ConstWrapper(T value) : mValue(value) {}
	T mValue;
};

// class AttachPointHolder
// {
// public:
// 	AttachPointHolder(const Carpenter::AttachPoint* attachPoint);
// 	const Carpenter::AttachPoint* mAttachPoint;
// };

}

#endif
