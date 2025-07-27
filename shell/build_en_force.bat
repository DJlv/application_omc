@echo off
chcp 65001 >nul
echo ========================================
echo Application OMC - Build and Deploy Script (Force Clean)
echo ========================================

REM Set Qt path
set QT_PATH=D:\soft\Qt\6.10.0\msvc2022_64
set WINDEPLOYQT=%QT_PATH%\bin\windeployqt.exe

echo Cleaning build directory...
if exist build rmdir /s /q build

echo Creating build directory...
mkdir build
cd build

echo Configuring CMake project...
cmake ..

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

echo Building project...
cmake --build .

if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo Deploying Qt runtime libraries...
%WINDEPLOYQT% Debug\application_omc.exe

if %errorlevel% neq 0 (
    echo Qt deployment failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build and deployment completed!
echo ========================================

REM Force clean intermediate files
echo.
echo Force cleaning intermediate files...
cd ..
cd build

echo.
echo Final executable location: D:\project\vs-C++\application_omc\build\Debug\application_omc.exe
echo.
echo To run the program, execute:
echo D:\project\vs-C++\application_omc\build\Debug\application_omc.exe
echo.
taskkill /f /im application_omc.exe
echo taskkill 结束进程成功!
D:\project\vs-C++\application_omc\build\Debug\application_omc.exe