@echo off
REM Test building wxWidgets

call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" >nul 2>&1

cd /d "C:\wxWidgets\build\msw"
nmake -f makefile.vc BUILD=release

pause
