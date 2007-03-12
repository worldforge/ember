@Echo off
set OLDDIR=%CD%

:EMBER
ember.exe
cd %OLDDIR%
goto END


:END
echo END.