@echo off

set scriptsDir=%~dp0
cd /d %scriptsDir%..

:: set VSCMD_DEBUG=1
call "C:\Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

set path=%scriptsDir%;%path%;
set INCLUDE=%scriptsDir%;..\external\SDL2\include;..\external\SDL2_image\include;..\external\SDL2_ttf\include;..\external\SDL2_mixer\include;..\external\SDL2_net\include;%INCLUDE%
set LIB=%scriptsDir%;..\external\SDL2\lib\x64;..\external\SDL2_image\lib\x64;..\external\SDL2_ttf\lib\x64;..\external\SDL2_mixer\lib\x64;..\external\SDL2_net\lib\x64;%LIB%