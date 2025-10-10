cmake_minimum_required(VERSION 3.23)

project(pixelarium VERSION 0.0.7)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# setting global module directories
set(glfw3_module_DIR ${PROJECT_SOURCE_DIR}/modules/glfw)
set(glfw3_DIR "${glfw3_module_DIR}/CMake")
set(imgui_DIR ${PROJECT_SOURCE_DIR}/modules/imgui)
set(pfd_DIR ${PROJECT_SOURCE_DIR}/modules/portable-file-dialogs)
set(spdlog_DIR ${PROJECT_SOURCE_DIR}/modules/spdlog)

message(STATUS "GLFW:\t" ${glfw3_module_DIR})
message(STATUS "PFD:\t\t" ${pfd_DIR})
message(STATUS "SPDLOG:\t" ${spdlog_DIR})

#====================
# Options
option(PIXELARIUM_BUILD_UNITTESTS "Generate Unittests"                        ON)
option(PIXELARIUM_BUILD_DOCS      "Generate Documentation"                    ON)
option(PIXELARIUM_BUILD_DOCS_ONLY "Build only Documentation (no compilatoin)" OFF)
#====================

if(PIXELARIUM_BUILD_DOCS OR PIXELARIUM_BUILD_DOCS_ONLY)
  include(${PROJECT_SOURCE_DIR}/cmake/awesomeDoxygen.cmake)
  set(MAINPAGE_FILE "doc/index.md")
  find_package(Doxygen)
  if (DOXYGEN_FOUND)

    set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message(STATUS "Building Docs")

    add_custom_target(doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Generating Docs")

  else (DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen need to be installed to generate the doxygen documentation")
  endif (DOXYGEN_FOUND)

  if(PIXELARIUM_BUILD_DOCS_ONLY)
    message(STATUS "Documentation Generated successful")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    message(STATUS "PIXELARIUM_BUILD_DOCS_ONLY was set to ON. Thus, no compilation will follow.")
    message(STATUS "If compilation shall occur, set '-DPIXELARIUM_BUILD_DOCS_ONLY=OFF'.")
    list(POP_BACK CMAKE_MESSAGE_INDENT)

    return()
  endif()
endif()

find_package(OpenGL REQUIRED)

if(WIN32)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/Zi /EHsc")
  else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")
  endif()
endif()
if(UNIX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")
endif()

string(TOUPPER "${CMAKE_PROJECT_NAME}" PIXELARIUM_TITLE)

add_subdirectory(${pfd_DIR})
add_subdirectory(${spdlog_DIR})
add_subdirectory(${glfw3_module_DIR})
add_subdirectory(lib)

set(SRC
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
  PUBLIC ${LIBCZI_INCLUDE_DIR}
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR})


if(PIXELARIUM_BUILD_UNITTESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()
