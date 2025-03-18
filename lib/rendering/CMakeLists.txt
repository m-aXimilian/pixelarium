set(RENDERLIBNAME pixelariumrenderlib)

set(RENDERLIBSRC
  CvMatRender.cpp)

add_library(${RENDERLIBNAME} STATIC
  ${RENDERLIBSRC})

target_link_libraries(${RENDERLIBNAME}
  PRIVATE pixelariumimagelib)

if(LINUX)
  target_link_libraries(${RENDERLIBNAME}
    PRIVATE GL)
endif()

target_include_directories(${RENDERLIBNAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/lib)