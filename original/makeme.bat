@echo off
echo Compiling Code Split Example

:Setup
set ROOT=C:\ultra
set ALLOWMOVE=1
set MOVEFOLDER=Z:\
set FORCEMOVE=0
set ROMREGISTER=%ROOT%\GCC\MIPSE\BIN\nrdc.exe
set ALLOWREGISTER=1


:Setup the environment for N64 compilation
set gccdir=%ROOT%\gcc
set PATH=%ROOT%\gcc\mipse\bin;%ROOT%\usr\sbin;C:\WINDOWS\system32;
set gccsw=-mips3 -mgp32 -mfp32 -funsigned-char -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__
set n64align=on
set DEBUG_MODE=0
goto CheckDebugMode


:CheckDebugMode
for /f "tokens=3" %%a in ('find /i "DEBUG_MODE" debug.h') do if not defined debugcheck set "debugcheck=%%a"
if "%debugcheck%"=="1" (
    echo Debug mode enabled
    set DEBUG_MODE=1
) else if "%debugcheck%"=="0" (
    echo Debug mode disabled
) else (
    echo Unable to retrieve debug mode
)
echo.
goto CheckDirectories


:CheckDirectories
if not exist %ROOT% (
    echo ERROR: Unable to find Ultra64 folder.
    goto Finish
)
if not exist %MOVEFOLDER% (
    echo WARNING: Unable to find ROM folder.
    echo.
    set ALLOWMOVE=0
)
if not exist %ROMREGISTER% (
    echo WARNING: Unable to find NRDC.exe.
    echo.
    set ALLOWREGISTER=0
)
goto MoveFromOut


:MoveFromOut
if not exist out goto make
cd out >nul 2>&1
move *.o .. >nul 2>&1
move *.out .. >nul 2>&1
move *.n64 .. >nul 2>&1
move *.cvt .. >nul 2>&1
cd..
goto Make


:Make
make
set MAKEERROR=%errorlevel%
echo.
goto ErrorCheck


:ErrorCheck
if "%MAKEERROR%"=="2" (
    echo An error occurred during compilation.
)
goto MoveToOut


:MoveToOut
md out >nul 2>&1
move *.o out >nul 2>&1
move *.out out >nul 2>&1
move *.n64 out >nul 2>&1
move *.cvt out >nul 2>&1
if "%MAKEERROR%"=="0" (
	echo Project compiled sucessfully!
    if "%ALLOWMOVE%"=="1" goto MoveToROMFolderPoll
)
goto Finish


:MoveToROMFolderPoll
if "%FORCEMOVE%"=="1" goto MoveToROMFolder
set /P PROMPT=Move the ROM to the ROM Folder[Y/N]?
if /I "%PROMPT%" EQU "N" goto Finish 
if /I "%PROMPT%" EQU "Y" goto MoveToROMFOLDER
goto MoveToROMFolderPoll


:MoveToROMFolder
cd out
for /R "%~dp0out" %%f in (*.n64) do copy /y "%%f" "%MOVEFOLDER%" >nul
if "%FORCEMOVE%"=="0" (
	echo Done! 
) else (
	echo Moving ROM to the ROM folder
)
goto Finish


:Finish
echo.
pause
exit