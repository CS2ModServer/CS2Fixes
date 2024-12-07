@echo off

:: Define the source folder. 
::set "sourcePath=E:\TEST_SERVER\cs2\game\csgo\addons\Source2Py\build\Debug"
::set "sourcePath=E:\CS2Fixes\build\package"
::set "sourcePath=E:\CS2Fixes\package"
for %%I in ("%~dp0\.") do set "root=%%~fI"
set sourcePath=%root%\package
set destinationPath=%root%\..\..\..

%systemroot%\System32\xcopy /E /I /Y "%sourcePath%" "%destinationPath%"

:: Define the destination folder.
::set "DestinationPath=E:\TEST_SERVER\cs2\game\csgo\addons\Source2Py\bin"
::set "DestinationPath=E:\TEST_SERVER\cs2\game\csgo"
::%systemroot%\System32\xcopy /E /I /Y "%sourcePath%" "%destinationPath%"
