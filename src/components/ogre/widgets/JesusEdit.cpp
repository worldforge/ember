//
// C++ Implementation: JesusEdit
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
#include "JesusEdit.h"
#include "ColouredListItem.h"

#include "../jesus/Jesus.h"
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIGUISheet.h>
#include <elements/CEGUISlider.h>
#include "../GUIManager.h"
#include "../carpenter/Carpenter.h"
#include "../carpenter/BluePrint.h"

#include <CEGUIWindowManager.h>
#include <CEGUIImagesetManager.h> 
#include <CEGUIImageset.h> 
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIEditbox.h>

#include "../EmberOgre.h"
#include "../AvatarCamera.h"
#include "../Avatar.h"
#include "../EmberEntity.h"
#include "../EmberPhysicalEntity.h"
// #include "../PersonEmberEntity.h"
#include "../AvatarEmberEntity.h"
#include "../model/Model.h"
#include "framework/ConsoleBackend.h"
#include "../jesus/XMLJesusSerializer.h"

#include "EntityCEGUITexture.h"
#include "../SimpleRenderContext.h"

namespace EmberOgre {



JesusEdit::JesusEdit()
 : Widget(), mInJesusMode(false), mCurrentConstruction(0), mCurrentlySelectedBlock(0), mCurrentlySelectedAttachPointNode(0), mJesus(0), mFile(0), mPreview(0)
{


}


JesusEdit::~JesusEdit()
{
}


void JesusEdit::buildWidget()
{

	loadMainSheet("JesusEdit.widget", "JesusEdit/");
	mMainWindow->setVisible(false);
	
	

	
	//bind buttons
	CEGUI::PushButton* switchButton = static_cast<CEGUI::PushButton*>(getWindow("SwitchMode"));
	BIND_CEGUI_EVENT(switchButton, CEGUI::ButtonBase::EventMouseClick, JesusEdit::SwitchMode_Click)
	
	CEGUI::PushButton* fileButton = static_cast<CEGUI::PushButton*>(getWindow("File"));
	BIND_CEGUI_EVENT(fileButton, CEGUI::ButtonBase::EventMouseClick, JesusEdit::File_Click)
	
	mCreateNew = static_cast<CEGUI::PushButton*>(getWindow("CreateNew"));
	BIND_CEGUI_EVENT(mCreateNew, CEGUI::ButtonBase::EventMouseClick, JesusEdit::CreateNew_Click)
	
	mCreate = static_cast<CEGUI::PushButton*>(getWindow("Create"));
	BIND_CEGUI_EVENT(mCreate, CEGUI::ButtonBase::EventMouseClick, JesusEdit::Create_Click)
	
	mBind = static_cast<CEGUI::PushButton*>(getWindow("Bind"));
	BIND_CEGUI_EVENT(mBind, CEGUI::ButtonBase::EventMouseClick, JesusEdit::Bind_Click)
	
	mRemove = static_cast<CEGUI::PushButton*>(getWindow("Remove"));
	BIND_CEGUI_EVENT(mRemove, CEGUI::ButtonBase::EventMouseClick, JesusEdit::Remove_Click)
	
	
	
	mNewName = static_cast<CEGUI::Editbox*>(getWindow("NewName"));
	
	
	
	//bind lists
	mCurrentBlocksList = static_cast<CEGUI::Listbox*>(getWindow("CurrentBlocks"));
	BIND_CEGUI_EVENT(mCurrentBlocksList, CEGUI::Listbox::EventSelectionChanged, JesusEdit::CurrentBlocksList_SelectionChanged)
	BIND_CEGUI_EVENT(mCurrentBlocksList, CEGUI::Listbox::EventListContentsChanged, JesusEdit::CurrentBlocksList_SelectionChanged)
	
	
	mCurrentPointsList = static_cast<CEGUI::Listbox*>(getWindow("CurrentPoints"));
	BIND_CEGUI_EVENT(mCurrentPointsList, CEGUI::Listbox::EventSelectionChanged, JesusEdit::CurrentPointsList_SelectionChanged)
	BIND_CEGUI_EVENT(mCurrentPointsList, CEGUI::Listbox::EventListContentsChanged, JesusEdit::CurrentPointsList_SelectionChanged)
	
	mAvailableBlocksList = static_cast<CEGUI::Listbox*>(getWindow("AvailableBlocks"));
	BIND_CEGUI_EVENT(mAvailableBlocksList, CEGUI::Listbox::EventSelectionChanged, JesusEdit::AvailableBlocksList_SelectionChanged)
	BIND_CEGUI_EVENT(mAvailableBlocksList, CEGUI::Listbox::EventListContentsChanged, JesusEdit::AvailableBlocksList_SelectionChanged)
	
	mNewPointsList = static_cast<CEGUI::Listbox*>(getWindow("NewBlockPoints"));
	BIND_CEGUI_EVENT(mNewPointsList, CEGUI::Listbox::EventSelectionChanged, JesusEdit::NewPointsList_SelectionChanged)
	BIND_CEGUI_EVENT(mNewPointsList, CEGUI::Listbox::EventListContentsChanged, JesusEdit::NewPointsList_SelectionChanged)
	
	
	
	//bind external events
	mMousePicker.EventPickedModelBlock.connect(sigc::mem_fun(*this, &JesusEdit::pickedModelBlock));
	mMousePicker.EventPickedAttachPointNode.connect(sigc::mem_fun(*this, &JesusEdit::pickedAttachPointNode));
	EmberOgre::getSingleton().EventCreatedJesus.connect(sigc::mem_fun(*this, &JesusEdit::createdJesus));
	
	getMainSheet()->addChildWindow(mMainWindow); 
//	getMainSheet()->addChildWindow(mPreviewWindow); 
	
	//make sure the buttons are disabled by default
	updateBindingButton();
	updateCreateButton();

	registerConsoleVisibilityToggleCommand("builder");
	enableCloseButton();
	
}



void JesusEdit::show()
{
	if (mJesus)
	{
		if (mMainWindow) 
			mMainWindow->setVisible(true);
		if (mPreview) 
			mPreview->setVisible(true);
		S_LOG_VERBOSE("Showing builder window.");
	} else {
		S_LOG_FAILURE("Can't show builder window before the main Jesus object is loaded.");
	}
}
void JesusEdit::hide()
{
	S_LOG_VERBOSE("Hiding builder window.");
	if (mMainWindow) 
		mMainWindow->setVisible(false);
	if (mPreview) 
		mPreview->setVisible(false);
	if (mFile) 
		mFile->hide();
}

bool JesusEdit::SwitchMode_Click(const CEGUI::EventArgs& args)
{
//TODO: this should use a IWorldPickListener instead

/*	if (!mInJesusMode) {
		mGuiManager->pushMousePicker(&mMousePicker);
	} else {
		mGuiManager->popMousePicker();
	}*/
	mInJesusMode = !mInJesusMode;
	return true;
}

void JesusEdit::pickedModelBlock(ModelBlock* modelBlock, const MousePickerArgs&)
{
	if (mCurrentConstruction != modelBlock->getConstruction()) {
		loadConstruction(modelBlock->getConstruction());
	}
	
	//use the lookup map to see what ListBoxItem corresponds to the picked ModleBlock
	std::map<ModelBlock*, CEGUI::ListboxItem*>::iterator I = mCurrentBlocksListLookup.find(modelBlock);
	if (I != mCurrentBlocksListLookup.end()) {
		CEGUI::ListboxItem* item = I->second;
		mCurrentBlocksList->ensureItemIsVisible(item);
		mCurrentBlocksList->setItemSelectState(item, true);
	}

}

void JesusEdit::loadConstruction(Construction* construction) 
{
	mCurrentConstruction = construction;
	mCurrentBlocksList->resetList();
	mCurrentBlocksList->clearAllSelections();
	mCurrentBlocksListLookup.clear();
	std::vector<ModelBlock*> blocks = construction->getModelBlocks();
	
	for (std::vector<ModelBlock*>::iterator I = blocks.begin(); I != blocks.end(); ++I) 
	{
		CEGUI::String name((*I)->getBuildingBlock()->getName());
		CEGUI::ListboxItem* item = new Gui::ColouredListItem(name, 0, *I);
		mCurrentBlocksList->addItem(item);
		//add to the lookup map
		mCurrentBlocksListLookup.insert(std::map<ModelBlock*, CEGUI::ListboxItem*>::value_type(*I, item));
	}
	
	//loadFromJesus(construction->getJesus());
	
}

void JesusEdit::createdJesus(Jesus* jesus)
{
	loadFromJesus(jesus);
	mPreview = new JesusEditPreview(mGuiManager, jesus);
	mFile = new JesusEditFile(mGuiManager, this, jesus);
	mFile->hide();

}

void JesusEdit::loadFromJesus(Jesus* jesus)
{
	mJesus = jesus;
	mAvailableBlocksList->resetList();
	mAvailableBlocksList->clearAllSelections();
	const std::map<const std::string , Carpenter::BuildingBlockSpec >* bblockSpecs = jesus->getCarpenter()->getBuildingBlockSpecs();
	for (std::map<const std::string , Carpenter::BuildingBlockSpec >::const_iterator I = bblockSpecs->begin(); I != bblockSpecs->end(); ++I) 
	{
		ConstWrapper<const Carpenter::BuildingBlockSpec*>* holder = new ConstWrapper<const Carpenter::BuildingBlockSpec*>(&I->second);
		CEGUI::String name(I->second.getName());
		CEGUI::ListboxItem* item = new Gui::ColouredListItem(name, 0, holder);
		mAvailableBlocksList->addItem(item);
	}
}


void JesusEdit::pickedAttachPointNode(AttachPointNode* pointNode, const MousePickerArgs&)
{
	CEGUI::ListboxItem* item = mCurrentPointsListLookup.find(pointNode)->second;
	mCurrentPointsList->ensureItemIsVisible(item);
	mCurrentPointsList->setItemSelectState(item, true);
	
}

bool JesusEdit::CurrentBlocksList_SelectionChanged( const CEGUI::EventArgs & args )
{
	if (mCurrentlySelectedBlock) {
		mCurrentlySelectedBlock->deselect();
		mCurrentlySelectedBlock = 0;
	}
	CEGUI::ListboxItem* item = mCurrentBlocksList->getFirstSelectedItem();
	if (item) {
		ModelBlock* block = static_cast<ModelBlock*>(item->getUserData());
		block->select();
		mCurrentlySelectedBlock = block;
		
		fillAttachPointList(block);
	} else {
		mCurrentPointsList->resetList();
		mCurrentPointsList->clearAllSelections();
	}
	updateRemoveButton();
	
	return true;
	
}

bool JesusEdit::AvailableBlocksList_SelectionChanged( const CEGUI::EventArgs & args )
{
	const Carpenter::BuildingBlockSpec* bblockSpec = getNewBuildingBlockSpec( );
	removeBindings();
	if (bblockSpec) {
		fillNewAttachPointList(bblockSpec->getBlockSpec());
		
		
		mPreview->showBuildingBlock(bblockSpec->getName());
		
/*		mPreview->clearAndDestroyModel();
		Model* model = EmberOgre::getSingleton().getJesus()->createModelForBlockType(bblockSpec->getName(),"JesusEditPreviewModel");
		if (model) {
			mPreview->setModel(model);
		}*/
	} else {
		mNewPointsList->resetList();
		mNewPointsList->clearAllSelections();
	
	}
	updateCreateButton();
	return true;

}

void JesusEdit::removeBindings()
{

	mBindings.clear();
}

void JesusEdit::fillNewAttachPointList(const Carpenter::BlockSpec * blockspec )
{
	mNewPointsList->resetList();
	mNewPointsList->clearAllSelections();
	
	const std::vector<const Carpenter::AttachPoint*> nodes = blockspec->getAllPoints();
	for (std::vector<const Carpenter::AttachPoint*>::const_iterator I = nodes.begin(); I != nodes.end(); ++I) 
	{
		CEGUI::String name((*I)->getAttachPair()->getName() + "/" + (*I)->getName() + " ("+(*I)->getAttachPair()->getType() +")");
		ConstWrapper<const Carpenter::AttachPoint*>* holder = new ConstWrapper<const Carpenter::AttachPoint*>(*I);
		CEGUI::ListboxItem* item = new Gui::ColouredListItem(name, 0, holder);
		mNewPointsList->addItem(item);
		
	}

}


void JesusEdit::fillAttachPointList(ModelBlock* block)
{	
	mCurrentPointsList->resetList();
	mCurrentPointsList->clearAllSelections();
	
	std::vector<AttachPointNode*> nodes = block->getAttachPointNodes();
	for (std::vector<AttachPointNode*>::iterator I = nodes.begin(); I != nodes.end(); ++I) 
	{
		CEGUI::String name((*I)->getAttachPoint()->getAttachPair()->getName() + "/" + (*I)->getAttachPoint()->getName()+ " ("+(*I)->getAttachPoint()->getAttachPair()->getType() +")");
		CEGUI::ListboxItem* item = new Gui::ColouredListItem(name, 0, *I);
		mCurrentPointsList->addItem(item);
		mCurrentPointsListLookup.insert(std::map<AttachPointNode*, CEGUI::ListboxItem*>::value_type(*I, item));
		
	}

}




// AttachPointHolder::AttachPointHolder(const Carpenter::AttachPoint* attachPoint)
// : mAttachPoint(attachPoint)
// {
// }




bool JesusEdit::CurrentPointsList_SelectionChanged( const CEGUI::EventArgs & args )
{
	if (mCurrentlySelectedAttachPointNode) {
		mCurrentlySelectedAttachPointNode->deselect();
		mCurrentlySelectedAttachPointNode = 0;
	}
	CEGUI::ListboxItem* item = mCurrentPointsList->getFirstSelectedItem();
	if (item) {
		AttachPointNode* pointNode = static_cast<AttachPointNode*>(item->getUserData());
		mCurrentlySelectedAttachPointNode = pointNode;
		
		pointNode->select();
	}
	updateBindingButton( );
	return true;

}



bool JesusEdit::NewPointsList_SelectionChanged( const CEGUI::EventArgs & args )
{
	CEGUI::ListboxItem* item = mNewPointsList->getFirstSelectedItem();
	if (item) {
		const Carpenter::AttachPoint* point = getSelectedPointForNewBlock();
		mPreview->selectAttachPoint(point);
	}

	updateBindingButton( );
	return true;
}




void JesusEdit::updateCreateButton( )
{
	if (mBindings.size() > 1) {
		mCreate->setEnabled(true);
	} else {
		mCreate->setEnabled(false);
	}
}

void JesusEdit::updateRemoveButton( )
{
	bool enable =  getSelectedBlock() != 0 && mCurrentConstruction->getBluePrint()->isRemovable(getSelectedBlock()->getBuildingBlock());
	mRemove->setEnabled(enable);
}


void JesusEdit::updateBindingButton( )
{
	const Carpenter::AttachPoint * currentPoint = getSelectedPointForCurrentBlock();
	const Carpenter::AttachPoint * newPoint = getSelectedPointForNewBlock();
	bool enableButton = false;
	if (currentPoint && newPoint)
	{
		//make sure both point are of the same type, and have inverted normals
		if (currentPoint->getAttachPair()->getType() == newPoint->getAttachPair()->getType()) {
			enableButton = true;
			//TODO: check the normals
/*			if (currentPoint->getNormal().inverse() == newPoint->getNormal())
			{
				enableButton = true;
			}*/
		}
	}
	mBind->setEnabled(enableButton);
	
}

const Carpenter::AttachPoint * JesusEdit::getSelectedPointForCurrentBlock( ) const
{
	if (mCurrentlySelectedAttachPointNode) {
		return mCurrentlySelectedAttachPointNode->getAttachPoint();
	}
	return 0;
}

const Carpenter::AttachPoint * JesusEdit::getSelectedPointForNewBlock( ) const
{
	CEGUI::ListboxItem* item = mNewPointsList->getFirstSelectedItem();
	if (item) {
		ConstWrapper<const Carpenter::AttachPoint*>* holder = static_cast< ConstWrapper<const Carpenter::AttachPoint*>* >(item->getUserData());
		const Carpenter::AttachPoint* point = holder->mValue;
		return point;
	} else {
		return 0;
	}

}

bool JesusEdit::Create_Click( const CEGUI::EventArgs & args )
{
	Carpenter::BuildingBlockDefinition definition;
	definition.mName = mNewName->getText().c_str();
	if (definition.mName == "") {
		std::stringstream ss;
		ss << mCurrentConstruction->getModelBlocks().size();
		definition.mName = std::string("_buildingBlock") + ss.str();
	}	
	definition.mBuildingBlockSpec = getNewBuildingBlockSpec()->getName();
	
	Carpenter::BuildingBlock* bblock = mCurrentConstruction->getBluePrint()->createBuildingBlock(definition);
	
	std::vector< Carpenter::BuildingBlockBinding* > bindings = createBindingsForNewBlock(bblock);
	mCurrentConstruction->getBluePrint()->placeBindings(bblock, createBindingsForNewBlock(bblock));
	if (bblock->isAttached()) {
		mCurrentConstruction->createModelBlock(bblock, true);
	}
	removeBindings();
	loadConstruction(mCurrentConstruction);
	return true;
}

bool JesusEdit::Remove_Click( const CEGUI::EventArgs & args )
{
	if (mCurrentlySelectedBlock != 0) {
		mCurrentConstruction->remove(getSelectedBlock());
		mCurrentlySelectedBlock = 0;
		removeBindings();
		loadConstruction(mCurrentConstruction);
	}
	return true;
}

const Carpenter::BuildingBlockSpec * JesusEdit::getNewBuildingBlockSpec( ) const
{
	CEGUI::ListboxItem* item = mAvailableBlocksList->getFirstSelectedItem();
	if (item) {
		ConstWrapper<const Carpenter::BuildingBlockSpec*>* holder = static_cast< ConstWrapper<const Carpenter::BuildingBlockSpec*>* >(item->getUserData());
		const Carpenter::BuildingBlockSpec* bblockSpec = holder->mValue;
		return bblockSpec;
	} else {
		return 0;
	}


}

std::vector< Carpenter::BuildingBlockBinding* > JesusEdit::createBindingsForNewBlock( Carpenter::BuildingBlock * newBlock )
{
	std::vector< Carpenter::BuildingBlockBinding* > blockBindings;
	std::map<AttachPointNode*, const Carpenter::AttachPoint*>::iterator I = mBindings.begin();
	std::map<AttachPointNode*, const Carpenter::AttachPoint*>::iterator I_end = mBindings.end();
	
	for (;I != I_end; ++I)
	{
		Carpenter::BuildingBlockBindingDefinition def;
		Carpenter::BuildingBlockBinding* binding = mCurrentConstruction->getBluePrint()->addBinding(getSelectedBlock()->getBuildingBlock(), I->first->getAttachPoint(), newBlock, I->second);
		if (binding) {
			blockBindings.push_back(binding);
		} 
	}
	return blockBindings;
}

bool JesusEdit::Bind_Click( const CEGUI::EventArgs & args )
{
	if (mCurrentlySelectedAttachPointNode && getSelectedPointForNewBlock()) {
		mBindings.insert(std::map<AttachPointNode*, const Carpenter::AttachPoint*>::value_type(mCurrentlySelectedAttachPointNode, getSelectedPointForNewBlock()));
	}
	CEGUI::ListboxItem* item;
	item = mNewPointsList->getFirstSelectedItem();
	std::stringstream ss;
	ss << mBindings.size();
	item->setText(item->getText() + " (" + ss.str() + ")");
	item = mCurrentPointsList->getFirstSelectedItem();
	item->setText(item->getText() + " (" + ss.str() + ")");

	updateCreateButton();
	return true;
}

Construction* JesusEdit::createNewConstructionFromBlueprint(Carpenter::BluePrint* blueprint)
{
	AvatarCamera* camera = EmberOgre::getSingleton().getMainCamera();

	//create a new node on the same "level" as the avatar
	Ogre::SceneNode* node =  EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode();
	
	Construction* construction = new Construction(blueprint, mJesus, node);
	construction->buildFromBluePrint(true);
	
	//place the node in front of the avatar
	Ogre::Vector3 o_vector(0,0,-5);
	Ogre::Vector3 o_pos = camera->getPosition() + (camera->getOrientation(false) * o_vector);
	node->setPosition(o_pos);
	
	//for now, don't rotate the construction, there are some bugs
	//node->setOrientation(camera->getOrientation());

	return construction;

}

JesusEditPreview::JesusEditPreview(GUIManager* guiManager, Jesus* jesus)
: mGuiManager(guiManager), mBlueprint(0), mConstruction(0), mJesus(jesus), mSelectedAttachPointNode(0),mMinCameraDistance(0.5), mMaxCameraDistance(40)
{
	mPreviewWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout(mGuiManager->getLayoutDir() + "JesusEditPreview.widget", "JesusEditPreview/");
	
	guiManager->getMainSheet()->addChildWindow(mPreviewWindow);
	createPreviewTexture();
	setVisible(false);
}

JesusEditPreview::~JesusEditPreview()
{
	delete mBlueprint;
	delete mConstruction;
}

void JesusEditPreview::setVisible(bool visible)
{
	mPreviewWindow->setVisible(visible);
	mTexture->getRenderContext()->setActive(visible);
}

void JesusEditPreview::showBuildingBlock(const std::string & spec)
{

//make sure to delete the old blueprint and construction
//it's a bit of resource waste, but it's ok
	delete mBlueprint;
	delete mConstruction;
	mSelectedAttachPointNode = 0;
	//delete mModelBlock;

	mBlueprint = new Carpenter::BluePrint("preview", mJesus->getCarpenter());
	mConstruction = new Construction(mBlueprint, mJesus, mTexture->getRenderContext()->getSceneNode());
	
	Carpenter::BuildingBlockDefinition def;
	def.mName = "preview";
	def.mBuildingBlockSpec = spec;
	mBlock = mBlueprint->createBuildingBlock(def);
	
	mModelBlock = mConstruction->createModelBlock(mBlock, true);
	mModelBlock->select();
	
	mTexture->getRenderContext()->repositionCamera();
	mTexture->getRenderContext()->showFull(mModelBlock->getModel());
	

}

void JesusEditPreview::setZoom(float value)
{
	mTexture->getRenderContext()->setCameraDistance(mTexture->getRenderContext()->getDefaultCameraDistance() * value);
/*	Ogre::Real newDistance = (mMaxCameraDistance * mMinCameraDistance) * value;
	Ogre::Vector3 position = mTexture->getCamera()->getPosition();
	position.z = -newDistance;
	mTexture->getCamera()->setPosition(position);*/
}

bool JesusEditPreview::Zoom_ValueChanged(const CEGUI::EventArgs& args)
{
	setZoom(mZoomSlider->getCurrentValue());
	return true;
}


void JesusEditPreview::selectAttachPoint(const Carpenter::AttachPoint* point)
{
	if (mSelectedAttachPointNode) {
		mSelectedAttachPointNode->deselect();
		mSelectedAttachPointNode = 0;
	}
	
	std::vector<AttachPointNode*> nodes = mModelBlock->getAttachPointNodes();
	for (std::vector<AttachPointNode*>::iterator I = nodes.begin(); I != nodes.end(); ++I) 
	{
		if ((*I)->getAttachPoint() == point) {
			mSelectedAttachPointNode = (*I);
			break;
		}
	}
	if (mSelectedAttachPointNode)
	{
		mSelectedAttachPointNode->select();
	}

}

void JesusEditPreview::createPreviewTexture()
{
	CEGUI::GUISheet* imageWidget = static_cast<CEGUI::GUISheet*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEditPreview/Image"));
	mTexture = new EntityCEGUITexture(imageWidget->getName().c_str(), 256, 256);
	imageWidget->setProperty("image", "set:" + mTexture->getImage()->getImagesetName() + " image:" + mTexture->getImage()->getName());

	
	mZoomSlider = static_cast<CEGUI::Slider*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEditPreview/Zoom"));
	BIND_CEGUI_EVENT(mZoomSlider, CEGUI::Slider::EventValueChanged, JesusEditPreview::Zoom_ValueChanged);

}


JesusEditFile::JesusEditFile(GUIManager* guiManager, JesusEdit* jesusEdit, Jesus* jesus) : mJesusEdit(jesusEdit), mJesus(jesus)
{
	mWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout(guiManager->getLayoutDir() + "JesusEditFile.widget", "JesusEditFile/");

	mBluePrintList = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEditFile/Blueprints"));
/*	BIND_CEGUI_EVENT(mBluePrintList, CEGUI::Listbox::EventSelectionChanged, JesusEditFile::BluePrintList_SelectionChanged)
	BIND_CEGUI_EVENT(mBluePrintList, CEGUI::Listbox::EventListContentsChanged, JesusEditFile::BluePrintList_SelectionChanged)*/
	
	
	mLoadButton = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEditFile/Load"));
	BIND_CEGUI_EVENT(mLoadButton, CEGUI::ButtonBase::EventMouseClick, JesusEditFile::Load_Click)
	mSaveButton = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEditFile/Save"));
	BIND_CEGUI_EVENT(mSaveButton, CEGUI::ButtonBase::EventMouseClick, JesusEditFile::Save_Click)

	mNewNameEditBox = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEditFile/NewName"));
	
	guiManager->getMainSheet()->addChildWindow(mWindow);
		
	fillBluePrintList();

}

void JesusEditFile::fillBluePrintList()
{
	mBluePrintList->resetList();
	
	const std::map<std::string, Carpenter::BluePrint* >* blueprintMap = mJesus->getAllBluePrints();
	
	std::map<std::string, Carpenter::BluePrint* >::const_iterator I = blueprintMap->begin();
	std::map<std::string, Carpenter::BluePrint* >::const_iterator I_end = blueprintMap->end();
	
	for (; I != I_end; ++I)
	{
		CEGUI::String name(I->first);
		CEGUI::ListboxItem* item = new Gui::ColouredListItem(name, 0, 0);
		mBluePrintList->addItem(item);
		
	}
	
	
	
}
	
bool JesusEditFile::Load_Click( const CEGUI::EventArgs & args )
{
	CEGUI::ListboxItem* item = mBluePrintList->getFirstSelectedItem();
	if (item) {
		std::string name(item->getText().c_str());
	
		Carpenter::BluePrint* blueprint = mJesus->getBluePrint(name);
		if (blueprint) {
			Construction* construction = mJesusEdit->createNewConstructionFromBlueprint(blueprint);
			//load the construction in jesusEdit
			mJesusEdit->loadConstruction(construction);
		
		}
	}

	return true;
}


bool JesusEditFile::Save_Click( const CEGUI::EventArgs & args )
{

	std::string name(mNewNameEditBox->getText().c_str());
	
	//check that there is a name
	if (name != "") {
		saveBluePrint(name, mJesusEdit->getConstruction()->getBluePrint());
/*		//check if there's already a blueprint saved with that name
		if (mJesus->getAllBluePrints()->find(name) == mJesus->getAllBluePrints().end()) {
		} else {
			//there is a blueprint already, but check if this is defined in the home dir
		}*/
		
	}

	return true;
}

bool JesusEditFile::saveBluePrint(const std::string& name, Carpenter::BluePrint* blueprint )
{
	XMLJesusSerializer serializer(mJesus);
	serializer.saveBlueprintToFile(blueprint, name + ".blueprint.xml", name);
	return true;
}

void JesusEditFile::show()
{
	mWindow->setVisible(true);
	mWindow->moveToFront ();
}

void JesusEditFile::hide()
{
	mWindow->setVisible(false);
}
	
void JesusEditFile::switchVisibility()
{
	if (mWindow->isVisible()) {
		hide();
	} else {
		show();
	}
}


	
bool JesusEdit::CreateNew_Click( const CEGUI::EventArgs & args )
{

	Carpenter::BluePrint* blueprint = new Carpenter::BluePrint("blueprint", mJesus->getCarpenter());
	
	Carpenter::BuildingBlockDefinition definition;
	definition.mName = mNewName->getText().c_str();
	if (definition.mName == "") {
		definition.mName = "startingblock";
	}	
	definition.mBuildingBlockSpec = getNewBuildingBlockSpec()->getName();
	
	
	Carpenter::BuildingBlock* bblock = blueprint->createBuildingBlock(definition);
	blueprint->setStartingBlock(definition.mName);
	blueprint->compile();
		
	Construction* construction = createNewConstructionFromBlueprint(blueprint);
//	construction->createModelBlock(bblock, true);
	removeBindings();
	
	
	//load the construction in jesusEdit
	loadConstruction(construction);
	

	return true;
}

bool JesusEdit::File_Click(const CEGUI::EventArgs& args)
{
	if (mFile)
	{
		mFile->switchVisibility();
	}
	return true;
}
	

}







