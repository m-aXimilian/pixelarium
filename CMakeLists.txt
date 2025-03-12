cmake_minimum_required(VERSION 3.23)

project(pixelarium VERSION 0.0.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED true)

set(imgui_DIR ${PROJECT_SOURCE_DIR}/modules/imgui)
set(glfw_DIR ${PROJECT_SOURCE_DIR}/modules/glfw)
set(pfd_DIR ${PROJECT_SOURCE_DIR}/modules/portable-file-dialogs)
set(spdlog_DIR ${PROJECT_SOURCE_DIR}/modules/spdlog)

include(cmake/libCZI.cmake)

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)

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

set(SRC src/main.cpp)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME}
  LINK_PUBLIC ${OpenCV_LIBS})

target_include_directories(${PROJECT_NAME}
  PUBLIC ${OpenCV_INCLUDE_DIRS}
  PUBLIC ${imgui_DIR}
  PUBLIC ${imgui_DIR}/backends
  PUBLIC ${glfw_INCLUDE_DIR}
  PUBLIC ${pfd_DIR}
  PUBLIC ${LIBCZI_INCLUDE_DIR})

if(WIN32)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE opengl32.lib
    PRIVATE glfw
    PRIVATE OpenGL::GL
    PRIVATE spdlog::spdlog_header_only)
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
