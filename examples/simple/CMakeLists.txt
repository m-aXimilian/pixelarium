set(SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(SIMPLE_NAME "${PROJECT_NAME}_SIMPLE")

add_executable(${SIMPLE_NAME} ${SRC})

target_link_libraries(${SIMPLE_NAME}
  PRIVATE pixelariumimagelib
  PRIVATE pixelariumrenderlib
  PRIVATE pixelariumutilslib
  PRIVATE pixelariumresourcelib
  PRIVATE pixelariumapplicationlib)

target_include_directories(${SIMPLE_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/src
  PRIVATE ${PROJECT_SOURCE_DIR}/lib
  PRIVATE ${PROJECT_SOURCE_DIR}/lib/imaging
  PRIVATE ${PROJECT_SOURCE_DIR}/lib/app
  PRIVATE ${spdlog_DIR}/include
  PRIVATE ${LIBCZI_INCLUDE_DIR}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
