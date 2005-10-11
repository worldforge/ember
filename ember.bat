@Echo off
rem if not exist java.exe (
rem 	echo I couldn't find java, which is needed for the media update. Go to www.java.com to install it. Aborting.
rem 	goto :EOF
rem )
set OLDDIR=%CD%
echo Updating media from the server, this might take a while if you have a slow connection (circa 30Mb).
java -jar "WFUT.jar" dest "%APPDATA%\\Ember" update ember-media
ember.exe
cd %OLDDIR%
