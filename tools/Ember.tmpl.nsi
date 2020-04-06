;Windows installer config script.
;Usage: -Install NSIS http://sourceforge.net/projects/nsis/files/NSIS%202/2.46/nsis-2.46.zip/download
;       -Put release files into "release" directory.
;       -Run NSIS generator.


; Use Modern UI
!include "MUI2.nsh"

;--------------------------------
;General

  ; The name of the installer
  Name "Ember"

  ; The file to write
  OutFile "Ember-@VERSION_PACKAGE@.exe"

  ; The default installation directory
  InstallDir $PROGRAMFILES\Worldforge\Ember

  ; Registry key to check for directory (so if you install again, it will 
  ; overwrite the old one automatically)
  InstallDirRegKey HKLM "Software\Ember" "Install_Dir"
  !define MSVS_DIR "d:\MSVS2010"


  ;Request application privileges for Windows Vista+
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------

; Pages

!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\bin\ember.exe"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------


;--------------------------------
;Languages

!insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections
Section "Ember (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ;Check if we have redistributable installed
  Call  CheckRedistributableInstalled
  Pop $R0

  ${If} $R0 == "Error"
    File "${MSVS_DIR}\vcredist_x86.exe" 	
    ExecWait '"$INSTDIR\vcredist_x86.exe"  /passive /norestart'	
  ${EndIf}  

  ; Put files there
  File /r "@CMAKE_INSTALL_PREFIX@\*.*"

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Ember "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ember" "DisplayName" "Ember"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ember" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ember" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ember" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

  CreateShortCut $INSTDIR\Ember.lnk" "$INSTDIR\bin\ember.exe" "" "$INSTDIR\bin\ember.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Desktop Shortcut"
  CreateShortCut "$DESKTOP\Ember.lnk" "$INSTDIR\bin\ember.exe" "" "$INSTDIR\bin\ember.exe"
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\Ember"
  CreateShortCut "$SMPROGRAMS\Ember\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Ember\Ember.lnk" "$INSTDIR\bin\ember.exe" "" "$INSTDIR\bin\ember.exe"
SectionEnd

;--------------------------------
; Uninstaller

Section "un.Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ember"
  DeleteRegKey HKLM SOFTWARE\Ember

  ; Remove files and uninstaller
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\*.*
  RMDir /r $INSTDIR

  Delete $DESKTOP\Ember.lnk

  ; Remove shortcuts, if any
  Delete $DESKTOP\Ember.lnk
  Delete "$SMPROGRAMS\Ember\*.*"
	
  ; Remove directories used
  RMDir "$SMPROGRAMS\Ember"
  RMDir "$INSTDIR"

SectionEnd


Function CheckRedistributableInstalled

  ;{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5} - msvs2010 sp1
  ;{071c9b48-7c32-4621-a0ac-3f809523288f} - Microsoft Visual C++ 2005 Redistributable
  ;{8220EEFE-38CD-377E-8595-13398D740ACE} - Microsoft Visual C++ 2008 Redistributable

  Push $R0
  ClearErrors
   
  ;try to read Version subkey to R0
  ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}" "Version"

  ;was there error or not?
  IfErrors 0 NoErrors
   
  ;error occured, copy "Error" to R0
  StrCpy $R0 "Error"

  NoErrors:
  
    Exch $R0 
FunctionEnd