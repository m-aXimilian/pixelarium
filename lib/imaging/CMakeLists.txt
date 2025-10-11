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
)

add_library(${PIXELARIUM_IMAGE_LIB_NAME}
  STATIC ${IMAGELIBSRC})

target_compile_definitions(${PIXELARIUM_IMAGE_LIB_NAME} PUBLIC _LIBCZISTATICLIB)

target_link_libraries(${PIXELARIUM_IMAGE_LIB_NAME}
  PUBLIC ${OpenCV_LIBS}
  PUBLIC pixelariumutilslib
  PRIVATE libCZIStatic)


target_include_directories(${PIXELARIUM_IMAGE_LIB_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/lib
  PUBLIC ${OpenCV_INCLUDE_DIRS}
  PUBLIC ${LIBCZI_INCLUDE_DIR})

# appending the public interface directories
# to the therefore prepared variable
list(APPEND PIXELARIUM_INCLUDE_DIR
  ${OpenCV_INCLUDE_DIRS}
  ${LIBCZI_INCLUDE_DIR})

set(PIXELARIUM_INCLUDE_DIR ${PIXELARIUM_INCLUDE_DIR} PARENT_SCOPE)
