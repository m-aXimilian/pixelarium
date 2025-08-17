message(STATUS "Configuring Resources")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/uiresources_local.h.in
  ${CMAKE_BINARY_DIR}/uiresources_local.h @ONLY)
