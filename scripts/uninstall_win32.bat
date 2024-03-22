@echo off
REM ===========================================================
REM uninstall_windows.bat
REM
REM Written by Renato Fermi <repiazza@gmail.com> in March 2024
REM
REM Script to uninstall Cattie
REM
REM ===========================================================

REM Use PowerShell to get the My Documents folder path
for /f "usebackq tokens=*" %%a in (`powershell -Command "[Environment]::GetFolderPath('MyDocuments')"`) do set "baseDir=%%a\cattie"

set "shortcutName=%APPDATA%\Microsoft\Windows\Start Menu\Programs\cattie.lnk"

REM Checking if user is running as administrator
NET SESSION >nul 2>&1
if %errorLevel% NEQ 0 (
    echo This script must be run as Administrator
    exit /b 1
)

echo.
echo #######################
echo # Uninstalling cattie #
echo #######################

REM Remove the app directory and all its contents
if exist "%baseDir%" (
    rmdir /s /q "%baseDir%"
    echo Removed cattie directory from My Documents.
) else (
    echo Cattie directory not found.
)

REM Remove the shortcut
if exist "%shortcutName%" (
    del /f /q "%shortcutName%"
    echo Removed cattie shortcut.
) else (
    echo Cattie shortcut not found.
)

echo.
echo Cattie was uninstalled successfully!
pause
