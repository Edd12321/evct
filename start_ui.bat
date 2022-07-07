@echo off

:: This is a helper script that starts the UI for evct.
:: To be used with Cygwin.

set filename=?
if exist bin/Release (
	set filename=bin/Release/gui.py
) else if exist bin/Debug (
	set filename=bin/Debug/gui.py
) else (
	set filename=gui.py
)

setlocal
path=C:\cygwin;C:\cygwin64;%path%
sh -c "export DISPLAY=:0 && ./%filename%"
endlocal