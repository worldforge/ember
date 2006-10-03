@Echo off
set OLDDIR=%CD%

:WFUT
echo Updating media from the server, this might take a while if you have a slow connection (circa 30Mb).
java -DserverRoot=http://amber.worldforge.org/WFUT/ -jar "WFUT.jar" dest "%APPDATA%\\Ember" update ember-media-0.4.1
if %errorlevel% LEQ 1 goto EMBER
GOTO ERROR

:EMBER
ember.exe
cd %OLDDIR%
goto END

:ERROR
echo Java is needed for the media update.
echo I couldn't find java, or something failed when updating media.
echo.
echo Go to www.java.com to install Java.
echo Aborting.
pause
goto END


:END
echo END.