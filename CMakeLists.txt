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

# 动态收集所有源文件
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/core/*.cpp"
    "src/widgets/*.cpp"
    "src/controllers/*.cpp"
    "src/utils/*.cpp"
)

# 动态收集所有头文件
file(GLOB_RECURSE HEADERS 
    "include/*.h"
    "include/core/*.h"
    "include/widgets/*.h"
    "include/controllers/*.h"
    "include/utils/*.h"
)

# 动态收集UI文件
file(GLOB_RECURSE UI_FILES "src/core/*.ui")

# 动态收集资源文件（如果有的话）
file(GLOB_RECURSE RESOURCE_FILES "resources/*.qrc")

# 打印收集到的文件（调试用）
message(STATUS "收集到的源文件:")
foreach(SOURCE ${SOURCES})
    message(STATUS "  ${SOURCE}")
endforeach()

message(STATUS "收集到的头文件:")
foreach(HEADER ${HEADERS})
    message(STATUS "  ${HEADER}")
endforeach()

message(STATUS "收集到的UI文件:")
foreach(UI_FILE ${UI_FILES})
    message(STATUS "  ${UI_FILE}")
endforeach()

# 创建可执行文件
qt_add_executable(application_omc
    WIN32 MACOSX_BUNDLE
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

# 链接Qt库
target_link_libraries(application_omc
    PRIVATE
        Qt::Core
        Qt::Widgets
)

# 设置编译选项
target_compile_features(application_omc PRIVATE cxx_std_17)

# 设置输出目录
set_target_properties(application_omc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# 安装配置
include(GNUInstallDirs)

install(TARGETS application_omc
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# 生成部署脚本
qt_generate_deploy_app_script(
    TARGET application_omc
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# 自定义目标：清理构建文件
add_custom_target(clean-build
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMENT "清理构建文件"
)

# 自定义目标：重新构建
add_custom_target(rebuild
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "重新构建项目"
)

# 自定义目标：部署
add_custom_target(deploy
    COMMAND ${CMAKE_COMMAND} -E echo "部署Qt运行时库..."
    COMMAND "D:/soft/Qt/6.10.0/msvc2022_64/bin/windeployqt.exe" 
            "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/application_omc.exe"
    COMMENT "部署应用程序"
    DEPENDS application_omc
)

# 打印项目信息
message(STATUS "=== 项目配置信息 ===")
message(STATUS "项目名称: ${PROJECT_NAME}")
message(STATUS "Qt版本: ${Qt6_VERSION}")
message(STATUS "编译器: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "输出目录: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "源文件数量: ${SOURCES}")
message(STATUS "头文件数量: ${HEADERS}")
message(STATUS "UI文件数量: ${UI_FILES}")
message(STATUS "=====================")