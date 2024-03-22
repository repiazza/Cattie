@echo off
REM ===========================================================
REM install_windows.bat
REM
REM Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in October 2023
REM Maintained by Renato Fermi  <repiazza@gmail.com> since March 2024
REM
REM Batch installation script to Cattie
REM 
REM ============================================================

REM Use PowerShell to get the My Documents folder path
for /f "usebackq tokens=*" %%a in (`powershell -Command "[Environment]::GetFolderPath('MyDocuments')"`) do set "baseDir=%%a\cattie"

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
echo # Installing Cattie #
echo #####################

REM Creating the app directory
mkdir "%baseDir%" >nul 2>&1

REM Creating the img directory of cattie
mkdir "%imgDir%" >nul 2>&1

REM Creating the documentation directory
mkdir "%docDir%" >nul 2>&1

REM Install the binary of the software and your autocomplete script
copy "%baseInstallPath%cattie.exe" "%baseDir%" >nul 2>&1

REM Installing the configuration file of the software
@REM copy "cattie.conf" "%baseDir%" >nul 2>&1

REM Create shortcut
set "shortcutTarget=%baseDir%\cattie.exe"
set "shortcutName=%APPDATA%\Microsoft\Windows\Start Menu\Programs\cattie.lnk"
powershell -Command "$ws = New-Object -ComObject WScript.Shell; $s = $ws.CreateShortcut('%shortcutName%'); $s.TargetPath = '%shortcutTarget%'; $s.Save()" >nul 2>&1

REM Copy the images
copy "%baseInstallPath%img\cat2.png" "%imgDir%" >nul 2>&1
copy "%baseInstallPath%img\forward.png" "%imgDir%" >nul 2>&1
copy "%baseInstallPath%img\laser.png" "%imgDir%" >nul 2>&1
copy "%baseInstallPath%img\rotate2.png" "%imgDir%" >nul 2>&1
copy "%baseInstallPath%img\gear.png" "%imgDir%" >nul 2>&1
copy "%baseInstallPath%img\confirm.png" "%imgDir%" >nul 2>&1

icacls "%baseDir%" /grant:r "%USERNAME%:(OI)(CI)F" >nul 2>&1

REM Installing the documentation
@REM copy "man\cattie.1" "%docDir%" >nul 2>&1
@REM copy "LICENSE.gz" "%docDir%" >nul 2>&1
@REM copy "AUTHORS.gz" "%docDir%" >nul 2>&1
@REM copy "NEWS.gz" "%docDir%" >nul 2>&1
@REM copy "README.gz" "%docDir%" >nul 2>&1

echo Cattie was installed successfully!
