//
// C++ Implementation: OgreSetup
//
// Description:
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OgreSetup.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#ifdef WIN32
	#include <SDL.h>
	#include <SDL_syswm.h>
	#include <float.h>
	#define _NSIG NSIG //would it perhaps be better to just use NSIG all the time? I don't know whether that's available on *NIX, but mingw only has NSIG
#else
	#include <SDL/SDL.h>
	#include <SDL/SDL_syswm.h>
	#include "framework/binreloc.h"
	#include <GL/glx.h>
#endif
#include "SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManager.h"
// #include "image/OgreILCodecs.h"
#include "framework/Tokeniser.h"

extern "C" {
#include <signal.h>    /* signal name macros, and the signal() prototype */

sighandler_t oldSignals[_NSIG];
}

namespace EmberOgre {



OgreSetup::OgreSetup()
:
mRoot(0),
mRenderWindow(0),
mIconSurface(0),
mSceneManagerFactory(0),
mMainVideoSurface(0)
{
}


OgreSetup::~OgreSetup()
{
}

void OgreSetup::shutdown()
{
	S_LOG_INFO("Shutting down Ogre.");
//	Ogre::ILCodecs::deleteCodecs();
	if (mRoot) {
		if (mSceneManagerFactory) {
			mRoot->removeSceneManagerFactory(mSceneManagerFactory);
			delete mSceneManagerFactory;
			mSceneManagerFactory = 0;
		}

// 		if (mRenderWindow) {
// // 			mRoot->getRenderSystem()->destroyRenderWindow(mRenderWindow->getName());
// 			mRoot->detachRenderTarget(mRenderWindow);
// 			delete mRenderWindow;
// 			mRenderWindow = 0;
// 		}
	}
	delete mRoot;
	mRoot = 0;
	S_LOG_INFO("Ogre shut down.");

	if (mIconSurface) {
		SDL_FreeSurface(mIconSurface);
		mIconSurface = 0;
	}
///we should clean up after us, but the surface seems to be destroyed when the render window is destroyed, so this won't be needed
// 	if (mMainVideoSurface) {
// 		SDL_FreeSurface(mMainVideoSurface);
// 		mMainVideoSurface = 0;
// 	}
	S_LOG_INFO("Shutting down SDL.");
	///there seems to be memory leaks in the SDL_Video subsystem. Perhaps shutting it down explictely will help?
	SDL_VideoQuit();
	SDL_Quit();
	S_LOG_INFO("SDL shut down.");
}


Ogre::Root* OgreSetup::createOgreSystem()
{

// 	const std::string& sharePath(Ember::EmberServices::getSingleton().getConfigService()->getSharedConfigDirectory());
	std::string pluginExtension = ".so";
	mRoot = new Ogre::Root("", "ogre.cfg", "");

	///we will try to load the plugins from series of different location, with the hope of getting at least one right
	std::vector<std::string> pluginLocations;

	Ember::ConfigService* configSrv = Ember::EmberServices::getSingleton().getConfigService();
	if (configSrv->itemExists("ogre", "plugins")) {
		std::string plugins(configSrv->getValue("ogre", "plugins"));
		///if it's defined in the config, use that location first
		if (configSrv->itemExists("ogre", "plugindir")) {
			std::string pluginDir(configSrv->getValue("ogre", "plugindir"));
			pluginLocations.push_back(pluginDir);
		}
	#ifdef __WIN32__
		pluginExtension = ".dll";
		pluginLocations.push_back("."); ///on windows we'll bundle the dll files in the same directory as the executable
	#else
		pluginExtension = ".so";

		#ifdef ENABLE_BINRELOC
		///binreloc might be used
		char* br_libdir = br_find_lib_dir(br_strcat(PREFIX, "/lib"));
		std::string libDir(br_libdir);
		free(br_libdir);
		pluginLocations.push_back(libDir + "/ember/OGRE");
		#endif
		#ifdef OGRE_PLUGINDIR
		///also try with the plugindir defined for Ogre
		pluginLocations.push_back(OGRE_PLUGINDIR);
		#endif
		///enter the usual locations if Ogre is installed system wide, with local installations taking precedence
		pluginLocations.push_back("/usr/local/lib/OGRE");
		pluginLocations.push_back("/usr/lib/OGRE");
	#endif
		Ember::Tokeniser tokeniser(plugins, ",");
		std::string token = tokeniser.nextToken();
		while (token != "") {
			for (std::vector<std::string>::iterator I = pluginLocations.begin(); I != pluginLocations.end(); ++I) {
				std::string pluginPath((*I) + "/" + token + pluginExtension);
				bool success = false;
				try {
					S_LOG_INFO("Trying to load the plugin " << pluginPath);
					mRoot->loadPlugin(pluginPath);
					success = true;
					break;
				} catch (...) {
					S_LOG_INFO("Error when loading plugin '" << token << "' with path '" << pluginPath << "'. This is not fatal, we will continue trying with some other paths.");
				}
				if (!success) {
					S_LOG_WARNING("Error when loading plugin '" << token << "' after trying different parts. We'll continue, but there might be problems later on.");
				}
			}
			token = tokeniser.nextToken();
		}
	}

//	std::vector<Ogre::String> tokens = Ogre::StringUtil::split(dsp, ".");
	Ember::Tokeniser tokeniser();

	// Register image codecs
//    Ogre::ILCodecs::registerCodecs();

	return mRoot;
}

/**
Shut down SDL correctly, else if run in full screen the display might be messed up.
*/
extern "C" void shutdownHandler(int sig)
{
	std::cerr << "Crashed with signal " << sig << ", will try to shut down SDL gracefully. Please report bugs at https://bugs.launchpad.net/ember" << std::endl;
	SDL_Quit();

	if (oldSignals[sig] != SIG_DFL && oldSignals[sig] != SIG_IGN)
	{
		/* Call saved signal handler. */
		oldSignals[sig](sig);
	}
	else
	{
		/* Reraise the signal. */
		signal(sig, SIG_DFL);
		raise(sig);
	}
}


/** Configures the application - returns false if the user chooses to abandon configuration. */
bool OgreSetup::configure(void)
{
	bool suppressConfig = false;
	bool success = false;
	if (Ember::EmberServices::getSingleton().getConfigService()->itemExists("ogre", "suppressconfigdialog")) {
		suppressConfig = static_cast<bool>(Ember::EmberServices::getSingleton().getConfigService()->getValue("ogre", "suppressconfigdialog"));
	}
	if (suppressConfig) {
		success = mRoot->restoreConfig();
	} else {
		success = mRoot->showConfigDialog();
	}

	if(success)
    {
#if __WIN32__
	///this will only apply on DirectX
	///it will force DirectX _not_ to set the FPU to single precision mode (since this will mess with mercator amongst others)
	try {
		mRoot->getRenderSystem()->setConfigOption("Floating-point mode", "Consistent");

	} catch (const Ogre::Exception&)
	{
		///we don't know what kind of render system is used, so we'll just swallow the error since it doesn't affect anything else than DirectX
	}


    mRenderWindow = mRoot->initialise(true, "Ember");

   ///do some FPU fiddling, since we need the correct settings for stuff like mercator (which uses fractals etc.) to work
   	_fpreset();
	_controlfp(_PC_64, _MCW_PC);
	_controlfp(_RC_NEAR , _MCW_RC);

   // Allow SDL to use the window Ogre just created

   // Old method: do not use this, because it only works
   //  when there is 1 (one) window with this name!
   // HWND hWnd = FindWindow(tmp, 0);

   // New method: As proposed by Sinbad.
   //  This method always works.
   HWND hWnd;
   mRenderWindow->getCustomAttribute("WINDOW", &hWnd);

   char tmp[64];
   // Set the SDL_WINDOWID environment variable
   sprintf(tmp, "SDL_WINDOWID=%d", hWnd);
   putenv(tmp);

   if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
    {
      S_LOG_FAILURE("Couldn't initialize SDL:\n\t\t");
      S_LOG_FAILURE(SDL_GetError());
    }

      // if width = 0 and height = 0, the window is fullscreen

      // This is necessary to allow the window to move1
      //  on WIN32 systems. Without this, the window resets
      //  to the smallest possible size after moving.
      SDL_SetVideoMode(mRenderWindow->getWidth(), mRenderWindow->getHeight(), 0, 0); // first 0: BitPerPixel,
                                             // second 0: flags (fullscreen/...)
                                             // neither are needed as Ogre sets these

   static SDL_SysWMinfo pInfo;
   SDL_VERSION(&pInfo.version);
   SDL_GetWMInfo(&pInfo);

   // Also, SDL keeps an internal record of the window size
   //  and position. Because SDL does not own the window, it
   //  missed the WM_POSCHANGED message and has no record of
   //  either size or position. It defaults to {0, 0, 0, 0},
   //  which is then used to trap the mouse "inside the
   //  window". We have to fake a window-move to allow SDL
   //  to catch up, after which we can safely grab input.
   RECT r;
   GetWindowRect(pInfo.window, &r);
   SetWindowPos(pInfo.window, 0, r.left, r.top, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

   ///do some FPU fiddling, since we need the correct settings for stuff like mercator (which uses fractals etc.) to work
   	_fpreset();
	_controlfp(_PC_64, _MCW_PC);
	_controlfp(_RC_NEAR , _MCW_RC);
#else

	/// we start by trying to figure out what kind of resolution the user has selected, and whether full screen should be used or not
	unsigned int height = 768, width = 1024;
	bool fullscreen;

	parseWindowGeometry(mRoot->getRenderSystem()->getConfigOptions(), width, height, fullscreen);

	SDL_Init(SDL_INIT_VIDEO);

	///this is a failsafe which guarantees that SDL is correctly shut down (returning the screen to correct resolution, releasing mouse etc.) if there's a crash.
	atexit(SDL_Quit);
	oldSignals[SIGSEGV] = signal(SIGSEGV, shutdownHandler);
	oldSignals[SIGABRT] = signal(SIGABRT, shutdownHandler);
	oldSignals[SIGBUS] = signal(SIGBUS, shutdownHandler);
	oldSignals[SIGILL] = signal(SIGILL, shutdownHandler);


	///set the window size
//  int flags = SDL_OPENGL | SDL_HWPALETTE | SDL_RESIZABLE | SDL_HWSURFACE;
// 	int flags = SDL_HWPALETTE | SDL_HWSURFACE | SDL_OPENGL;
 	int flags = SDL_OPENGL;

// 	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
// 	bool useAltSwapControl = SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1) != 0;

		// request good stencil size if 32-bit colour
/*        if (colourDepth == 32)
		{
			SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8);
		}*/

	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
	}

	mMainVideoSurface = SDL_SetVideoMode(width, height, 0, flags); // create an SDL window

	SDL_WM_SetCaption("Ember","ember");

	Ogre::NameValuePairList misc;

// 	misc["externalGLControl"] = Ogre::String("True");
	misc["currentGLContext"] = Ogre::String("True");
// 	misc["vsync"] = Ogre::String("No");
	


	/// initialise root, without creating a window
	mRoot->initialise(false);

	mRenderWindow = mRoot->createRenderWindow("MainWindow", width, height, false, &misc);

	///we need to set the window to be active and visible by ourselves, since GLX by default sets it to false, but then activates it upon receiving some X event (which it will never recieve since we'll use SDL).
	///see OgreGLXWindow.cpp
	mRenderWindow->setActive(true);
	mRenderWindow->setAutoUpdated(true);
	mRenderWindow->setVisible(true);
	
	
// 	mRoot->addFrameListener(this);



    ///set the icon of the window
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

/**
An bitmap of the ember icon, to be used for setting the window icon for SDL.
*/
static struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 3:RGB, 4:RGBA */
  unsigned char	 pixel_data[64 * 64 * 3 + 1];
} emberIcon = {
  64, 64, 3,
  "C+\37B)\36?&\31?'\32C+\36D-\40@(\34='\33>(\33C,\40O8+YC7[F8U>1M6&F.\36E,"
  "\35M3!R8&Q7&N3#J/\37F,\35G*\34K.\40Q2$V7*\\<0^=2_>4_?5\\<1R2)R2(]<5hIAjL"
  "BdHA_D;ZB8W@7V?7[F>bOFkYNsaUo^T`OFN>4I8.G5*?,!7$\31:%\33D-$L2*M3*K1(J.&N"
  "/(U7/[<5Z<5R80N5+K1'E-\40B*\37C-\37B.\40@*\35?)\34B,\37H1$N:-T?4M9+B,\40"
  "<%\35>'\36C,\37I2\"O5%R8(S7(R6'N3$N1#O3$S5'W8+X8.Y8.W6-T5*O/$B\"\31?\37\27"
  "F&\37Q4+S8/U;3X@7_E=aI@_I@^JB]KBaQHjZQn^UhYO_PFXI?PC8D7+:.!;-\40C2'H4*F1"
  "'A*!:#\32:#\33E,$Q70X?7\\D<R:0U=3T<1bLCbMC_KBYE;UC8TC7SD9VE;[LCSC:I92E53"
  "G75K95L9/P;/YD9_J@^H=aI>bJ>bG<_D8aD9`C7aC8^@5Z:/Y8/Y:1_?6dF<gJ@dI@`G>^F>"
  "_G?_JAcMDdPGeTKhWNp_Wrd\\l`WeYQaWN]UJZRGZRFe]Qpf[vi`ug^jZQ\\IAP<4M:0J7.I"
  "4,I4,ZB7cMAzf[ye[wf\\wf[q`VhZQ[MESF=WI@VIAUHAPD;OA8SF=UG=VG<WH=ZH;fRFhRF"
  "rYLmTGfK>a@2`?0a=/f@1f<,f=.c:,]3$c7+oOByXL{[NyZOwZOoRGt\\SnVL\\D:V@6R<1]"
  "I?saX\210xn\211|rwmaZSFSL@UPD_[Orm_|vi\177uj\202vk\201sj\201pg\200qfyi_h"
  "XNWG=P8-V?4r^UmZOveZxk_tg\\naXXLDI<5F:3J@9KA:MC;E:2L@9SG@PC9A1'Q<0Y?3fH;"
  "nOBrPBrOBnI8zVEuK7uH2wB+wB*u8!\243\32\24\240!\23\235\30\23r@-pI9g?1hB5lH"
  ";}_T\177cXeF:U8*K/#H.\"L5)XB7`LBTH>D;2=7-<8.=9/EB6HB7KB8UJAbTLqd\\\177rh"
  "\203umzmdpcZF3)@-$aQJUE=`QHl`Ti\\S_RIF:3;/(LB;\\TMaZStngNG@D;4G<7>1,7\"\32"
  "D*\36X:,mOB{\\OsPArL;qJ7pD/o?'v@%u7\32u0\20\1772\22\262\17\4\260\30\3\200"
  "'\11z/\20s/\23m3\35d.\31d5\40mF4\177cSxYLiL@M0$C'\32;!\26@*!A,\";*\":,#<"
  "1(3+\"/%\34:0&5+$,$\37)#\40'\36\32)\35\25:.%I=6H<4MA8\36\21\15\27\14\12A"
  "857-%8.%K@8PC<:*&'\31\25\33\22\17\34\24\20""61*65-64.'$\37C<53&\40*\32\24"
  "=)\"F1'O5%X;*`?.iA.r@+|G.\205K1\217P2\226O/\226H\"\225?\21\232A\21\316\5"
  "\1\316\14\1\243C\26\240M\"\230B\30\220>\27\206>\34\202H+\204V;\204^G\177"
  "YHxSEkG:Y9+J,\40P5*H.&@)\";'!7%\35""4#\33,\35\27#\27\22!\24\17\30\17\15\25"
  "\20\17\22\14\13\31\17\11+!\31;3+1)\40*#\33\30\20\16\23\15\12<74+%\40\34\26"
  "\21\34\23\20(\33\30""6%\".\36\33*\37\34""92,_\\SLLBGF<DB63)\37C3*ZB:sZR\220"
  "~uiJ:iC/nB-wE0\206Q:\233fL\243eF\242Q,\250M\36\251C\17\261G\17\276=\21\351"
  "%\5\3475\10\301G$\260`9\255fC\237U3\220G&\206D&\210Q9\204S>\200UBvL;]3!X"
  "2#]=0eF:X:/U<3R;5S>7J73@/*T>9N:4<+#.\40\31!\31\23%\37\33($!-(%\33\27\24\17"
  "\14\12\"\35\30\35\27\22C>972.)$\40,%#4+(5-+60-72/KFBRMH@93<5-:0'8'\34<'\35"
  "I1%I*\33T0\37U+\30i8!{F.\212V=\236iO\251lN\246Z4\245H\27\262O\31\276T\35"
  "\305R\31\3378\24\376O\0\376m\0\355=\"\332\205Y\323\203]\302lB\263_:\245X"
  "5\226N.\213L.\211ZB\204[FpI5c=)eA1`@0O1!G,!J1)='\40""0\37\31""6\"\34B+$D"
  ",&;'!/\37\27,#\36.'#1+(/,*\31\27\25\15\13\10\10\4\3\13\10\10""6/-6/,FA<]"
  "WQ]VRQLIMKFIFBNGBJ?:?/)A/(I1(M2%V</P6%cD/fA)l@%uC(\212W9\222Y:\234Z7\240"
  "P'\245H\30\262M\27\277U\35\314b+\325g/\354?\37\376_\0\376\261\0\376\5\0\342"
  "s5\331n5\322p>\317\200Z\304zR\271rK\255kD\244kL\250\200j\240ze\205_G\204"
  "cNyXCkL7oTEs^PcPEWG<UD;L;0R@3O=1K;3XJA^QHvh`}tl^VN>7/2.)-*&D?<@:5TNIVOI0"
  "(#'\40\32;/&WI=bN@P9+O6(Q6&Q5'U?3fQC\\D4t[IoK4{O6\214_F\217Y=\244mO\244^"
  ";\256^7\270_4\306g8\323uD\334vC\346\200L\364=\40\376k\0\376\326\0\376D\0"
  "\364E\40\346s9\336uB\320f2\313sG\301qC\262c9\247a@\225R4\214P5\204Q6\200"
  "S:jA*^:#G*\30C.\37G8*H?5JF=<8-A=.C?/A;1LF=G?7WNFhaXQLBFA8841)%!:53=74^XT"
  "mfaQIC<2+A4)WF8U<*U:)W:*]>._@1cJ>ycWhQAfJ9nG3tB)\217[>\214K)\236X5\245Q)"
  "\264Z.\307nB\323wI\333q>\345r;\357s3\3720\22\376y\0\376\326\0\376\263\0\376"
  "\22\0\365o<\353{E\341zH\325qA\306b,\277k=\261gB\252nP\225Y:\234lO\225jL\200"
  "U9mC+a<%cC0`E5WA3RA6YL>J?1RG7WK>RE9UIAZOG^TM>4-2+$\37\34\30\34\31\25=:65"
  "2/D?;UOIXQKXNFXNCVH;XE6R9,S;/gK?^D7[C6aI:pVF\225\200p\207cN\217_F\217S3\232"
  "V3\253a=\270f=\301a2\321n?\332m7\344p8\360v:\365u7\3720\23\376z\0\376\326"
  "\0\376\326\0\376y\0\3726\32\365z=\356\203R\344\204W\326r=\310h5\267d:\254"
  "fC\225Q-\220X6\211V8\206X:yL0rJ7dA1X9+P7+S?6mYL_K=UC5M:1_MCTD;]PGcYQG<49"
  "/'\22\16\14\22\16\13=9640-1,'60*80+7/)1,'80)NA6N;2P=5ZE:^E:cH:iN@oO>wR=\177"
  "M4\211N0\234[;\264tU\271jE\276^0\316g6\332k7\347vA\363\203P\365~F\364w<\371"
  "@\36\376c\0\376\326\0\376\326\0\376\320\0\3767\0\370I!\365z?\355w@\341t="
  "\321g1\300^0\263\\4\240Q+\230R+\227\\;\226fF\213_AxR9{]IxdTeUHK;0]J<gUEe"
  "SFaPDZJ?RB:^QHl_XSF?F<4\24\21\16\11\7\6""421*'%\40\34\27#\36\31*$\36'\40"
  "\35!\33\27""4*$=0'L:/Q<0Y>.sSBqP>yUB\203YB\211Y>\226^?\242b?\257hB\271g;"
  "\311sF\325zJ\337yD\351v:\364\202G\365~C\364s5\364s4\366`-\376$\0\376\311"
  "\0\376\326\0\376\326\0\376\247\0\376\4\0\366e-\366\202J\357\205P\342|F\330"
  "|J\307l:\262X'\246U)\234X.\231c=\221b@\200S6\206bI\204eO\200eSiTFq_Sxg[f"
  "VIhXM]PFVJAdUNvh`cRKZMD\15\13\11\4\4\4-,*%\"\40#\35\32""0)$<50C<8:2,@4,K"
  "=4TB7[C5lN:tP9uL4{J/\200I*\216Q-\246f@\251]2\266`1\306k8\322o:\341}G\354"
  "\200E\366\210L\366\210L\367\212Q\366\212P\367\220X\367\221[\373-\32\376\215"
  "\0\376\326\0\376\326\0\376\326\0\376t\0\3736\36\365\200G\365|@\352m0\335"
  "k2\317f-\302e1\271i:\255i<\241g>\230eA\220aB\202U:yO7jG5fK;r\\Ls_QiWJbPD"
  "aOCM=3P@7[MEI8/I8/\6\5\5\1\1\1.++#\37\36\34\27\24\33\23\20$\33\31""1'$)\""
  "\35*\36\26:&\32I1%Q5(eB-\200V<\216aD\223aA\227[6\236Z2\257h>\277rF\305k7"
  "\323q;\337v<\353y;\365y<\365~B\365z9\365z6\365u.\365x6\365|=\371E\"\376^"
  "\0\376\326\0\376\326\0\376\326\0\376\326\0\376Q\0\371>\34\365u5\364p/\347"
  "g(\332j0\307Z!\275a.\262f7\245c6\234c;\215V4\200L/tF,oG-qQ<cG4L3\"\\F8XC"
  "5RA5K:2:)!C4-,\32\22:(\37\26\26\25\10\10\11""965,'%\"\31\26\"\27\22(\33\26"
  """6)#4(\40""8)\37F0!X<(aB.wP8\213_A\215X6\230^:\242`8\250\\/\267d3\307p="
  "\324u?\337t6\353t2\366p0\376\2\0\365v4\366\177=\365{7\365x3\366~<\365t1\370"
  "M\40\376G\0\376\326\0\376\326\0\376\326\0\376\326\0\376\272\0\376\17\0\366"
  "p4\365|<\364z6\347x;\327o7\306`+\272d4\260h;\235Y/\221T1\206O/\203T7yN5m"
  "I5oP=cF2W<*^F6cOAeRDcPDcSJVC9lYO\23\23\22\25\26\26PLLE?=0%!&\31\24)\32\24"
  """4%\36""5'\35""5$\32F,\33Z:%kG/\177S7\225bA\220R+\234X-\261l?\275qC\306"
  "o;\323t<\335q2\352u0\370d0\376\22\0\376\7\0\366x1\366}5\365p(\364i\36\365"
  "y6\365|=\370U%\376=\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376"
  "?\0\370T&\365q,\365x3\361s0\342o1\323k2\304g4\264b0\241T'\227U2\207H'y@\37"
  "r@\"qF+gA'bA+Y=-Z?.dK:r\\Lp\\P\\JA.\31\20""3\36\25\10\12\11\3\4\4?<=D>=9"
  "/.7-+>3/@3/D4-F0&R8)W4!`4\34p9\36\212M-\231V1\242U,\253S%\301i:\322{I\332"
  "o5\347s6\370d5\3762\0\376\36\0\366`\"\365p)\364o'\364l$\364l$\364j!\364l"
  "$\370H\32\376K\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376^\0"
  "\370=\25\364d\33\364l%\365v3\354e\40\332^\36\312`&\266S\37\237F\30\237X/"
  "\221R,\200I'tD&c7\33`8\37U2\35V9'J/\40K3%fPC{f[\200meR;0:\"\31%''003[Y]M"
  "LN.-/\35\35\36\"\40\40/*(8-)?0'D-#C$\26L\"\17^)\21p.\17\1770\14\226@\30\246"
  "B\24\270G\24\313Q\31\336Y\33\365B\26\376)\0\376|\0\376\3\0\362X\23\362V\20"
  "\362V\17\362W\20\362W\21\362U\15\362S\10\370)\5\376n\0\376\326\0\376\326"
  "\0\376\326\0\376\326\0\376\326\0\376[\0\3672\11\362T\13\362T\13\362W\21\363"
  "\\\25\343S\20\314H\13\266E\20\242>\17\221>\26\200:\27r8\31a1\25P$\14F#\17"
  """5\27\10$\17\3\32\13\3\31\13\5\"\24\15K;4o^WM5-'\17\6\25\25\26\26\26\30"
  "GFJA@A122((&(&$-(%0(\37""3#\31""9\"\25G%\23X+\23i3\25{5\20\214=\23\235?\20"
  "\257B\16\306Q\30\333^!\357i,\373\33\12\376\236\0\376\210\0\372\"\10\362^"
  "\30\362\\\27\362^\33\363b\40\363b\40\363`\37\366?\22\376\24\0\376\270\0\376"
  "\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376E\0\367A\24\363_\34\362"
  "^\32\363]\34\364b$\363m-\340f-\314]*\271Y-\244L%\222D\40\207C#y>$n=(`;(V"
  ":,P7,G2)6%\35,\34\25F4,iWMS8.B&\35\12\11\14\1\1\1,'()##0(%9-&:-&?.$J4%Z;"
  "(Z5\37g:!zA$\221Q-\243W*\261]+\272T\34\314]\40\335e#\354e!\371E\35\376;\0"
  "\376\326\0\376\221\0\373\40\11\365j%\365j$\364h$\364l*\364j'\366U\35\376"
  "\17\0\376\235\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\317"
  "\0\376#\0\366^%\364h\37\364c\32\376\11\0\376\17\0\366f%\360m*\336_\40\317"
  "]%\274S\37\247@\24\237A\30\224?\33\211?\40\204F,w@,i3\35f8%W0\40D#\26D(\37"
  "P943\33\30""0\33\30\25\26\30\10\12\12""3-++\40\35)\32\25""4\"\33@)\40J-\40"
  "T1\37i<%\200L1\212N.\231W0\246[6\257Z1\303f3\321o7\335s8\353w6\366o,\376"
  "\11\0\376\254\0\376\326\0\376\270\0\376\13\0\365k,\365o)\365o-\365r0\365"
  "h!\372(\11\376\200\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376"
  "\326\0\376\256\0\376\10\0\365d\40\366o(\371A\24\376E\0\376\30\0\366X\27\365"
  "g!\354f\40\333\\\32\311V\36\274R!\254H\34\236C\31\223C!\212?\37\207A$\177"
  ">%f,\33T#\21M#\25H(\37D*&\25\1\1\16\0\0\17\21\20\25\26\25""941/$\37'\31\23"
  "/\36\26""8\37\25?\40\25M(\33h:$tB+\203E%\222L&\243R)\264Z,\312n:\325o7\341"
  "i)\362r1\371F!\376X\0\376\326\0\376\326\0\376\323\0\3763\0\367[,\365s2\365"
  "w9\365u7\367Z$\376+\0\376\320\0\376\326\0\376\326\0\376\326\0\376\326\0\376"
  "\326\0\376\326\0\376t\0\3725\25\365p)\366]\31\376\27\0\376\256\0\376!\0\366"
  "^&\365q/\365m'\342b!\322`'\301\\(\252L\37\231B\32\217C\34\207A\37w6\32n4"
  "\34h7#Y1!_>0eL@^I@5\36\23""0\31\14\0\0\0\1\1\1+&$(\35\30/#\31>)\36E)\31I"
  "+\32^8$sA'\177D$\215P.\235R(\255Z*\277d0\315h/\337w;\360\203G\366{<\376\10"
  "\0\376\257\0\376\326\0\376\326\0\376\326\0\376d\0\371@\36\365p.\366z<\366"
  "\200E\3736\35\376}\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376"
  "\326\0\376\322\0\376-\0\370^(\366w3\372@\35\376g\0\376\326\0\376u\0\3735"
  "\33\366z:\365l&\354h$\337t<\317l;\273[,\255W.\224D\33\204@\31\200F#xF)wN"
  "7vT@~cS~k`qbZPC9N?7\2\2\0\1\1\1*&&$\32\27\36\21\15(\26\17""7\34\23E%\27e"
  "8\37xB%\203K*\220I\"\240N\40\257R\37\274R\33\320[\37\343g#\364k#\367A\15"
  "\376H\0\376\326\0\376\326\0\376\326\0\376\326\0\376\207\0\372#\12\364`\32"
  "\364c\34\366_\37\376\23\0\376\300\0\376\326\0\376\326\0\376\326\0\376\326"
  "\0\376\326\0\376\326\0\376\214\0\376\1\0\365j\"\365`\35\376\15\0\376\270"
  "\0\376\326\0\376\322\0\376U\0\371-\14\365i\"\364h\36\344a\34\322Z\35\273"
  "H\20\255L\34\231B\27\207=\24u2\15e*\13h8\35c>'\\A0|i\\\236\222\207\222\205"
  "}dWM\4\3\0\1\1\1.*)!\32\27\33\20\15%\25\17>'\36R7*oI5rA(}=\37\231P-\253X"
  ".\271[+\306[%\332i)\353g\40\365s.\3731\30\376\203\0\376\326\0\376\326\0\376"
  "\326\0\376\326\0\376\223\0\373!\12\364j#\365l'\367G\30\376F\0\376\326\0\376"
  "\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\317\0\376+\0\366I\21\365"
  "j\37\370T\35\3768\0\376\326\0\376\326\0\376\326\0\376\316\0\3763\0\371>\30"
  "\365k$\356b\31\333]\34\312\\#\266O\35\245M\40\226N&\206G$}G+pB(M(\23A%\25"
  "XE6seY`REL<.\26\22\12\5\4\2""2-+'\34\31-\33\23""5!\26J+\36Y8)jC.u<!\211D"
  "!\237X2\262b9\303l>\317e/\341m0\363o*\364a\27\376\3\0\376\241\0\376\326\0"
  "\376\326\0\376\326\0\376\326\0\376\244\0\374\31\10\367Z\37\367h*\3728\30"
  "\376n\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\233"
  "\0\374\36\14\376\3\0\366T\30\370F\27\376L\0\376\326\0\376\326\0\376\326\0"
  "\376\326\0\376\266\0\376\26\0\367U\40\366w3\345i(\324c)\301\\)\261[0\241"
  "Z3\222T1\206Q1\201V=rR>mSBfRCN=2\31\11\5\20\6\5\21\16\7\26\20\12C;6:,$<'"
  "\35C+\37P/\37a6#n?'\203C!\225M$\244S)\262W(\305c/\324b'\345]\25\365i\34\364"
  "^\23\376\3\0\376\247\0\376\326\0\376\326\0\376\326\0\376\326\0\376\316\0"
  "\376S\0\376+\0\376\24\0\373,\21\376\203\0\376\326\0\376\326\0\376\326\0\376"
  "\326\0\376\326\0\376\322\0\376:\0\371N\37\376#\0\376'\0\370O\35\376>\0\376"
  "\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\211\0\376\1\0\366u3\355"
  "h#\332b$\314g3\263V'\245\\4\214H$}@\40|K3uR?nRCiSDbOC6$\32\"\22\12\2\1\1"
  "\25\15\11E837%\35-\34\24""4\37\26<\35\21N\37\14d*\16{5\23\215>\24\233?\23"
  "\261J\27\300K\20\330[\33\355`\27\363U\11\363[\17\373!\10\376\214\0\376\326"
  "\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\303\0\376\35\0\373"
  "!\13\376\224\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376s\0\372"
  ")\10\365c\40\376\13\0\376\203\0\376\23\0\376\15\0\376\267\0\376\326\0\376"
  "\326\0\376\326\0\376\326\0\376\326\0\376E\0\371D\31\365x5\337a!\313^%\260"
  "I\26\240H\33\222F\36}8\25t9\33U.\33U7'mWI\213yl\205p_nVD\0\0\0\2\2\2""0+"
  "*#\30\24$\25\20""1\35\25>!\26V*\30o3\27\200;\30\220C\34\242G\33\267P\36\314"
  "[$\335[\31\362Z\16\364g\36\364f\36\367@\14\376E\0\376\326\0\376\326\0\376"
  "\326\0\376\326\0\376\326\0\376\326\0\376\272\0\376\13\0\373\35\11\376\231"
  "\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376P\0\373#\10\372)"
  "\10\376\5\0\376\266\0\376p\0\3712\21\376C\0\376\322\0\376\326\0\376\326\0"
  "\376\326\0\376\326\0\376\257\0\376\13\0\366w9\350t8\325k2\301a-\260\\/\237"
  "S+\217L*\177C'h;$aA-v^O\213zm\217|n\213uf\22\15\12\15\13\14;65-#!'\27\21"
  "+\23\13""7\30\13S&\22v9\32\206?\33\222>\27\244D\31\273O\32\322`&\345d\37"
  "\364f\35\364d\34\364h\40\364f\40\373\"\12\376\217\0\376\326\0\376\326\0\376"
  "\326\0\376\326\0\376\326\0\376\306\0\376\24\0\373!\14\376\224\0\376\326\0"
  "\376\326\0\376\326\0\376\326\0\376\326\0\376\312\0\376\210\0\376x\0\376\241"
  "\0\376\326\0\376\270\0\376\14\0\373*\22\376\210\0\376\326\0\376\326\0\376"
  "\326\0\376\326\0\376\326\0\376?\0\370Q\"\360\177E\331d)\311g4\274lB\245^"
  "7\215K(\211Q2yL4kL9YE7`QFYJ<lZLF;6XMH\207{vsaZC.&2\32\20""8\33\15H\37\15"
  "a$\11\204:\24\227E\33\260S#\302W\34\321Q\23\350^\30\365p(\365q-\370L\33\376"
  "\7\0\376\23\0\376\23\0\376\245\0\376\326\0\376\326\0\376\326\0\376\326\0"
  "\376\325\0\376J\0\371+\12\376u\0\376\326\0\376\326\0\376\326\0\376\326\0"
  "\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\303\0\376"
  "\23\0\371B\32\376Z\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376"
  "a\0\3719\21\363j&\337g,\316i9\274e:\245V0\223P.\177B%h:$M0#0$\36""3,(.\""
  "\30UF;\27\17\13""8+$od_hWP]H>^E:hI9`7#`(\17\2004\21\2178\17\250C\23\301O"
  "\30\327X\31\354Z\17\364^\21\367E\21\376\15\0\376\230\0\376c\0\3719\22\376"
  "\31\0\376\303\0\376\326\0\376\326\0\376\326\0\376\326\0\376\273\0\376\27"
  "\0\376\24\0\376\276\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376"
  "\326\0\376\326\0\376\326\0\376\313\0\376H\0\371;\32\373.\26\376\206\0\376"
  "\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376C\0\367L\33\365s.\341d"
  "$\311Z$\273b6\250Z4\234Z:\221]C{Q<cE5;,&*&$%\33\25aUL\0\0\0\7\6\5:42/#\37"
  ",\34\25""0\33\23D%\31O)\30X&\17\1773\15\226@\23\253C\21\302K\24\331X\30\360"
  "]\24\366L\14\376\10\0\376\226\0\376\326\0\376\214\0\373#\11\372\"\10\376"
  "\211\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\250\0\376\27"
  "\0\375v\0\373\324\1\373\323\1\372\323\1\374\324\0\375\325\0\376\326\0\376"
  "\326\0\376\316\0\376A\0\3728\31\372,\16\376P\0\376\322\0\376\326\0\376\326"
  "\0\376\326\0\376\326\0\376\263\0\376\15\0\365i&\365};\351z=\325o7\300d5\253"
  "Z0\234U0\213P2vI1eF5K9/:1+\33\24\16@94%\40\35\32\26\23?620!\34*\26\17""3"
  "\31\17H%\27U,\31[)\16y.\11\2238\12\253C\16\303L\21\334W\30\363b\32\372*\13"
  "\376z\0\376\326\0\376\326\0\376\270\0\376\13\0\3709\16\376Z\0\376\326\0\376"
  "\326\0\376\326\0\376\326\0\376\326\0\375\326\1\375\303\1\375\300\3\355\312"
  "\17\245\215!\237\200\15\334\267\3\367\317\0\373\323\0\375\325\0\376\310\0"
  "\3762\0\376-\0\376|\0\376\322\0\376\326\0\376\326\0\376\326\0\376\326\0\376"
  "\323\0\376?\0\371H$\365s/\366~>\355y:\331p6\307h3\262Z*\240Q%\214H!\200S"
  "6qN:cM@XMG\30\25\22\23\21\17;3,-%\37F:45#\34""4\35\23I-\40Y7(d7%j1\27{/\16"
  "\2268\15\253C\20\310M\20\336U\20\367C\17\376>\0\376\324\0\376\326\0\376\326"
  "\0\376\321\0\376*\0\370D\26\376L\0\376\326\0\376\326\0\376\326\0\375\325"
  "\0\376\326\1\376\326\0\375\326\6\352\314.\263\246l569#\40\40;)\1\224v\1\333"
  "\266\0\366\317\0\373\323\0\375\317\0\376\322\0\376\326\0\376\326\0\376\326"
  "\0\376\326\0\376\326\0\376\326\0\376g\0\3724\30\366\200B\365s/\365w3\357"
  "s/\330c$\312j3\267`-\245W*\223P*\201Q5nH1r]Opf_\"\37\33\0\0\0\36\31\23\33"
  "\25\17=502\"\33,\30\17>'\34T7)f?-u>'\212F)\250Q,\265U*\312X\"\336W\26\373"
  "\40\12\376\223\0\376\326\0\376\326\0\376\326\0\376\326\0\376=\0\3729\32\376"
  "n\0\376\326\0\375\325\0\375\326\0\376\326\0\376\326\0\370\323\17\347\316"
  "H\341\327\250mkd\3\3\0'(*\0\1\3\5\2\0<+\0\224v\0\332\265\0\366\317\0\372"
  "\323\0\375\325\0\376\326\0\376\326\0\376\326\0\376\326\0\376\207\0\376\1"
  "\0\365_\34\366z7\366~=\366z6\361u2\336j*\312b'\273b.\254].\226R+\200N0yU"
  "?\202l_\211~wQMI\"!\37\2\1\0\11\7\2.*&\40\27\21\37\20\11,\27\15""8\35\20"
  "C\40\21`)\20v1\21\2319\16\260F\24\304Y!\340e&\374\25\10\376\256\0\376\326"
  "\0\376\326\0\376\326\0\376\326\0\376\200\0\376K\0\375\302\0\375\325\0\376"
  "\326\0\376\326\0\373\324\5\356\314\33\347\317Y\357\342\251\263\255\226\25"
  "\23\12\1\1\0!!#\21\22\22\0\0\0\0\0\0\6\2\0<*\0\230z\0\335\270\0\370\320\0"
  "\373\323\0\375\325\0\376\326\0\376\271\0\376\20\0\370['\365l\"\365d\36\366"
  "p.\366s+\363u.\341o.\316h,\276b.\254Z)\227P%}H)wS;\210qa\227\213\202uniP"
  "NJ\26\24\13\30\30\20""62+&\35\26\"\26\14(\30\14-\27\12""7\27\10V!\10p.\16"
  "\2165\13\2479\10\272C\16\330P\20\371#\7\376\206\0\376\326\0\376\326\0\376"
  "\326\0\376\326\0\376\326\0\375\325\0\376\326\0\376\326\0\367\321\7\347\305"
  "\26\337\3039\352\325s\355\337\246\220\204yGD9\4\3\0\0\0\0\16\16\16##%\0\0"
  "\0\0\0\0\0\0\0\0\0\0\5\2\0>-\0\230{\0\336\271\0\371\321\0\373\324\0\375\211"
  "\0\373%\14\365j#\371M\36\376\14\0\376\15\0\366s1\365{6\343u5\321n2\301h0"
  "\261b0\234X+xG&kF.u^N{mdME>1+%CC934+C@:3+$2'\35""6(\35<(\32D)\33]2\35t;\37"
  "\206<\31\251D\24\277N\30\322V\32\361K\22\376\34\0\376\262\0\376\326\0\376"
  "\326\0\376\326\0\376\326\0\375\325\0\366\316\4\335\272\21\312\260;\332\307"
  "y\346\327\236\343\327\265\32\22\21\203\200\177\6\5\0\1\1\0\0\0\0\2\2\2''"
  ")\6\6\7\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\3\0\0<-\0\246\210\7\360\312\3\375\300"
  "\0\3765\0\376\7\0\376I\0\376\203\0\376A\0\370R\35\364u)\341l&\317f'\272U"
  "\32\257Z'\235W+o;\35_9\"[B1VF<\37\25\14\22\7\1KKA[[Snmea[QRF<A1&6\"\25""7"
  "\36\20Q+\30o:!\204A\40\240A\23\273G\20\313M\20\351^\27\3707\13\376\36\0\376"
  "\231\0\376\326\0\376\326\0\376\326\0\374\324\1\307\245\21\221\201:\245\233"
  "o\264\253\222\302\274\263\252\244\252\253\251\270\36\34\34\1\1\0\0\0\0\0"
  "\0\0\0\0\0\34\34\35\30\30\31\0\0\0\0\0\0\0\0\0\0\0\0\5\5\5\13\14\15\34\34"
  "\36ME/\342\277\10\376\326\0\376\315\0\376\271\0\376\311\0\376>\0\376I\0\371"
  "L\32\362o\"\336`\30\314]\36\275Z\"\252L\30\231M#xD*\\2\35K/\37E5+\"\32\20"
  "/&\36\33\34\23\32\32\25""52/4/'?5)O?2J4%J0!eB0s?&~<\35\217:\21\262D\20\311"
  "U\35\342_\35\364`\24\367>\10\376\2\0\376C\0\376\246\0\376\326\0\374\324\0"
  "\305\245\10""91\10""0,\30;8.GEDfenPPY\17\17\17\6\6\7\2\2\2\0\0\0\0\0\0\12"
  "\12\13,,.\17\17\20\26\26\27\33\33\35\35\36\37\35\35\37\34\34\36\22\22\24"
  "i[\13\362\315\2\375\325\0\376\326\0\376\257\0\376,\0\373:\27\376c\0\372="
  "\23\361p#\337j&\320e,\300\\'\266^/\235S.\206W>\201ZDfM@N?80(\"PGB87*47.:"
  "83+&\35.$\31""7(\35=)\36""8!\23N.\36e8\"t8\32\2007\24\251K\34\304O\24\327"
  "V\25\362k#\365b\27\365h\35\370A\21\376\12\0\376\237\0\373\324\0\346\302\2"
  "O@\1\2\2\0\4\3\0\2\2\1\4\4\4\33\33\34++.((*$$%\40\40!\36\36\40'')DDH//1\40"
  "\40\"\16\16\17\7\7\7\1\1\1\0\0\0\1\1\0\245\213\4\371\322\0\375\325\0\376"
  "\314\0\376#\0\371E\31\376\11\0\376\201\0\373,\15\356q&\335m+\313^$\272V!"
  "\263\\.\231T1o?'iE2H2(9.*\14\11\10\34\33\31(%\35""53(B<53*!1%\35/\40\27""8"
  "#\32@(\33P1#]1\36u:!\205A\"\243L#\300Q\31\321\\\36\350d\37\365k\"\365k\36"
  "\365o$\371<\21\376c\0\374\324\4\334\274\26w_\6\10\4\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\2\2\2\4\4\5\6\6\6\7\7\10\20\20\20**-\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0(\40\0\327\265\0\373\323\0\376\326\0\376\316\0\376<\0\373'\15\376"
  "C\0\376\222\0\373%\15\352p(\333p0\312h/\275e3\263e6\227X3h>%]<(R?4QGB\35"
  "\33\32\17\17\16\25\16\13\33\22\13""8.(6+#B3,Q@7cNDoSFrQAoC0|F.\212M0\237"
  "R-\272S!\315a*\340f%\364z6\365n$\365k\40\365<\24\355|3\350\323g\213|*?7\13"
  "\22\13\0\17\6\0\6\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\27\27\30"
  "\34\34\35\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0v`\0\363\315\0\375\325\0\376"
  "\326\0\376\326\0\376\311\0\376\215\0\376\303\0\376s\0\3728\25\346r.\324j"
  "*\306g0\265^+\260d9\217P-kB)`@.\\I?VNI$!\37\23\21\20I>7C82MD>F:4D7/SB8r\\"
  "R|_S}ZLqD2l6\37w8\33\211E%\245F\30\311f2\330p9\350n0\356l*\320zP\304\207"
  "\220qg_\227\221z'!\0,(\26\7\7\4\0\0\0\10\3\0\13\3\0\3\1\1\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0$$%\14\14\14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\34\22\4\275\233"
  "\1\372\323\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0"
  "\376,\0\363b+\341x:\323r9\307o?\270e9\253_8\213N/a:%O0!<,$4.,\17\17\16\16"
  "\16\15RD>WICgZUXHC?.':&\35P7.hK@zWJ\200VFzE0\177B(\204C%\226B\30\271U%\311"
  "e3\331d)\224Z:\200\177\217\277\277\337\265\263\313\40\37\32\11\10\0\22\21"
  "\17\17\17\16\2\2\2\4\5\5\11\11\12\37\32\31\7\6\6\0\0\0\0\0\0\0\0\0\5\5\5"
  "(()\3\3\3\0\0\0\1\1\1\7\6\6\34\33\36VV[\217\210xK>\30\261\217\1\376\326\0"
  "\376\326\0\376\326\0\376\326\0\376\307\0\376d\0\3730\21\347t0\332r5\312h"
  "/\273_.\255Z-\237S)\211O1X0\33B&\26J8/`WRROLIFC\32\12\7(\31\23L<:J82:%\37"
  """5\37\30A&\37S4)c@4pE7vD/zA'\201@\40\220E\40\252G\26\301R\35\326d,\177A"
  "!\16\16\17$$())/\17\17\20\21\21\22\27\27\31'')\21\21\22\22\22\22\16\16\17"
  "\12\12\12\1\1\1\0\0\0\0\0\0\0\0\0\20\20\21\"\"#\0\0\0\0\0\0\10\6\5%#'st\205"
  "\273\272\323769\0\1\2\14\14\14jP\0\366\203\0\376r\0\376U\0\376\35\0\3727"
  "\23\356i#\337n-\320j.\302c,\265]-\240N\"\225K#\207K+^5!;\37\22/\40\30""9"
  "0+#\37\32\40\36\31\35\14\10\26\6\2<,(F2.E/*K4,V:0X7+\\:-f=,j<'q:!\205E&\231"
  "V2\251U)\275X$\311Q\27\305X\35\40\13\2\0\0\0\0\0\0\4\4\4\5\5\5\16\17\17\31"
  "\31\32\1\1\1\0\0\0\0\0\0\10\3\0\4\1\0\4\1\0\0\0\0\0\0\0$$&\22\22\24\0\0\0"
  "\5\1\0\11\1\0\22\12\2,(!PLJ\25\24\20\11\11\10\23\24\25\5\5\5I\16\5\3232\24"
  "\371?\16\366\\\27\361p$\342f\37\323a\37\305_$\262O\31\260\\/\234Q'\210?\32"
  "\200G)pF1W<0\\KCjb\\he_mlfE6/>0*UEAQ>:@*$3\34\24""7\33\23@!\26R1#a:)c7#h"
  "7\40w?\"\216Q0\242\\3\270b0\307b*\324a\"\1779\21\1\1\0\0\0\0\0\0\0\0\0\0"
  "\23\23\24\11\11\11\0\0\0\0\0\0\35\22\0\241\201\0\234}\0\207j\1oY\2fR\3j["
  "\20iU\10sZ\3\216l\1\251\204\0\233|\0&\40\0\15\13\0\6\6\4\11\11\12\36\37\40"
  "\21\21\22\11\12\13\222E\30\365q$\362g\32\343_\26\322R\14\302L\11\272W\36"
  "\252M\33\234D\27\216C\32}8\23t;\36rJ7\202h\\\211zq\213\202~{wszxr;,%:-'I"
  ";6B2+B-&@(!9\33\23<\33\20L,\37c?.g=+f7!l6\33\200B\"\222L'\247R&\272T\37\305"
  "O\26\277R\31*\21\3\1\0\0\0\0\0\1\1\2\30\31\32\2\3\3\1\0\0\3\1\0\205d\0\373"
  "\323\0\372\322\0\370\320\0\365\316\0\362\314\1\361\313\1\362\314\1\365\316"
  "\0\370\320\0\373\323\0\363\313\0XB\0\0\0\0\0\0\0\0\0\0\21\21\22\1\2\2\0\0"
  "\0\255E\17\362b\27\346h%\326X\25\307Q\21\272P\25\256M\31\235E\30\232N#\217"
  "J(q3\30l3\34`8)jNAkZQ`WP51+(%\40>/*D72L@9M>7bNFpZQmSHpSG{]R}[LsL:j>*s@(\200"
  "D%\207E#\235P'\267^/\275S\32\314[\36\263R\35\212<\21y5\21e\32\14g\36\17o"
  "I\2\220n\0\266\224\0\361\312\0\376\326\0\376\326\0\376\326\0\376\326\0\376"
  "\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\314\246"
  "\0WF\0-!\0\25\20\4\33\25\24\20\5\2""6\23\3\332V\21\345a\33\326U\22\313V\26"
  "\272H\14\263O\33\243F\24\224>\21\2068\16\177;\30n1\26c-\27I$\24K0%ZI?jbY"
  "LHA;81OD=ND=LA<G95TE=cQJgSHkSGqYLtTDqM<g<(p>&}F)\206J*\224O)\256[/\264S\40"
  "\301Y!\315\\\35\332a\34\347i\40\363p%\371?\24\376\31\0\376\210\0\376\316"
  "\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0"
  "\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\326\0\376\316\0\361"
  "\236\0\337D\1\317%\13\324W\25\342Y\21\341V\16\327\\\32\310R\22\275P\26\261"
  "K\25\243C\20\233E\27\234P(\213B\36w6\27g,\23`-\31I$\26C(\34ZI?ypgqoehg]1"
  ")!%\35\26/#\37&\32\30+\37\32&\33\25""0\"\32A,#>(\37O0#c=+mE0xG/\200H*\211"
  "N.\224Q-\243T,\256V)\271Y'\301V\34\320b&\331a\40\345j&\357l#\370Q\24\373"
  "(\14\376%\0\376Y\0\376}\0\376\225\0\376\246\0\376\257\0\376\263\0\376\263"
  "\0\376\260\0\376\251\0\376\231\0\376\206\0\376o\0\376N\0\376#\0\376\4\0\370"
  ">\16\364`\25\352^\24\337X\23\320K\15\310O\23\275L\26\262G\25\251H\32\234"
  "A\27\235J(\221C\"\1778\34u4\32g-\27U%\24A\36\22""4\30\16""4!\32:1,&$\34&"
  "%\32\10\3\1\26\11\4""7,%?3,L<4?0)0\40\26""3\34\20/\26\13<\37\22U.\34Y/\33"
  "k9\"v<!~>\37\205?\35\217A\34\231D\31\250M\37\255E\22\271G\21\306O\26\321"
  "W\31\336b\36\346e\40\355^\21\363I\12\366<\17\370-\14\372!\11\372\27\6\372"
  "\23\5\373\21\4\373\22\6\372\26\11\372\31\11\371\37\11\370*\20\3673\20\366"
  "I\35\364Z\40\363g\37\353a\34\337U\20\327U\20\312O\20\300V\40\261H\26\242"
  "9\14\234;\16\2126\16\2047\25\2014\25\2007\26l(\15X\33\7T!\17D\33\20""0\22"
  "\13$\16\11\"\24\20#\36\34\3\2\2\0\0\0)\36\31*\33\25""3$\36)\27\24,\31\25"
  ".\35\25.\34\22""1\34\21/\31\20""4\36\23<\37\21U1\40T,\27`0\27g1\25m2\24z"
  "9\32\2009\31\215A\34\224@\21\242H\25\261V%\265Q\35\301Y\37\313`*\320[\36"
  "\324U\20\331]\26\337d\36\340d\34\341f\36\341[\21\337X\20\340i$\341f!\340"
  "e\36\341`\30\341e\36\340^\33\333[\31\326X\31\317Z\35\311V\32\303P\26\271"
  "J\25\253@\20\250F\27\235?\22\223:\23\215;\24\2020\13~5\31o+\22f+\23h.\33"
  "a*\27M\"\25@\33\22H+\40VA6fVMng_MMC79/B0,<*&6%\"\40\16\12\31\6\3!\15\6""0"
  "\36\27""8$\33""3\40\27,\30\17+\25\13""5\33\17""4\30\14?\37\20I%\23U-\32c"
  "6\36d2\30n6\31p1\16s.\12|3\21\2037\21\226J\36\250U'\257X)\254N\33\253J\30"
  "\260N\33\260L\31\262I\23\266I\17\275N\23\300W\36\277Y\"\274T\35\275X$\275"
  "^/\275c5\267O\34\261P\40\256X.\240G\35\233E\35\216;\21\2066\17\1772\20\200"
  "7\30\206>\37\203=\35{8\35j.\26a+\26^,\30i4\"m=,g=0W6*Z@6oZO\215\200u\242"
  "\234\222\220\216\203mmaC.+H30R<:C,*7\40\33""8\40\33E/*P93W@8T<2N5*P1'K-#"
  "F(\34T2\40oL6\202\\D\200YAyM5~L.|D${@\40y<\32\202>\32\222N(\232M#\241P&\253"
  "Y/\260X+\261\\.\250O\40\247I\30\263V%\274e7\270c3\261Z+\261^1\262_4\260c"
  ";\247T*\240P(\232P,\226M+\216I*\207J/~A'w:#p3\37~C.\210P=\213XI\200OBuE7"
  "l>1rH;rL@mKBlOCgODn[P\201uh\231\223\211\243\242\226\231\231\214L40M40[@<"
  "T84G+(D)%M3-P92S=6Q>8[G?[D=Q:3R;/YA5eK>lP?sS@uR<zT={S:wJ0sC'wB%\204Q3\207"
  "P3\207L/\220T7\225S4\223Q2\212J*\203B\"\212K+\223Y;\216V7\217[;\225cC\222"
  "]@\214\\AxJ1qE-j@*h>*mA/pC1j=,^2#V*\36W-\"[9/[=5]A8Z=5P4,D,%>(!@,%<+$0\""
  "\34+\37\31""6-%IE?IJDPQJ,\36\34%\27\25""3$!,\34\31%\26\25'\31\27""3$\40:"
  "+);20C96SGBZNH\\OIXKFbUOcTL\\MG`NFfRFmWKoXJv_RpUGoPBsTCtTEoN@pPBnM?gH;bF"
  "9]A4X?1W=0S9+V@3\\E7ZC4O:.B1&<-%7*\"2$\35""7%\36=&\37:$\36""3!\33""1\35\30"
  "/\36\31(\35\31#\33\27#\32\27#\33\30%\35\32(!\35&!\35%\40\35-'%3.,3/,*&%\""
  "\36\34\5\4\2\7\6\5\6\4\3\7\5\4\5\3\2\10\6\5\22\20\17\34\32\31\23\22\20\23"
  "\22\22""20/0.-*'%,)'#!\40)'%@<:\31\26\25?<:732_XW.**D?>'%$\21\21\22)%$:5"
  "3\31\30\27+'%\34\32\30\27\25\24\30\26\25\32\25\24\34\25\23\33\30\27\34\33"
  "\33\27\26\26\27\25\25\30\25\24\27\24\21\22\16\15\24\21\17\20\16\14\30\23"
  "\20\35\26\23\"\31\27*!\37\37\34\32\25\23\21\36\33\31#\34\30'\40\33""40-3"
  "30>?=TUSddaffbYXUgec\215\211\207\230\225\221zuqQIE>6292.(\40\31""2)\"3*#"
  "90*B:3G?8C:3<1+4+#1'\40;1*F;4>6/2)\"90)OFA\\UO[TOUOJNJEOIEVQN_[Vic^mgcle"
  "`d\\W]VPYQKWPHYOG]SMf[Th^XcYRWMGNC;F;2;1)2)!2'\36""9.$G9/SD;WJ@RE;K>4H=4"
  "I@6IC<MLHUXT`ebqus\200\201\200wxu__[`a\\\203\201\177\232\224\221\213\204"
  "\201sjig]\\\\SO",
};


///We'll use the emberIcon struct
	mIconSurface = SDL_CreateRGBSurfaceFrom(emberIcon.pixel_data, 64, 64, 24, 64*3,
                                      rmask, gmask, bmask, 0);
	if (mIconSurface) {
		SDL_WM_SetIcon(mIconSurface, 0);
	}


#endif

