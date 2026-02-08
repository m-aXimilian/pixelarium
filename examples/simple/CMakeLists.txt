set(SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/simple.cpp)

set(SIMPLE_NAME "${PROJECT_NAME}_SIMPLE")

add_executable(${SIMPLE_NAME} ${SRC})

target_link_libraries(${SIMPLE_NAME}
  PRIVATE pixelariumapplicationlib)

