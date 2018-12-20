rem CMD ,run "L218OP.bat all" or "L218OP.bat clean"
@echo off
set path=%CD%\build\winmake;%path%
rem make -f build\Makefile --debug=b PROJ=%~n0 CFG=user %1
make -f build\Makefile PROJ=%~n0 CFG=user %1
