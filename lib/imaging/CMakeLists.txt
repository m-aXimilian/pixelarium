include(${CMAKE_SOURCE_DIR}/cmake/libCZI.cmake)

find_package(OpenCV REQUIRED)

message(STATUS "Found opencv: " ${OpenCV_INCLUDE_DIRS})
message(STATUS "OpenCV_LIBs from: " ${OpenCV_LIBS})

set(IMAGELIBSRC
  IPixelariumImage.hpp
  PixelariumImageFactory.hpp
  PixelariumImageFactory.cpp
  impl/PixelariumJpg.hpp
  impl/PixelariumJpg.cpp
  impl/PixelariumPng.hpp
  impl/PixelariumPng.cpp
  impl/PixelariumCzi.hpp
  impl/PixelariumCzi.cpp
  impl/PixelariumTiff.hpp
  impl/PixelariumTiff.cpp
)

set(IMAGELIBLIBNAME pixelariumimagelib)

add_library(${IMAGELIBLIBNAME}
  STATIC ${IMAGELIBSRC})

target_compile_definitions(${IMAGELIBLIBNAME} PUBLIC _LIBCZISTATICLIB)

target_link_libraries(${IMAGELIBLIBNAME}
  PUBLIC ${OpenCV_LIBS}
  PUBLIC pixelariumutilslib
  PRIVATE libCZIStatic)


target_include_directories(${IMAGELIBLIBNAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/lib
  PUBLIC ${OpenCV_INCLUDE_DIRS}
  PUBLIC ${LIBCZI_INCLUDE_DIR})
