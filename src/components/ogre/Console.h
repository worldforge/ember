/** Console from Die */

#ifndef Console_H__
#define Console_H__

#include <OgreTextAreaGuiElement.h>
#include <OgreEventListeners.h>
#include <OgreNoMemoryMacros.h>
#include <OgreEventQueue.h>
#include <OgreGuiElement.h>
#include <OgreGuiContainer.h>
#include <OgreGuiManager.h>
#include <OgreInput.h>
#include <OgreKeyEvent.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgrePanelGuiElement.h>
#include <OgreTextAreaGuiElement.h>
#include <OgreNoMemoryMacros.h>
#include <OgreFrameListener.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <stdlib.h>

//class Ember::ConsoleBackend;
#include "framework/ConsoleBackend.h"

//#include "Prerequisites.h"
//#include "PythonInterpreter.h"

namespace EmberOgre {


// The Console is the physical graphical thing that also deals with keyboard input.
// It is not needed on a server. Only needed on a graphical client
class Console : public Ogre::KeyListener, public Ogre::FrameListener
{
    public:
        ~Console();

        static Console & getSingleton(void);


        void write(std::string output);

        bool isOpen(void) const { if(mState==CS_OPEN) return true; return false; };
        bool isClosed(void) const { if(mState==CS_CLOSED) return true; return false; };

        void keyClicked(Ogre::KeyEvent* e) {};
        void keyPressed(Ogre::KeyEvent* e);
        void keyReleased(Ogre::KeyEvent* e);

        bool frameStarted(const Ogre::FrameEvent & event);

        void open(void);
        void close(void);

    private:
        Console(void);

        static Console* _instance;

        enum State { CS_OPEN, CS_CLOSED, CS_OPENING, CS_CLOSING };
        State mState;

        int getNumLines(void) const;
        void setLineText(int lineNum, std::string text);
        std::string getLineText(int lineNum) const;

        void scrollUp(void);

        enum { TEXT_HEIGHT=14 };
        enum { BORDER_PADDING=5 };

        enum { MAX_TEXT_LINES=50 };
        Ogre::TextAreaGuiElement* mTextLine[MAX_TEXT_LINES];

        std::string mCommandLine;
        int mCursorPos;
        bool mShiftDown;
        bool mCapsDown;
        Ember::ConsoleBackend *myBackend;

        bool mPrompt;
};


}

#endif

