;Windows installer config script.
;Usage: -Install NSIS http://sourceforge.net/projects/nsis/files/NSIS%202/2.46/nsis-2.46.zip/download
;       -Put release files into "release" directory.
;       -Run NSIS generator.


; Use Modern UI
!include "MUI2.nsh"
!include "LogicLib.nsh"

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

  !define VCplus_URL64 "https://download.microsoft.com/download/A/8/0/A80747C3-41BD-45DF-B505-E9710D2744E0/vcredist_x64.exe";
  !define VCplus_URL_man "https://www.microsoft.com/en-us/download/details.aspx?id=26999";
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
   Call DownloadRedistributableInstall
    Pop $0
        ${if} $0 == "Success"
            DetailPrint "Continue installation"
        ${ElseIf} $0 == 'Cancel'
            DetailPrint "Please ensure that you have installed VC++ Redistributables."
        ${Else}
            MessageBox MB_OK "An error occured: $0"
            DetailPrint "You can Download vcredist manually from:" 
            DetailPrint ${VCplus_URL_man}  
        ${EndIf}   
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
  
  ;ToDo investigate if we can have another way to read installed status
  ; possible way to read from ReadRegDWORD $0 HKLM Software\Microsoft\VisualStudio\10.0\VC\Runtimes\x64 Installed
  ;try to read Version subkey to R0
  ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}" "Version"

  ;was there error or not?
  IfErrors 0 NoErrors
   
  ;error occured, copy "Error" to R0
  StrCpy $R0 "Error"

  NoErrors:
  
    Exch $R0 
FunctionEnd

Function   DownloadRedistributableInstall
  
  Push $0

  DetailPrint "Beginning download of latest VC++ Redistributable."
  
  NSISdl::download /TIMEOUT=30000 ${VCplus_URL64} "$TEMP\vcredist_x64.exe" 
  
  Pop $0
  DetailPrint "Result: $0"
  
  StrCmp $0 "Success" InstallVCplusplus
  
 StrCmp $0 "Cancel" GiveUpVCplusplus

 
 StrCpy $0 "Error"
 
  InstallVCplusplus:
  DetailPrint "Completed download."
  Pop $0
  ${If} $0 == "cancel"
    MessageBox MB_YESNO|MB_ICONEXCLAMATION \
    "Download cancelled.  Continue Installation?" \
    IDYES NewVCplusplus IDNO GiveUpVCplusplus
  ${EndIf}
  
 
;  TryFailedDownload:
  DetailPrint "Pausing installation while downloaded VC++ installer runs."
  DetailPrint "Installation could take several minutes to complete."
   
   ExecWait '$TEMP\vcredist_x64.exe /passive /norestart'
 
  DetailPrint "Completed VC++ Framework install/update. Removing VC++ installer."
  
   Delete "$TEMP\vcredist_x64.exe"
  
  DetailPrint "VC++ installer removed."
  goto NewVCplusplus
 
 GiveUpVCplusplus:
  Abort "Installation cancelled by user."
 
NewVCplusplus:
  Pop $7
  Pop $6
  Pop $5
  Pop $4
  Pop $3
  Pop $2
  Pop $1
  Pop $0
   
FunctionEnd
