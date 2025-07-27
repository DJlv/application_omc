cmake_minimum_required(VERSION 3.19)
project(application_omc LANGUAGES CXX)

# 设置Qt查找路径
set(CMAKE_PREFIX_PATH 
    "D:/soft/Qt/6.10.0/msvc2022_64"
    ${CMAKE_PREFIX_PATH}
)

# 查找Qt6
find_package(Qt6 6.0 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# 设置包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 创建可执行文件
qt_add_executable(application_omc
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/gridwidget.cpp
    src/mainwindow.ui
    include/mainwindow.h
    include/gridwidget.h
)

target_link_libraries(application_omc
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS application_omc
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET application_omc
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})