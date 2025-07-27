# Application OMC

这是一个基于Qt6的C++应用程序项目。

## 项目结构

```
application_omc/
├── src/                    # 源代码文件
│   ├── main.cpp           # 主程序入口
│   ├── mainwindow.cpp     # 主窗口实现
│   └── mainwindow.ui      # Qt Designer UI文件
├── include/               # 头文件
│   └── mainwindow.h       # 主窗口头文件
├── resources/             # 资源文件（图片、图标等）
├── docs/                  # 文档文件
├── build/                 # 构建输出目录
│   └── Debug/
│       ├── application_omc.exe  # 生成的可执行文件
│       ├── Qt6Cored.dll        # Qt核心库
│       ├── Qt6Widgetsd.dll     # Qt Widgets库
│       ├── Qt6Guid.dll         # Qt GUI库
│       └── [其他Qt DLL和插件]   # 完整的Qt运行时
├── CMakeLists.txt         # CMake构建配置
├── build.bat              # 自动化构建脚本
├── clean_simple.bat       # 简单清理脚本（推荐）
├── clean_safe.bat         # 安全清理脚本
├── clean_force.bat        # 强制清理脚本
├── check_files.bat        # 文件占用检查脚本
└── README.md              # 项目说明文档
```

## 构建说明

### 环境要求
- Qt 6.10.0 或更高版本
- Visual Studio 2022 或更高版本
- CMake 3.19 或更高版本

### 构建步骤

#### 方法1：使用自动化脚本（推荐）
```bash
build.bat
```
这个脚本会：
1. 清理旧的构建目录
2. 配置CMake项目
3. 构建项目
4. 部署Qt运行时库
5. 提供清理选项

#### 方法2：手动构建
1. **确保Qt已正确安装**
   - Qt安装路径：`D:/soft/Qt/6.10.0/msvc2022_64`
   - 如果Qt安装在其他位置，请修改`CMakeLists.txt`中的`CMAKE_PREFIX_PATH`

2. **创建构建目录**：
   ```bash
   mkdir build
   cd build
   ```

3. **配置项目**：
   ```bash
   cmake ..
   ```

4. **构建项目**：
   ```bash
   cmake --build .
   ```

5. **部署Qt运行时库**（重要！）：
   ```bash
   D:\soft\Qt\6.10.0\msvc2022_64\bin\windeployqt.exe Debug\application_omc.exe
   ```

6. **运行程序**：
   ```bash
   ./Debug/application_omc.exe
   ```

## 清理中间文件

### 清理脚本说明

#### clean_simple.bat - 简单清理脚本（推荐）
- 删除所有中间文件和构建目录
- 保留最终的可执行文件和Qt运行时库
- 避免编码问题，使用英文输出

#### clean_safe.bat - 安全清理脚本
- 检查文件占用情况
- 安全删除文件，提供错误处理
- 显示详细的清理报告

#### clean_force.bat - 强制清理脚本
- 强制终止占用文件的进程
- 强制删除所有中间文件
- 适用于顽固文件的情况

#### check_files.bat - 文件占用检查脚本
- 检查哪些进程正在使用文件
- 诊断清理失败的原因
- 提供解决方案建议

### 使用方法

```bash
# 简单清理（推荐）
clean_simple.bat

# 安全清理
clean_safe.bat

# 强制清理（如果其他方法失败）
clean_force.bat

# 检查文件占用情况
check_files.bat
```

### 清理的文件类型
- CMake生成的文件（CMakeCache.txt, CMakeFiles等）
- Visual Studio项目文件（*.vcxproj, *.sln等）
- Qt生成的中间文件（moc_*.cpp, ui_*.h等）
- 编译生成的中间文件（*.obj, *.ilk等）
- 构建目录（application_omc.dir, x64, .qt等）
- 临时文件和日志文件

### 保留的文件
- ✅ application_omc.exe (可执行文件)
- ✅ Qt6*.dll (Qt运行时库)
- ✅ 插件目录 (platforms, styles, imageformats等)
- ✅ translations (翻译文件)

### 解决"Code is already running"错误

如果遇到文件占用错误：

1. **检查文件占用**：
   ```bash
   check_files.bat
   ```

2. **关闭相关程序**：
   - 关闭application_omc.exe
   - 等待编译器完成

3. **使用强制清理**：
   ```bash
   clean_force.bat
   ```

4. **重启计算机**：如果问题持续存在

## 文件分类说明

- **src/**: 包含所有的.cpp源文件和.ui文件
- **include/**: 包含所有的.h头文件
- **resources/**: 存放应用程序资源文件（如图片、图标、样式表等）
- **docs/**: 存放项目文档
- **build/**: CMake构建输出目录（自动生成）

## 构建状态

✅ **构建成功** - 项目已成功配置并构建，生成的可执行文件位于 `build/Debug/application_omc.exe`

✅ **部署完成** - Qt运行时库已成功部署，程序可以正常运行

✅ **清理功能** - 提供多种清理脚本，可删除中间文件节省空间

## 常见问题解决

### Qt DLL缺失错误
如果遇到 "找不到 Qt6Cored.dll" 或 "找不到 Qt6Widgetsd.dll" 错误：

1. **确保已运行部署命令**：
   ```bash
   D:\soft\Qt\6.10.0\msvc2022_64\bin\windeployqt.exe Debug\application_omc.exe
   ```

2. **检查Debug目录**：确保包含以下文件：
   - `Qt6Cored.dll`
   - `Qt6Widgetsd.dll`
   - `Qt6Guid.dll`
   - 以及其他Qt插件目录

3. **在Debug目录中运行程序**：确保在包含Qt DLL的目录中运行可执行文件

### 文件占用错误
如果遇到 "Code is already running" 或文件无法删除：

1. **使用检查脚本**：
   ```bash
   check_files.bat
   ```

2. **使用强制清理**：
   ```bash
   clean_force.bat
   ```

3. **重启计算机**：如果问题持续存在

## 注意事项

- UI文件（.ui）放在src目录中，这样Qt的AutoUic工具可以正确找到并处理它们
- 项目使用Qt 6.10.0和MSVC 2022编译器
- 确保Qt安装路径正确配置在CMakeLists.txt中
- **重要**：构建后必须运行windeployqt.exe来部署Qt运行时库
- **清理建议**：使用clean_simple.bat进行简单清理，避免编码问题
- **文件占用**：如果清理失败，先检查文件占用情况

这种结构使项目更加清晰和易于维护，同时确保了与Qt构建系统的兼容性。 