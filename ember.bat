@Echo off
set OLDDIR=%CD%

:WFUT
echo Updating media from the server, this requires java.
java -DserverRoot=http://amber.worldforge.org/WFUT/ -jar "WFUT.jar" dest "%APPDATA%\\Ember" update ember-media-0.4.2
GOTO EMBER

:EMBER
ember.exe
cd %OLDDIR%
goto END


:END
echo END.