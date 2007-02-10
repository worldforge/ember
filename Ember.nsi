; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "Ember-0.4.2"

; The file to write
OutFile "Ember-0.4.2.exe"

; The default installation directory
InstallDir $PROGRAMFILES\Worldforge\Ember

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Ember" "Install_Dir"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Ember (required)"


  SectionIn RO

  SetOutPath $APPDATA\Ember
  File /oname=ogre.cfg etc\ember\ogre.cfg 
  File /oname=plugins.cfg etc\ember\plugins.cfg 

  SetOutPath $APPDATA\Ember\ember-media-0.4.2
  File /r media\user\*.*
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put files there
  File /r "*.*"
;  File "*.*"
 


  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Ember "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ember" "DisplayName" "Ember"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ember" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ember" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ember" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Ember"
  CreateShortCut "$SMPROGRAMS\Ember\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Ember\Ember.lnk" "$INSTDIR\Ember.bat" "" "$INSTDIR\ember.ico"
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ember"
  DeleteRegKey HKLM SOFTWARE\Ember

  ; Remove files and uninstaller
  Delete $INSTDIR\Ember.nsi
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\*.*
  RMDir /r $INSTDIR

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Ember\*.*"
	
  ; Remove directories used
  RMDir "$SMPROGRAMS\Ember"
  RMDir "$INSTDIR"

SectionEnd
