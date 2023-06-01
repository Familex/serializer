﻿cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy (SET CMP0141 NEW)
  set (CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

project (main)

include (src/download_project/DownloadProject.cmake)

# copy source of yaml_cpp (yaml-cpp library)
download_project (
    PROJ yaml_cpp 
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG master
    UPDATE_DISCONNECTED 1
)
add_subdirectory (${yaml_cpp_SOURCE_DIR} ${yaml_cpp_BINARY_DIR})
include_directories ("${yaml_cpp_SOURCE_DIR}/include")

add_subdirectory ("src/serializer")

add_executable (main "src/main.cpp")

# compile warnings
target_compile_options(
   main
   INTERFACE
   "$<$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
   "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:$<BUILD_INTERFACE:-W3>>"
)

# link
target_link_libraries (
    main
    serializer
    yaml-cpp # link with internal yaml-cpp.git library
)
