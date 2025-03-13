set(PIXELARIUM_TITLE ${CMAKE_PROJECT_NAME})

message(STATUS "Configuring Resources")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/uiresources.h.in
  ${CMAKE_BINARY_DIR}/uiresources.h @ONLY)
