set(UTILSLIBNAME pixelariumutilslib)

set(UTILSLIBSRC
  SpdLogger.cpp)

add_library(${UTILSLIBNAME} STATIC ${UTILSLIBSRC})

target_include_directories(${UTILSLIBNAME}
  PRIVATE ${spdlog_DIR}/include)

target_link_libraries(${UTILSLIBNAME}
  PRIVATE spdlog::spdlog_header_only)