		setStandardValues();

		return true;
    }
    else
    {
        return false;
    }
}

void OgreSetup::setStandardValues()
{
	/// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	/// Set default animation mode
	Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_SPLINE);

	///remove padding for bounding boxes
	Ogre::MeshManager::getSingletonPtr()->setBoundsPaddingFactor(0);

	///all new movable objects shall by default be unpickable; it's up to the objects themselved to make themselves pickable
	Ogre::MovableObject::setDefaultQueryFlags(0);
}


EmberPagingSceneManager* OgreSetup::chooseSceneManager()
{
    /// Create new scene manager factory
    mSceneManagerFactory = new EmberPagingSceneManagerFactory();

    /// Register our factory
    Ogre::Root::getSingleton().addSceneManagerFactory(mSceneManagerFactory);

	EmberPagingSceneManager* sceneMgr = static_cast<EmberPagingSceneManager*>(mRoot->createSceneManager(Ogre::ST_EXTERIOR_REAL_FAR, "EmberPagingSceneManager"));

	///We need to call init scene since a lot of components used by the scene manager are thus created
	sceneMgr->InitScene();

	return sceneMgr;
}

void OgreSetup::parseWindowGeometry(Ogre::ConfigOptionMap& config, unsigned int& width, unsigned int& height, bool& fullscreen)
{
	Ogre::ConfigOptionMap::iterator opt = config.find("Video Mode");
	if (opt != config.end()) {
		Ogre::String val = opt->second.currentValue;
		Ogre::String::size_type pos = val.find('x');
		if (pos != Ogre::String::npos) {

			width = Ogre::StringConverter::parseUnsignedInt(val.substr(0, pos));
			height = Ogre::StringConverter::parseUnsignedInt(val.substr(pos + 1));
		}
	}

	///now on to whether we should use fullscreen
	opt = config.find("Full Screen");
	if (opt != config.end()) {
		fullscreen = (opt->second.currentValue == "Yes");
	}

}

bool OgreSetup::frameEnded(const Ogre::FrameEvent & evt)
{
	SDL_GL_SwapBuffers();
	
	return true;
}


}
