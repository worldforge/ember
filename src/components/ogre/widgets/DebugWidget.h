#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H



namespace DimeOgre {


class DebugWidget
: public Widget
{
public:

	DebugWidget(GUIManager* guiManager);
	virtual ~DebugWidget();
	
	void frameStarted(const Ogre::FrameEvent & evt);
	
	void buildWidget();
	
protected:
	CEGUI::Window* mFwnd;
	CEGUI::Window* mSheet;

	void updateStats(void);
	
};

}

#endif // DEBUGWIDGET_H
