cmake_minimum_required(VERSION 3.5)

project(Swmm5 VERSION 5.2 LANGUAGES C)

# 设置编译器输出的目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 添加 OpenMP 支持
find_package(OpenMP)
if(OpenMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# 配置 MSVC 的选项
if(MSVC)
  set(CMAKE_C_FLAGS_RELEASE "/GL")
  add_definitions(-MT)
endif()

# MacOS 配置
if(APPLE)
  set(INSTALL_NAME_DIR @executable_path/../lib)
  set(CMAKE_MACOSX_RPATH 1)
  set(CMAKE_SHARED_LIBRARY_SUFFIX ".dylib")
endif()

# Linux 配置
if(UNIX AND NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
  set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif()

# Windows 配置
if(WIN32)
  set(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")
endif()

# 获取 SWMM 源文件列表
file(GLOB SWMM_LIB_ALL RELATIVE ${PROJECT_SOURCE_DIR} src/*)

# 排除 main.c 文件
list(REMOVE_ITEM SWMM_LIB_ALL "src/main.c")

# 配置动态库生成
if(MSVC AND "${CMAKE_VS_PLATFORM_NAME}" MATCHES "(Win32)")
  message(STATUS "Configuring with swmm5.def mapping")
  add_library(swmm5 SHARED ${SWMM_LIB_ALL} ${PROJECT_SOURCE_DIR}/src/swmm5.def)
  set_source_files_properties(${PROJECT_SOURCE_DIR}/src/swmm5.def PROPERTIES HEADER_FILE_ONLY TRUE)
else()
  list(REMOVE_ITEM SWMM_LIB_ALL "src/swmm5.def")
  add_library(swmm5 SHARED ${SWMM_LIB_ALL})
  find_library(MATH_LIBRARY m)
  if(MATH_LIBRARY)
    target_link_libraries(swmm5 PUBLIC ${MATH_LIBRARY})
  endif()
endif()

# 为动态库设置目标输出名称
set_target_properties(swmm5 PROPERTIES
        OUTPUT_NAME "swmm5"
        PREFIX ""                # Windows 不需要 lib 前缀
        SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX}  # 根据平台设置动态库后缀
)

# 配置命令行工具
add_executable(runswmm ${PROJECT_SOURCE_DIR}/src/main.c)
target_link_libraries(runswmm swmm5)

# 安装目标（可选）
install(TARGETS swmm5
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(TARGETS runswmm
        RUNTIME DESTINATION bin)
