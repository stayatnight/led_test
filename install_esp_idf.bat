@echo off
echo ====================================
echo      ESP-IDF 自动安装脚本
echo ====================================
echo.

echo 正在下载ESP-IDF安装器...
echo 请稍等，这可能需要几分钟时间。
echo.

echo 步骤 1: 下载 ESP-IDF Windows 离线安装器
echo 访问: https://dl.espressif.com/dl/esp-idf/
echo.

echo 或者手动安装步骤：
echo 1. 访问 https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/windows-setup.html
echo 2. 下载 ESP-IDF Windows Installer
echo 3. 运行安装器并选择 ESP-IDF v5.0 或更高版本
echo 4. 安装完成后重新运行 build.bat
echo.

echo 快速安装命令（如果有 Git）：
echo mkdir C:\Espressif
echo cd C:\Espressif
echo git clone --recursive https://github.com/espressif/esp-idf.git
echo cd esp-idf
echo install.bat
echo.

pause
