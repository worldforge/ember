# Microsoft Developer Studio Project File - Name="dime" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=dime - Win32 Debug CppUnit
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dime.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dime.mak" CFG="dime - Win32 Debug CppUnit"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dime - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "dime - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "dime - Win32 Debug CppUnit" (based on "Win32 (x86) Application")
!MESSAGE "dime - Win32 Release CppUnit" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dime - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I ".\src\\" /I "." /FI"mscprag.h" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"bin_release/dime.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 sdlmain.lib sdl.lib /nologo /subsystem:windows /machine:I386 /out:"bin_release/dime.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "dime - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".\src\\" /I "." /FI"mscprag.h" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"bin/dime.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libsigc.lib sdlmain.lib sdl.lib sdl_image.lib /nologo /subsystem:windows /debug /machine:I386 /out:"bin/dime.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "dime - Win32 Debug CppUnit"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dime___Win32_Debug_CppUnit"
# PROP BASE Intermediate_Dir "dime___Win32_Debug_CppUnit"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_CppUnit"
# PROP Intermediate_Dir "Debug_CppUnit"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".\src\\" /I "." /FI"mscprag.h" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".\src\\" /I "." /FI"mscprag.h" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_CPP_UNIT" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"bin/dime.bsc"
# ADD BSC32 /nologo /o"bin/dime.bsc"
LINK32=link.exe
# ADD BASE LINK32 libsigc.lib sdlmain.lib sdl.lib cppunitcd.lib sdl_image.lib /nologo /subsystem:windows /debug /machine:I386 /out:"bin/dime.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 libsigc.lib sdlmain.lib sdl.lib cppunitcd.lib sdl_image.lib /nologo /subsystem:windows /debug /machine:I386 /out:"bin/dime.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "dime - Win32 Release CppUnit"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dime___Win32_Release_CppUnit"
# PROP BASE Intermediate_Dir "dime___Win32_Release_CppUnit"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_CppUnit"
# PROP Intermediate_Dir "Release_CppUnit"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I ".\src\\" /I "." /FI"mscprag.h" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I ".\src\\" /I "." /FI"mscprag.h" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_CPP_UNIT" /D "_AFXDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"bin_release/dime.bsc"
# ADD BSC32 /nologo /o"bin_release/dime.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sdlmain.lib sdl.lib cppunit.lib /nologo /subsystem:windows /machine:I386 /out:"bin_release/dime.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 sdlmain.lib sdl.lib cppunit.lib /nologo /subsystem:windows /machine:I386 /out:"bin_release/dime.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "dime - Win32 Release"
# Name "dime - Win32 Debug"
# Name "dime - Win32 Debug CppUnit"
# Name "dime - Win32 Release CppUnit"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\main\Application.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\Button.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\config\ConfigService.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main\Dime.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main\DimeServices.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\EventGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\GuiService.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\image\ImageService.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\input\InputDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\input\InputServiceTest.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\logging\LoggingService.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\Panel.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\platform\PlatformService.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\platform\RectangleRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\platform\SDLDrawDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\src\framework\ServiceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\Widget.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\main\Application.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\Application.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\Button.h
# End Source File
# Begin Source File

SOURCE=.\src\services\platform\Color.h
# End Source File
# Begin Source File

SOURCE=.\src\framework\Component.h
# End Source File
# Begin Source File

SOURCE=.\src\framework\ComponentFactory.h
# End Source File
# Begin Source File

SOURCE=.\src\services\config\ConfigService.h
# End Source File
# Begin Source File

SOURCE=.\src\components\cs\CSComponent.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\Dimension.h
# End Source File
# Begin Source File

SOURCE=.\src\services\platform\Dimension.h
# End Source File
# Begin Source File

SOURCE=.\src\main\DimeServices.h
# End Source File
# Begin Source File

SOURCE=.\src\services\platform\DrawDevice.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\EventGenerator.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\GuiService.h
# End Source File
# Begin Source File

SOURCE=.\src\services\image\ImageService.h
# End Source File
# Begin Source File

SOURCE=.\src\services\input\InputDevice.h
# End Source File
# Begin Source File

SOURCE=.\src\services\input\InputMapping.h
# End Source File
# Begin Source File

SOURCE=.\src\services\input\InputService.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\Label.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\LineEdit.h
# End Source File
# Begin Source File

SOURCE=.\src\services\logging\LoggingService.h
# End Source File
# Begin Source File

SOURCE=.\src\services\metaserver\MetaserverService.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\MouseButtonEvent.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\MouseMotionEvent.h
# End Source File
# Begin Source File

SOURCE=.\src\mscprag.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\Panel.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\ParaGuiApplication.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\ParaGuiService.h
# End Source File
# Begin Source File

SOURCE=.\src\services\platform\PlatformService.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\Rectangle.h
# End Source File
# Begin Source File

SOURCE=.\src\services\platform\Rectangle.h
# End Source File
# Begin Source File

SOURCE=.\src\services\platform\RectangleRenderer.h
# End Source File
# Begin Source File

SOURCE=.\src\services\platform\SDLDrawDevice.h
# End Source File
# Begin Source File

SOURCE=.\src\services\server\ServerService.h
# End Source File
# Begin Source File

SOURCE=.\src\framework\Service.h
# End Source File
# Begin Source File

SOURCE=.\src\framework\ServiceManager.h
# End Source File
# Begin Source File

SOURCE=.\src\services\test\TestService.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\TextBox.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\Theme.h
# End Source File
# Begin Source File

SOURCE=.\src\services\gui\Widget.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
