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

#include "../jesus/Jesus.h"
#include <elements/CEGUIPushButton.h>
#include "../GUIManager.h"
#include "../carpenter/Carpenter.h"
#include "../carpenter/BluePrint.h"

namespace EmberOgre {

JesusEdit::JesusEdit()
 : Widget(), mInJesusMode(false), mCurrentConstruction(0), mCurrentlySelectedBlock(0), mCurrentlySelectedAttachPointNode(0)
{
}


JesusEdit::~JesusEdit()
{
}


void JesusEdit::buildWidget()
{
	
	mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"cegui/widgets/JesusEdit.widget", "JesusEdit/");
	mMainWindow->setVisible(true);
	
	
	//bind buttons
	CEGUI::PushButton* switchButton = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEdit/SwitchMode"));
	BIND_CEGUI_EVENT(switchButton, CEGUI::ButtonBase::EventMouseClick, JesusEdit::SwitchMode_Click)
	
	mCreateNew = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEdit/CreateNew"));
	BIND_CEGUI_EVENT(mCreateNew, CEGUI::ButtonBase::EventMouseClick, JesusEdit::CreateNew_Click)
	
	mCreate = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEdit/Create"));
	BIND_CEGUI_EVENT(mCreate, CEGUI::ButtonBase::EventMouseClick, JesusEdit::Create_Click)
	
	mBind = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEdit/Bind"));
	BIND_CEGUI_EVENT(mBind, CEGUI::ButtonBase::EventMouseClick, JesusEdit::Bind_Click)
	
	
	
	mNewName = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEdit/NewName"));
	
	
	
	//bind lists
	mCurrentBlocksList = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEdit/CurrentBlocks"));
	BIND_CEGUI_EVENT(mCurrentBlocksList, CEGUI::Listbox::EventSelectionChanged, JesusEdit::CurrentBlocksList_SelectionChanged)
	BIND_CEGUI_EVENT(mCurrentBlocksList, CEGUI::Listbox::EventListContentsChanged, JesusEdit::CurrentBlocksList_SelectionChanged)
	
	
	mCurrentPointsList = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEdit/CurrentPoints"));
	BIND_CEGUI_EVENT(mCurrentPointsList, CEGUI::Listbox::EventSelectionChanged, JesusEdit::CurrentPointsList_SelectionChanged)
	BIND_CEGUI_EVENT(mCurrentPointsList, CEGUI::Listbox::EventListContentsChanged, JesusEdit::CurrentPointsList_SelectionChanged)
	
	mAvailableBlocksList = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEdit/AvailableBlocks"));
	BIND_CEGUI_EVENT(mAvailableBlocksList, CEGUI::Listbox::EventSelectionChanged, JesusEdit::AvailableBlocksList_SelectionChanged)
	BIND_CEGUI_EVENT(mAvailableBlocksList, CEGUI::Listbox::EventListContentsChanged, JesusEdit::AvailableBlocksList_SelectionChanged)
	
	mNewPointsList = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEdit/NewBlockPoints"));
	BIND_CEGUI_EVENT(mNewPointsList, CEGUI::Listbox::EventSelectionChanged, JesusEdit::NewPointsList_SelectionChanged)
	BIND_CEGUI_EVENT(mNewPointsList, CEGUI::Listbox::EventListContentsChanged, JesusEdit::NewPointsList_SelectionChanged)
	
	
	
	//bind external events
	mMousePicker.EventPickedModelBlock.connect(SigC::slot(*this, &JesusEdit::pickedModelBlock));
	mMousePicker.EventPickedAttachPointNode.connect(SigC::slot(*this, &JesusEdit::pickedAttachPointNode));
	EmberOgre::getSingleton().EventCreatedJesus.connect(SigC::slot(*this, &JesusEdit::createdJesus));
	
	getMainSheet()->addChildWindow(mMainWindow); 

	
}

bool JesusEdit::SwitchMode_Click(const CEGUI::EventArgs& args)
{
	if (!mInJesusMode) {
		mGuiManager->pushMousePicker(&mMousePicker);
	} else {
		mGuiManager->popMousePicker();
	}
	mInJesusMode = !mInJesusMode;
}

void JesusEdit::pickedModelBlock(ModelBlock* modelBlock, const CEGUI::MouseEventArgs&)
{
	if (mCurrentConstruction != modelBlock->getConstruction()) {
		loadConstruction(modelBlock->getConstruction());
	}
	
	CEGUI::ListboxItem* item = mCurrentBlocksListLookup.find(modelBlock)->second;
	mCurrentBlocksList->ensureItemIsVisible(item);
	mCurrentBlocksList->setItemSelectState(item, true);

}

