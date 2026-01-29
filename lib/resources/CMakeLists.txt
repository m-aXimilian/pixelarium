set(RESOURCELIBNAME pixelariumresourcelib)

set(RESOURCELIBSRC
  include/resource.hpp
  resource.cpp)

add_library(${RESOURCELIBNAME} STATIC
  ${RESOURCELIBSRC})

add_library(pixelarium::lib::resources_static ALIAS ${RESOURCELIBNAME})

target_link_libraries(${RESOURCELIBNAME}
  PRIVATE pixelarium::lib::imaging_static)

target_include_directories(${RESOURCELIBNAME}
  INTERFACE
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
