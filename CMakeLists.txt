cmake_minimum_required(VERSION 3.23)

project(pixelarium VERSION 0.0.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED true)

set(imgui_DIR ${PROJECT_SOURCE_DIR}/modules/imgui)
set(glfw_DIR ${PROJECT_SOURCE_DIR}/modules/glfw)
set(pfd_DIR ${PROJECT_SOURCE_DIR}/modules/portable-file-dialogs)
set(spdlog_DIR ${PROJECT_SOURCE_DIR}/modules/spdlog)

find_package(OpenGL REQUIRED)

message(STATUS "IMGUI:\t" ${imgui_DIR})
message(STATUS "GLFW:\t" ${glfw_DIR})
message(STATUS "PFD:\t\t" ${pfd_DIR})
message(STATUS "SPDLOG:\t" ${spdlog_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "/std:c++20 /Zi /EHsc")
endif()
if (WIN32 AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -g --std=c++20")
endif()
if(UNIX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -g --std=c++20")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

add_subdirectory(${pfd_DIR})
add_subdirectory(${spdlog_DIR})
add_subdirectory(${glfw_DIR})
add_subdirectory(src)
add_subdirectory(lib)

set(SRC
  src/AppGLFW.cpp
  src/main.cpp
  ${imgui_DIR}/imgui.cpp
  ${imgui_DIR}/imgui_demo.cpp
  ${imgui_DIR}/imgui_draw.cpp
  ${imgui_DIR}/imgui_tables.cpp
  ${imgui_DIR}/imgui_widgets.cpp
  ${imgui_DIR}/backends/imgui_impl_opengl3.cpp
  ${imgui_DIR}/backends/imgui_impl_glfw.cpp)

add_executable(${PROJECT_NAME} ${SRC})

message(STATUS "Linking " pixelariumimagelib)
target_link_libraries(${PROJECT_NAME}
  PRIVATE pixelariumimagelib
  PRIVATE pixelariumrenderlib
  PRIVATE pixelariumutilslib)

target_include_directories(${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/src
  PUBLIC ${PROJECT_SOURCE_DIR}/lib
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/imaging
  PUBLIC ${spdlog_DIR}/include
  PUBLIC ${imgui_DIR}
  PUBLIC ${imgui_DIR}/backends
  PUBLIC ${glfw_INCLUDE_DIR}
  PUBLIC ${pfd_DIR}
  PUBLIC ${LIBCZI_INCLUDE_DIR}
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR})


if(WIN32)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE opengl32.lib
    PRIVATE glfw
    PRIVATE OpenGL::GL)
endif()
if(LINUX) 
  target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw GL)
endif()
if(APPLE)
  target_link_libraries(${PROJECT_NAME}
    PUBLIC glfw
    PUBLIC "-framework OpenGL")
endif()
