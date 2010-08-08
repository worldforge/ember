-----------------------------------------
-- Start of handler functions
-----------------------------------------

-----------------------------------------
-- Script Entry Point
-----------------------------------------

--is this needed?

local Debug = {}
local debugWidget = guiManager:createWidget()


function Debug_receiveLog(level, line)
	Debug.logTextBox:setText(line .. "\n" + Debug.logTextBox:getText())
end

function Debug_loglevel_SelectionChanged(args)
{
	item = Debug.loglevelDroplist:getSelectedItem()
	if item ~= nil then
		level = (Eris::LogLevel)item->getID();
		Eris::setLogLevel(level);
	end
	return true;

}

function Chat_appendOOGChatLine(line, entity)
	local textWnd = chatWidget:getWindow("TextBox")
	local chatString = textWnd:getText()
	chatString = "{" .. entity:getName() .. "}" .. line .. "\n" .. chatString
	textWnd:setText(chatString)
	
end

function Chat_appendIGChatLine(line, entity)
	local textWnd = chatWidget:getWindow("TextBox")
	local chatString = textWnd:getText()
	chatString = "<" .. entity:getName() .. ">" .. line .. "\n" .. chatString
	textWnd:setText(chatString)
end


debugWidget:loadMainSheet("Debug.layout", "Debug/")
chatWidget:registerConsoleVisibilityToggleCommand("debug")
chatWidget:enableCloseButton();

EmberOgre.LuaConnector:new(guiManager.AppendIGChatLine):connect("Chat_appendIGChatLine")
EmberOgre.LuaConnector:new(guiManager.AppendOOGChatLine):connect("Chat_appendIGChatLine")



























void DebugWidget::buildWidget() 
{
	
	mLogTextBox = static_cast<CEGUI::MultiLineEditbox*>(getWindow("TextBox"));
	mLoglevelDroplist = static_cast<CEGUI::Combobox*>(getWindow("LogLevelDropList"));
	fillListWithLogLevels(mLoglevelDroplist);
	
	BIND_CEGUI_EVENT(mLoglevelDroplist, CEGUI::Combobox::EventListSelectionAccepted, DebugWidget::loglevel_SelectionChanged);
	
	
	Eris::Logged.connect(sigc::mem_fun(*this, &DebugWidget::receiveLog));
	

}

void DebugWidget::fillListWithLogLevels(CEGUI::Combobox* list) 
{

	CEGUI::ListboxItem* item = new ColoredListItem("error", (uint)Eris::LOG_ERROR, 0);
	list->addItem(item);
	item = new ColoredListItem("warning", Eris::LOG_WARNING, 0);
	list->addItem(item);
	item = new ColoredListItem("notice", Eris::LOG_NOTICE, 0);
	list->addItem(item);
	item = new ColoredListItem("verbose", Eris::LOG_VERBOSE, 0);
	list->addItem(item);
	item = new ColoredListItem("debug", Eris::LOG_DEBUG, 0);
	list->addItem(item);
	
}



// void DebugWidget::frameStarted(const Ogre::FrameEvent & evt)
// {
// 	updateStats();
// }

/*void DebugWidget::updateStats(void)
{*/
/*	static CEGUI::String currFps = (CEGUI::utf8*)"Current FPS: ";
	static CEGUI::String avgFps = (CEGUI::utf8*)"Average FPS: ";
	static CEGUI::String bestFps = (CEGUI::utf8*)"Best FPS: ";
	static CEGUI::String worstFps = (CEGUI::utf8*)"Worst FPS: ";
	static CEGUI::String tris = (CEGUI::utf8*)"Triangle Count: ";

	if (mSkipCount >= mUpdateFreq)
	{
		mSkipCount = 0;

		try 
		{
			CEGUI::Window* guiAvg = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPAverageFPS");
			CEGUI::Window* guiCurr = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPCurrentFPS");
			CEGUI::Window* guiBest = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPBestFPS");
			CEGUI::Window* guiWorst = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPWorstFPS");

			const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();

			guiAvg->setText(avgFps + Ogre::StringConverter::toString(stats.avgFPS));
			guiCurr->setText(currFps + Ogre::StringConverter::toString(stats.lastFPS));
			guiBest->setText(bestFps + Ogre::StringConverter::toString(stats.bestFPS)
				+" "+Ogre::StringConverter::toString(stats.bestFrameTime)+" ms");
			guiWorst->setText(worstFps + Ogre::StringConverter::toString(stats.worstFPS)
				+" "+Ogre::StringConverter::toString(stats.worstFrameTime)+" ms");

			CEGUI::Window* guiTris = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPTriCount");
			guiTris->setText(tris + Ogre::StringConverter::toString(stats.triangleCount));

			CEGUI::Window* guiDbg = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPDebugMsg");
			guiDbg->setText(mWindow->getDebugText());
		}
		catch (CEGUI::UnknownObjectException x)
		{
			// just skip if windows are missing
		}

	}
	else
	{
		mSkipCount++;
	}
*/
// }


}

