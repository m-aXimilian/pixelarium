configure_file(${CMAKE_CURRENT_SOURCE_DIR}/app_resources_default.h.in
  ${CMAKE_BINARY_DIR}/app_resources_default.h @ONLY)

set(APPLIBSRC
  AppGLFW.hpp
  AppGLFW.cpp
  DefaultApp.hpp
  DefaultApp.cpp
  PixelariumGallery.hpp
  PixelariumGallery.cpp
  ${imgui_DIR}/imgui.cpp
  ${imgui_DIR}/imgui_demo.cpp
  ${imgui_DIR}/imgui_draw.cpp
  ${imgui_DIR}/imgui_tables.cpp
  ${imgui_DIR}/imgui_widgets.cpp
  ${imgui_DIR}/backends/imgui_impl_opengl3.cpp
  ${imgui_DIR}/backends/imgui_impl_glfw.cpp)

add_library(${PIXELARIUM_APP_LIB_NAME}
  STATIC ${APPLIBSRC})

target_link_libraries(${PIXELARIUM_APP_LIB_NAME}
  PRIVATE pixelariumutilslib
  PRIVATE pixelariumimagelib
  PRIVATE pixelariumrenderlib)

# This needs to be public to let the consumer know about it.
if(WIN32)
  target_link_libraries(${PIXELARIUM_APP_LIB_NAME}
    PUBLIC opengl32.lib
    PUBLIC glfw
    PUBLIC OpenGL::GL)
endif()
if(LINUX)
  target_link_libraries(${PIXELARIUM_APP_LIB_NAME}
    PUBLIC glfw GL)
endif()
if(APPLE)
  target_link_libraries(${PIXELARIUM_APP_LIB_NAME}
    PUBLIC glfw
    PUBLIC "-framework OpenGL")
endif()

target_include_directories(${PIXELARIUM_APP_LIB_NAME}
  PRIVATE ${CMAKE_BINARY_DIR}
  PRIVATE ${PROJECT_SOURCE_DIR}/lib
  PRIVATE ${PROJECT_SOURCE_DIR}/lib/imaging
  PUBLIC ${pfd_DIR}
  PUBLIC ${imgui_DIR}
  PUBLIC ${imgui_DIR}/backends)

# appending the public interface directories
# to the therefore prepared variable
list(APPEND PIXELARIUM_INCLUDE_DIR
  ${pfd_DIR}
  ${imgui_DIR}
  ${imgui_DIR}/backends)

set(PIXELARIUM_INCLUDE_DIR ${PIXELARIUM_INCLUDE_DIR} PARENT_SCOPE)
