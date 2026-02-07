set(SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/custom_1.cpp)

set(CUSTOM_1_NAME "${PROJECT_NAME}_custom1_app")

add_executable(${CUSTOM_1_NAME} ${SRC})

target_link_libraries(${CUSTOM_1_NAME}
  PRIVATE pixelariumapplicationlib)
