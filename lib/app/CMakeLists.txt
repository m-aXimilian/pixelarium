# Fetch implot
include(implot)
message(STATUS "IMPLOT sources at ${IMPLOT_DIR}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/app_resources_default.h.in
  ${CMAKE_BINARY_DIR}/app_resources_default.h @ONLY)

set(IMPLOTSRC
  "${IMPLOT_DIR}/implot.h"
  "${IMPLOT_DIR}/implot_internal.h"
  "${IMPLOT_DIR}/implot_items.cpp"
  "${IMPLOT_DIR}/implot.cpp"
  "${IMPLOT_DIR}/implot_demo.cpp"
)

set(IMGUISRC
  "${imgui_DIR}/imgui.cpp"
  "${imgui_DIR}/imgui_demo.cpp"
  "${imgui_DIR}/imgui_draw.cpp"
  "${imgui_DIR}/imgui_tables.cpp"
  "${imgui_DIR}/imgui_widgets.cpp"
  "${imgui_DIR}/backends/imgui_impl_opengl3.cpp"
  "${imgui_DIR}/backends/imgui_impl_glfw.cpp")

set(APPLIBSRC
  include/imgui_proxy.hpp
  include/AppGLFW.hpp
  include/DefaultApp.hpp
  include/PixelariumGallery.hpp
  AppGLFW.cpp
  DefaultApp.cpp
  PixelariumGallery.cpp)

set(RENDERSRC
  rendering/include/RenderHelpers.hpp
  rendering/include/RenderImageManager.hpp
  rendering/include/CvMatRender.hpp
  rendering/include/IPixelariumImageView.hpp
  rendering/include/PixelariumImageViewDefault.hpp
  rendering/include/PixelariumImageViewCzi.hpp
  rendering/include/ImageViewFactory.hpp
  rendering/RenderHelpers.cpp
  rendering/CvMatRender.cpp
  rendering/IPixelariumImageView.cpp
  rendering/RenderImageManager.cpp
  rendering/PixelariumImageViewDefault.cpp
  rendering/PixelariumImageViewCzi.cpp
  rendering/ImageViewFactory.cpp)

set(APPLIBNAME pixelariumapplicationlib)

add_library(${APPLIBNAME}
  STATIC ${APPLIBSRC} ${IMGUISRC} ${IMPLOTSRC} ${RENDERSRC})

add_library(pixelarium::lib::application_static ALIAS ${APPLIBNAME})

target_link_libraries(${APPLIBNAME}
  PUBLIC
  pixelarium::lib::utilities_static
  pixelarium::lib::imaging_static
  pixelarium::lib::resources_static)

# This needs to be public to let the consumer know about it.
if(WIN32)
  target_link_libraries(${APPLIBNAME}
    PUBLIC opengl32.lib
    PUBLIC glfw
    PUBLIC OpenGL::GL)
endif()
if(LINUX)
  target_link_libraries(${APPLIBNAME}
    PUBLIC glfw GL)
endif()
if(APPLE)
  target_link_libraries(${APPLIBNAME}
    PUBLIC glfw
    PUBLIC "-framework OpenGL")
endif()

target_include_directories(${APPLIBNAME}
  INTERFACE
  PRIVATE ${CMAKE_BINARY_DIR}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rendering/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${pfd_DIR}
  PUBLIC ${imgui_DIR}
  PUBLIC ${imgui_DIR}/misc/cpp
  PUBLIC ${imgui_DIR}/backends
  ${IMPLOT_DIR})
