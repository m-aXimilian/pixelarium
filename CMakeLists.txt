cmake_minimum_required(VERSION 3.23)

project(pixelarium VERSION 0.0.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED true)

# setting global module directories
set(glfw3_module_DIR ${PROJECT_SOURCE_DIR}/modules/glfw)
set(glfw3_DIR "${glfw3_module_DIR}/CMake")
set(imgui_DIR ${PROJECT_SOURCE_DIR}/modules/imgui)
set(pfd_DIR ${PROJECT_SOURCE_DIR}/modules/portable-file-dialogs)
set(spdlog_DIR ${PROJECT_SOURCE_DIR}/modules/spdlog)

find_package(OpenGL REQUIRED)

message(STATUS "GLFW:\t" ${glfw3_module_DIR})
message(STATUS "PFD:\t\t" ${pfd_DIR})
message(STATUS "SPDLOG:\t" ${spdlog_DIR})

if(WIN32)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/std:c++20 /Zi /EHsc")
  else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -g --std=c++20")
  endif()
endif()
if(UNIX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -g --std=c++20")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

add_subdirectory(${pfd_DIR})
add_subdirectory(${spdlog_DIR})
add_subdirectory(${glfw3_module_DIR})
add_subdirectory(src)
add_subdirectory(lib)

set(SRC
  src/MyApp.cpp
  src/main.cpp)

#====================
# needed for the spdlogger implementation
# this is not nice, but it won't work when, e.g. doing it from lower level cmake files
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
#====================

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME}
  PRIVATE pixelariumimagelib
  PRIVATE pixelariumrenderlib
  PRIVATE pixelariumutilslib
  PRIVATE pixelariumresourcelib
  PUBLIC pixelariumapplicationlib)

target_include_directories(${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/src
  PUBLIC ${PROJECT_SOURCE_DIR}/lib
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/imaging
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/app
  PUBLIC ${spdlog_DIR}/include
  PUBLIC ${pfd_DIR}
  PUBLIC ${LIBCZI_INCLUDE_DIR}
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

option(PIXELARIUM_BUILD_UNITTESTS "Generate Unittests" ON)


if(PIXELARIUM_BUILD_UNITTESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()
