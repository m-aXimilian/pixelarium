set(UTILSLIBNAME pixelariumutilslib)

set(UTILSLIBSRC
  SpdLogger.cpp)

add_library(${UTILSLIBNAME} STATIC ${UTILSLIBSRC})

# won't work
# target_compile_options(${UTILSLIBNAME}
#   PRIVATE
#   "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
#   "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

target_include_directories(${UTILSLIBNAME}
  PRIVATE ${spdlog_DIR}/include)

target_link_libraries(${UTILSLIBNAME}
  PRIVATE spdlog::spdlog_header_only)
