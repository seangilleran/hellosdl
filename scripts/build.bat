@echo off

set outputExe=HelloSDL
set files=..\source\main.cpp
set libs=SDL2.lib SDL2main.lib SDL2_image.lib SDL2_ttf.lib SDL2_mixer.lib SDL2_net.lib

set buildDir=%~dp0..\build
if not exist %buildDir% mkdir %buildDir%
pushd %buildDir%

set objDir=.\obj\
if not exist %objDir% mkdir %objDir%

set compileFlags=/Zi /FC /EHsc /std:c++17 /Fo%objDir% /Fe%outputExe%
set linkFlags=/link /SUBSYSTEM:CONSOLE

cl %compileFlags% %files% %libs% %linkFlags%

:: Copy dependencies...
xcopy /y ..\external\SDL2\lib\x64\SDL2.dll .
xcopy /y ..\external\SDL2_image\lib\x64\SDL2_image.dll .
xcopy /y ..\external\SDL2_image\lib\x64\libpng16-16.dll .
xcopy /y ..\external\SDL2_ttf\lib\x64\SDL2_ttf.dll .
xcopy /y ..\external\SDL2_mixer\lib\x64\SDL2_mixer.dll .
xcopy /y ..\external\SDL2_net\lib\x64\SDL2_net.dll .

popd