void JesusEdit::loadConstruction(Construction* construction) 
{
	mCurrentConstruction = construction;
	mCurrentBlocksList->resetList();
	mCurrentBlocksList->clearAllSelections();
	std::vector<ModelBlock*> blocks = construction->getModelBlocks();
	
	for (std::vector<ModelBlock*>::iterator I = blocks.begin(); I != blocks.end(); ++I) 
	{
		CEGUI::String name((*I)->getBuildingBlock()->getName());
		CEGUI::ListboxItem* item = new ColoredListItem(name, 0, *I);
		mCurrentBlocksList->addItem(item);
		mCurrentBlocksListLookup.insert(std::map<ModelBlock*, CEGUI::ListboxItem*>::value_type(*I, item));
	}
	
	loadFromJesus(construction->getJesus());
	
}

void JesusEdit::createdJesus(Jesus* jesus)
{
	mMainWindow->setVisible(true);
	loadFromJesus(jesus);
}

void JesusEdit::loadFromJesus(Jesus* jesus)
{
	mAvailableBlocksList->resetList();
	mAvailableBlocksList->clearAllSelections();
	const std::map<const std::string , Carpenter::BuildingBlockSpec >* bblockSpecs = jesus->getCarpenter()->getBuildingBlockSpecs();
	for (std::map<const std::string , Carpenter::BuildingBlockSpec >::const_iterator I = bblockSpecs->begin(); I != bblockSpecs->end(); ++I) 
	{
		ConstWrapper<const Carpenter::BuildingBlockSpec*>* holder = new ConstWrapper<const Carpenter::BuildingBlockSpec*>(&I->second);
		CEGUI::String name(I->second.getName());
		CEGUI::ListboxItem* item = new ColoredListItem(name, 0, holder);
		mAvailableBlocksList->addItem(item);
	}
}


void JesusEdit::pickedAttachPointNode(AttachPointNode* pointNode, const CEGUI::MouseEventArgs&)
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

	return true;
	
}

void JesusEdit::fillAttachPointList(ModelBlock* block)
{	
	mCurrentPointsList->resetList();
	mCurrentPointsList->clearAllSelections();
	
	std::vector<AttachPointNode*> nodes = block->getAttachPointNodes();
	for (std::vector<AttachPointNode*>::iterator I = nodes.begin(); I != nodes.end(); ++I) 
	{
		CEGUI::String name((*I)->getAttachPoint()->getAttachPair()->getName() + "/" + (*I)->getAttachPoint()->getName()+ " ("+(*I)->getAttachPoint()->getAttachPair()->getType() +")");
		CEGUI::ListboxItem* item = new ColoredListItem(name, 0, *I);
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
	updateBindingButton( );
	return true;
}

bool JesusEdit::AvailableBlocksList_SelectionChanged( const CEGUI::EventArgs & args )
{
	const Carpenter::BuildingBlockSpec* bblockSpec = getNewBuildingBlockSpec( );
	if (bblockSpec) {
		fillNewAttachPointList(bblockSpec->getBlockSpec());
	} else {
		mNewPointsList->resetList();
		mNewPointsList->clearAllSelections();
	
	}
	updateCreateButton();
	return true;

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
		CEGUI::ListboxItem* item = new ColoredListItem(name, 0, holder);
		mNewPointsList->addItem(item);
		
	}

}

void JesusEdit::updateCreateButton( )
{
	if (mBindings.size() > 1) {
		mCreate->setEnabled(true);
	} else {
		mCreate->setEnabled(false);
	}
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
	definition.mBuildingBlockSpec = getNewBuildingBlockSpec()->getName();
	
	Carpenter::BuildingBlock* bblock = mCurrentConstruction->getBluePrint()->createBuildingBlock(definition);
	
	//std::vector<Carpenter::BuildingBlockBinding*> bindings = createBindingsForNewBlock(bblock);
	mCurrentConstruction->getBluePrint()->placeBindings(bblock, createBindingsForNewBlock(bblock));
	if (bblock->isAttached()) {
		mCurrentConstruction->createModelBlock(bblock);
	}
	mBindings.clear();
	loadConstruction(mCurrentConstruction);
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
		blockBindings.push_back(binding);
	}
	return blockBindings;
}

bool JesusEdit::Bind_Click( const CEGUI::EventArgs & args )
{
	if (mCurrentlySelectedAttachPointNode && getSelectedPointForNewBlock()) {
		mBindings.insert(std::map<AttachPointNode*, const Carpenter::AttachPoint*>::value_type(mCurrentlySelectedAttachPointNode, getSelectedPointForNewBlock()));
	}
	updateCreateButton();
}

}

bool EmberOgre::JesusEdit::CreateNew_Click( const CEGUI::EventArgs & args )
{
}




