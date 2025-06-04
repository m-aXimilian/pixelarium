set(RESOURCELIBNAME pixelariumresourcelib)

set(RESOURCELIBSRC
  resource.cpp)

add_library(${RESOURCELIBNAME} STATIC
  ${RESOURCELIBSRC})

target_link_libraries(${RESOURCELIBNAME}
  PRIVATE pixelariumimagelib)

target_include_directories(${RESOURCELIBNAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/lib)
