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
#include <stdlib.h>

#include "Console.h"


	Console* Console::_instance = 0;

	Console & Console::getSingleton(void)
	{
	fprintf(stderr, "TRACE - GET SINGLETON - ENTERING\n");
		if(_instance == 0)
			_instance = new Console;
		return *_instance;
	}


	Console::Console(void)
	{
		fprintf(stderr, "TRACE - CONTRUCTOR - ENTERING\n");
		mState = CS_CLOSED;
		fprintf(stderr, "TRACE - CONTRUCTOR - STATE SET\n");
		static_cast<Ogre::Overlay*>(Ogre::OverlayManager::getSingleton().getByName("console"))->setScroll(0, 1);
		fprintf(stderr, "TRACE - CONTRUCTOR - SCROLL SET\n");
		static_cast<Ogre::Overlay*>(Ogre::OverlayManager::getSingleton().getByName("console"))->show();
		fprintf(stderr, "TRACE - CONSTRUCTOR - SHOWN\n");
		int i;
		for(i=0; i<getNumLines(); i++)
		{
			char buf[128];
			sprintf(buf, "console/textline%d", i);
			mTextLine[i] = static_cast<Ogre::TextAreaGuiElement*>(Ogre::GuiManager::getSingleton().createGuiElement("TextArea", buf));

			mTextLine[i]->setVerticalAlignment(Ogre::GVA_BOTTOM);
			mTextLine[i]->setFontName("console");
			mTextLine[i]->setMetricsMode(Ogre::GMM_PIXELS);
			mTextLine[i]->setLeft(BORDER_PADDING);
			mTextLine[i]->setCharHeight(TEXT_HEIGHT);
			mTextLine[i]->setColour(Ogre::ColourValue::White);
			mTextLine[i]->setTop(-TEXT_HEIGHT-BORDER_PADDING - i*TEXT_HEIGHT);
			mTextLine[i]->setCaption("");

			static_cast<Ogre::PanelGuiElement*>(Ogre::GuiManager::getSingleton().getGuiElement("console/panel"))->addChild(static_cast<Ogre::GuiElement*>(mTextLine[i]));
		}


		fprintf(stderr, "TRACE - CONSTRUCTOR - LINES SET\n");

		mTextLine[0]->setColour(Ogre::ColourValue(0.5, 1, 0.3));

		mCursorPos = 0;
		mShiftDown = false;
		mCapsDown = false;

		setLineText(0, ">>> ");

		//mPythonInterpreter = NULL;

		mPrompt = false;
	}

	Console::~Console()
	{
		int i;
		for(i=0; i<getNumLines(); i++)
		{
			char buf[128];
			sprintf(buf, "console/textline%d", i);
			static_cast<Ogre::PanelGuiElement*>(Ogre::GuiManager::getSingleton().getGuiElement("console/panel"))->removeChild(buf);

			Ogre::GuiManager::getSingleton().destroyGuiElement(buf);
		}

		static_cast<Ogre::Overlay*>(Ogre::OverlayManager::getSingleton().getByName("console"))->hide();
	}

	void Console::setLineText(int lineNum, std::string text)
	{
		mTextLine[lineNum]->setCaption(text);
	}

	std::string Console::getLineText(int lineNum) const
	{
		return mTextLine[lineNum]->getCaption();
	}

	void Console::scrollUp(void)
	{
		int i;
		for(i=getNumLines()-1; i>1; i--)
			setLineText(i, getLineText(i-1));
		setLineText(1, "");
	}

	void Console::open(void)
	{
		fprintf(stderr, "TRACE - CONSOLE - CALL TO OPEN METHOD\n");
		static_cast<Ogre::Overlay*>(Ogre::OverlayManager::getSingleton().getByName("console"))->show();
		mState = CS_OPENING;
	}

	void Console::close(void)
	{
		fprintf(stderr, "TRACE - CONSOLE - CALL TO CLOSE METHOD\n");
		mState = CS_CLOSING;
	}

	bool Console::frameStarted(const Ogre::FrameEvent & event)
	{
		if(mState == CS_OPENING)
		{
			fprintf(stderr, "TRACE - CONSOLE - OPENING\n");
			static_cast<Ogre::Overlay*>(Ogre::OverlayManager::getSingleton().getByName("console"))->scroll(0, -2*event.timeSinceLastFrame);
			if(static_cast<Ogre::Overlay*>(Ogre::OverlayManager::getSingleton().getByName("console"))->getScrollY() <= 0)
			{
				fprintf(stderr, "TRACE - CONSOLE - OPENING - FULLY OPEN\n");
				static_cast<Ogre::Overlay*>(Ogre::OverlayManager::getSingleton().getByName("console"))->setScroll(0, 0);
				mState = CS_OPEN;
			} else {
				fprintf(stderr, "TRACE - CONSOLE - OPENING - NOT YET OPEN\n");
			}
		}
		else if(mState == CS_CLOSING)
		{
			static_cast<Ogre::Overlay*>(Ogre::OverlayManager::getSingleton().getByName("console"))->scroll(0, 2*event.timeSinceLastFrame);
			if(static_cast<Ogre::Overlay*>(Ogre::OverlayManager::getSingleton().getByName("console"))->getScrollY() >= 1)
			{
				static_cast<Ogre::Overlay*>(Ogre::OverlayManager::getSingleton().getByName("console"))->setScroll(0, 1);
				mState = CS_CLOSED;
				static_cast<Ogre::Overlay*>(Ogre::OverlayManager::getSingleton().getByName("console"))->hide();
			}
		}

		return true;
	}

	int Console::getNumLines(void) const
	{
		return ((Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight() / 2) - BORDER_PADDING) / TEXT_HEIGHT;
	}

	void Console::keyPressed(Ogre::KeyEvent* e)
	{

		fprintf(stderr, "TRACE - CONSOLE - KEY PRESSED\n");

		// TODO This is all a big mess :(
		if(e->getKey() == Ogre::KC_LSHIFT || e->getKey() == Ogre::KC_RSHIFT)
			mShiftDown = true;
		else if(e->getKey() == Ogre::KC_CAPITAL)
			mCapsDown = true;
		if(mState != CS_CLOSED)
		{

			if(e->getKeyChar() >= 'a' && e->getKeyChar() <= 'z')
			{
				char c[2];
				c[1] = 0;
				c[0] = e->getKeyChar();

				if(mShiftDown ^ mCapsDown)
					c[0] -= 'a' - 'A';

				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKeyChar() == ' ')
			{
				mCommandLine.insert(mCursorPos, " ");
				mCursorPos++;
			}
			else if(e->getKeyChar() >= '0' && e->getKeyChar() <= '9')
			{
				char c[2];
				c[1] = 0;
				c[0] = e->getKeyChar();

				if(mShiftDown)
				{
					if(c[0] == '0')
						c[0] = ')';
					else if(c[0] == '1')
						c[0] = '!';
					else if(c[0] == '2')
						c[0] = '@';
					else if(c[0] == '3')
						c[0] = '#';
					else if(c[0] == '4')
						c[0] = '$';
					else if(c[0] == '5')
						c[0] = '%';
					else if(c[0] == '6')
						c[0] = '^';
					else if(c[0] == '7')
						c[0] = '&';
					else if(c[0] == '8')
						c[0] = '*';
					else if(c[0] == '9')
						c[0] = '(';
				}

				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKeyChar() == '-')
			{
				char c[2];
				c[1] = 0;
				c[0] = e->getKeyChar();
				if(mShiftDown)
					c[0] = '_';
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKeyChar() == '=')
			{
				char c[2];
				c[1] = 0;
				c[0] = e->getKeyChar();
				if(mShiftDown)
					c[0] = '+';
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKeyChar() == '\\')
			{
				char c[2];
				c[1] = 0;
				c[0] = e->getKeyChar();
				if(mShiftDown)
					c[0] = '|';
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKeyChar() == ';' || e->getKey() == Ogre::KC_F1)
			{
				char c[2];
				c[1] = 0;
				c[0] = ';';
				if(mShiftDown)
					c[0] = ':';
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKeyChar() == '\'' || e->getKey() == Ogre::KC_F2)
			{
				char c[2];
				c[1] = 0;
				c[0] = '\'';
				if(mShiftDown)
					c[0] = '"';
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKeyChar() == ',')
			{
				char c[2];
				c[1] = 0;
				c[0] = e->getKeyChar();
				if(mShiftDown)
					c[0] = '<';
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKeyChar() == '.')
			{
				char c[2];
				c[1] = 0;
				c[0] = e->getKeyChar();
				if(mShiftDown)
					c[0] = '>';
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKey() == Ogre::KC_SLASH)
			{
				char c[2];
				c[1] = 0;
				c[0] = '/';
				if(mShiftDown)
					c[0] = '?';
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKey() == Ogre::KC_TAB)
			{
				char c[2];
				c[1] = 0;
				c[0] = ' ';
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKey() == Ogre::KC_BACK)
			{
				if(mCursorPos > 0)
				{
					mCursorPos--;
					mCommandLine.erase(mCursorPos, 1);
				}
			}

			if(e->getKey() == Ogre::KC_RETURN)
			{
				if(!mPrompt)
					write(">>> " + mCommandLine + "\n");
				else
					write("... " + mCommandLine + "\n");
				//mPrompt = mPythonInterpreter->runsource(mCommandLine);
				if(!mPrompt)
					setLineText(0, ">>> _");
				else
					setLineText(0, "... _");
				mCommandLine = "";
				mCursorPos = 0;
			}
			else
			{
				if(!mPrompt)
					setLineText(0, ">>> " + mCommandLine + "_");
				else
					setLineText(0, "... " + mCommandLine + "_");
			}
		}
	}

	void Console::keyReleased(Ogre::KeyEvent* e)
	{
		if(e->getKey() == Ogre::KC_LSHIFT || e->getKey() == Ogre::KC_RSHIFT)
			mShiftDown = false;
		else if(e->getKey() == Ogre::KC_CAPITAL)
			mCapsDown = false;

		if(e->getKey() == Ogre::KC_GRAVE || e->getKey() == Ogre::KC_F12)
		{
			fprintf(stderr, "TRACE - F12 KEY RELEASED\n");
			if(mState == CS_OPEN || mState == CS_OPENING)
				close();
			else
				open();
		}
	}

	void Console::write(std::string output)
	{
		static bool lineStart = true;

		int i;
		for(i=0; i<output.size(); i++)
		{
			if(output.at(i) == '\n')
			{
				if(lineStart == true)
					scrollUp();
				else
					lineStart=true;
			}
			else
			{
				if(lineStart == true)
				{
					scrollUp();
				}
				char c[2];
				c[0] = output.at(i);
				c[1] = 0;
				setLineText(1, getLineText(1) + std::string(c));
				lineStart=false;
			}
		}

		std::cout << output;
	}


