@echo off

set scriptsDir=%~dp0
cd /d %scriptsDir%..

:: set VSCMD_DEBUG=1
call "C:\Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

set path=%scriptsDir%;%path%;
set INCLUDE=%scriptsDir%;%INCLUDE%
set LIB=%scriptsDir%;%LIB%