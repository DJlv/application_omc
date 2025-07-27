@echo off
echo ========================================
echo Application OMC - 构建脚本
echo ========================================

:: 设置变量
set PROJECT_DIR=%~dp0
set BUILD_DIR=%PROJECT_DIR%build
set QT_PATH=D:\soft\Qt\6.10.0\msvc2022_64

echo 项目目录: %PROJECT_DIR%
echo 构建目录: %BUILD_DIR%
echo Qt路径: %QT_PATH%

:: 检查Qt路径
if not exist "%QT_PATH%\bin\qmake.exe" (
    echo 错误: 找不到Qt安装路径 %QT_PATH%
    echo 请检查Qt是否正确安装
    exit /b 1
)

:: 创建构建目录
if not exist "%BUILD_DIR%" (
    echo 创建构建目录...
    mkdir "%BUILD_DIR%"
)

:: 进入构建目录
cd /d "%BUILD_DIR%"

:: 配置项目
echo.
echo 配置CMake项目...
cmake .. -G "Visual Studio 17 2022" -A x64
if %ERRORLEVEL% neq 0 (
    echo 错误: CMake配置失败
    exit /b 1
)

:: 构建项目
echo.
echo 构建项目...
cmake --build . --config Debug
if %ERRORLEVEL% neq 0 (
    echo 错误: 项目构建失败
    exit /b 1
)

:: 部署Qt运行时库
echo.
echo 部署Qt运行时库...
"%QT_PATH%\bin\windeployqt.exe" "bin\Debug\application_omc.exe"
if %ERRORLEVEL% neq 0 (
    echo 警告: Qt运行时库部署失败
)

:: 检查可执行文件
if exist "bin\Debug\application_omc.exe" (
    echo.
    echo ========================================
    echo 构建成功！
    echo ========================================
) else (
    echo 错误: 可执行文件未生成
    pause
    exit /b 1
)
echo ========================================
echo 启动程序...    
D:\project\vs-C++\application_omc\build\bin\Debug\application_omc.exe 
echo 程序已启动！
echo ========================================
