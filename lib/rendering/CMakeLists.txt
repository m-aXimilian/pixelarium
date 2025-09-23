set(RENDERLIBNAME pixelariumrenderlib)

set(RENDERLIBSRC
  CvMatRender.hpp
  CvMatRender.cpp
  RenderImageManager.hpp
  RenderImageManager.cpp
  IPixelariumImageView.hpp
  PixelariumImageViewDefault.hpp
  PixelariumImageViewDefault.cpp
  ImageViewFactory.hpp
  ImageViewFactory.cpp)

add_library(${RENDERLIBNAME} STATIC
  ${RENDERLIBSRC})

target_link_libraries(${RENDERLIBNAME}
  PRIVATE pixelariumimagelib)

target_include_directories(${RENDERLIBNAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/lib
  PRIVATE ${imgui_DIR})
