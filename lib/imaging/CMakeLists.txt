include(libCZI)

find_package(OpenCV REQUIRED)

message(STATUS "Found opencv: " ${OpenCV_INCLUDE_DIRS})
message(STATUS "OpenCV_LIBs from: " ${OpenCV_LIBS})

set(IMAGELIBSRC
  include/IPixelariumImage.hpp
  include/PixelariumImageFactory.hpp
  impl/include/PixelariumJpg.hpp
  impl/include/PixelariumPng.hpp
  impl/include/PixelariumCzi.hpp
  impl/include/PixelariumTiff.hpp
  impl/include/PixelariumMem.hpp
  IPixelariumImage.cpp
  PixelariumImageFactory.cpp
  impl/PixelariumJpg.cpp
  impl/PixelariumPng.cpp
  impl/PixelariumCzi.cpp
  impl/PixelariumTiff.cpp
  impl/PixelariumMem.cpp
)

set(IMAGELIBLIBNAME pixelariumimagelib)

add_library(${IMAGELIBLIBNAME}
  STATIC ${IMAGELIBSRC})

add_library(pixelarium::lib::imaging_static ALIAS ${IMAGELIBLIBNAME})

target_compile_definitions(${IMAGELIBLIBNAME} PUBLIC _LIBCZISTATICLIB)

target_link_libraries(${IMAGELIBLIBNAME}
  PUBLIC ${OpenCV_LIBS}
  PUBLIC pixelarium::lib::utilities_static
  PRIVATE pixelarium::lib::resources_static
  PRIVATE libCZIStatic)


target_include_directories(${IMAGELIBLIBNAME}
  INTERFACE
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/impl/include
  PUBLIC ${OpenCV_INCLUDE_DIRS}
  PUBLIC ${LIBCZI_INCLUDE_DIR})
