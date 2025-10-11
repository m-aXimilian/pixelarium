set(RESOURCELIBSRC
  resource.hpp
  resource.cpp)

add_library(${PIXELARIUM_RESOURCE_LIB_NAME} STATIC
  ${RESOURCELIBSRC})

target_link_libraries(${PIXELARIUM_RESOURCE_LIB_NAME}
  PRIVATE pixelariumimagelib)

target_include_directories(${PIXELARIUM_RESOURCE_LIB_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/lib)
