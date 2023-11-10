@echo off
REM install_windows.bat: Batch script to install cattie
REM
REM Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
REM
REM Date: 21/10/2023

REM Set the base directory path
set "baseDir=C:\Arquivos de Programas\cattie"
set "imgDir=%baseDir%\img"
set "docDir=%baseDir%\doc"
set "baseInstallPath=..\"

REM Checking if user is running as administrator
NET SESSION >nul 2>&1
if %errorLevel% NEQ 0 (
    echo This script must be run as Administrator
    exit /b 1
)

echo.
echo #####################
echo # Installing cattie #
echo #####################

REM Creating the app directory
mkdir "%baseDir%"

REM Creating the img directory of cattie
mkdir "%imgDir%"

REM Creating the documentation directory
mkdir "%docDir%"

REM Install the binary of the software and your autocomplete script
copy "%baseInstallPath%cattie.exe" "%baseDir%"

REM Installing the configuration file of the software
@REM REM copy "cattie.conf" "%baseDir%"

REM Create shortcut
@REM copy "cattie.lnk" "%APPDATA%\Microsoft\Windows\Start Menu\Programs"
set "shortcutTarget=%baseDir%\cattie.exe"
set "shortcutName=%APPDATA%\Microsoft\Windows\Start Menu\Programs\cattie.lnk"
powershell -Command "$ws = New-Object -ComObject WScript.Shell; $s = $ws.CreateShortcut('%shortcutName%'); $s.TargetPath = '%shortcutTarget%'; $s.Save()"

REM Copy the images
copy "%baseInstallPath%img\cat2.png" "%imgDir%"
copy "%baseInstallPath%img\forward.png" "%imgDir%"
copy "%baseInstallPath%img\laser.png" "%imgDir%"
copy "%baseInstallPath%img\rotate2.png" "%imgDir%"

icacls "%baseDir%" /grant:r "%USERNAME%:(OI)(CI)F"

REM Installing the documentation
@REM copy "man\cattie.1" "%docDir%"
@REM copy "LICENSE.gz" "%docDir%"
@REM copy "AUTHORS.gz" "%docDir%"
@REM copy "NEWS.gz" "%docDir%"
@REM copy "README.gz" "%docDir%"

echo cattie was installed successfully!
