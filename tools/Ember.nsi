;Windows installer config script.
;Usage: -Install NSIS http://sourceforge.net/projects/nsis/files/NSIS%202/2.46/nsis-2.46.zip/download
;       -Put release files into "release" directory.
;       -Run NSIS generator.


; Use Modern UI
!include "MUI2.nsh"

;--------------------------------
;General

  ; The name of the installer
  Name "Ember-0.7.2"

  ; The file to write
  OutFile "Ember-0.7.2.exe"

  ; The default installation directory
  InstallDir $PROGRAMFILES\Worldforge\Ember

  ; Registry key to check for directory (so if you install again, it will 
  ; overwrite the old one automatically)
  InstallDirRegKey HKLM "Software\Ember" "Install_Dir"

  ;Request application privileges for Windows Vista+
  RequestExecutionLevel user

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
  
  ; Put files there
  File /r "release\*.*"

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

Section "Uninstall"
  
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
