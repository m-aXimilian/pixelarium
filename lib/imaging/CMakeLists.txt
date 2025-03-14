include(${CMAKE_SOURCE_DIR}/cmake/libCZI.cmake)

find_package(OpenCV REQUIRED)

message(STATUS "Found opencv: " ${OpenCV_INCLUDE_DIRS})
message(STATUS "OpenCV_LIBs from: " ${OpenCV_LIBS})

set(IMAGELIBSRC
  Image.cpp)

set(IMAGELIBLIBNAME pixelariumimagelib)

add_library(${IMAGELIBLIBNAME}
  STATIC ${IMAGELIBSRC})

target_link_libraries(${IMAGELIBLIBNAME}
  PUBLIC ${OpenCV_LIBS}
  PRIVATE libCZIStatic)


target_include_directories(${IMAGELIBLIBNAME}
  PUBLIC ${OpenCV_INCLUDE_DIRS}
  PRIVATE ${LIBCZI_INCLUDE_DIR})