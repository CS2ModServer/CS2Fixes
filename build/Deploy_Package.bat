::@echo off

set originalPath=%cd%
ambuild

@echo off
:: copy ~/PyPlugins/ to package/cs2/addons/cs2fixes/PyPlugins
cd ../
cd ../
cd ../
set sourcePath=%cd%/CS2Server-Addons\CS2Fixes\PyPlugins
echo %sourcePath% -- sp
set destinationPath=%cd%/CS2Server-Addons\CS2Fixes\build\package\cs2\addons\cs2fixes\PyPlugins
echo %destinationPath% -- dp
%systemroot%\System32\xcopy "%sourcePath%" "%destinationPath%" /E /I /H /C /Y /Q
::%systemroot%\System32\robocopy "%sourcePath%" "%destinationPath%" *.* /E /COPYALL

set sourcePath=%cd%/CS2Server-Addons\CS2Fixes\resource
echo %sourcePath% -- sp
set destinationPath=%cd%/CS2Server-Addons\CS2Fixes\build\package\cs2\addons\cs2fixes\resource
echo %destinationPath% -- dp
%systemroot%\System32\xcopy "%sourcePath%" "%destinationPath%" /E /I /H /C /Y /Q
::%systemroot%\System32\robocopy "%sourcePath%" "%destinationPath%" *.* /E /COPYALL

set sourcePath=%cd%/CS2Server-Addons\CS2Fixes\build\package\cs2
echo %sourcePath% -- sp
set destinationPath=%cd%\CS2Server\cs2\game\csgo
echo %destinationPath% -- dp
%systemroot%\System32\xcopy "%sourcePath%" "%destinationPath%" /E /I /H /C /Y /Q
::%systemroot%\System32\robocopy "%sourcePath%" "%destinationPath%" *.* /E /COPYALL

cd %originalPath%
echo %time%