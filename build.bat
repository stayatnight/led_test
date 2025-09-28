@echo off
echo ====================================
echo       LED Test Project Build Script
echo ====================================
echo.

echo Checking ESP-IDF environment...
if exist "C:\Espressif\frameworks\esp-idf-v5.0\export.bat" (
    echo Found ESP-IDF v5.0, setting up environment...
    call "C:\Espressif\frameworks\esp-idf-v5.0\export.bat"
) else if exist "C:\Espressif\frameworks\esp-idf-v5.1\export.bat" (
    echo Found ESP-IDF v5.1, setting up environment...
    call "C:\Espressif\frameworks\esp-idf-v5.1\export.bat"
) else if exist "C:\Espressif\frameworks\esp-idf-v5.2\export.bat" (
    echo Found ESP-IDF v5.2, setting up environment...
    call "C:\Espressif\frameworks\esp-idf-v5.2\export.bat"
) else (
    echo ERROR: ESP-IDF installation not found!
    echo.
    echo Please install ESP-IDF first:
    echo 1. Download ESP-IDF Windows Installer
    echo 2. Or run: install_esp_idf.bat
    echo.
    pause
    exit /b 1
)

echo.
echo Starting project build...
idf.py build

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful!
    echo.
    echo Use the following command to flash to device:
    echo idf.py -p COM3 flash monitor
) else (
    echo.
    echo Build failed! Please check error messages.
)

echo.
pause
