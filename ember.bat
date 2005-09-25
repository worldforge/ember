@Echo off
set OLDDIR=%CD%
echo Updating media from the server, this might take a while if you have a slow connection.
java -jar "WFUT.jar" dest "%USERPROFILE%\\Application Data\\Ember" update ember-media
ember.exe
cd %OLDDIR%
