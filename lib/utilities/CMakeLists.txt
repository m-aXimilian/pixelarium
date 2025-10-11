set(UTILSLIBSRC
  ILog.hpp
  SpdLogger.hpp
  SpdLogger.cpp)

add_library(${PIXELARIUM_UTILS_LIB_NAME} STATIC ${UTILSLIBSRC})

# won't work
# target_compile_options(${UTILSLIBNAME}
#   PRIVATE
#   "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
#   "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

target_include_directories(${PIXELARIUM_UTILS_LIB_NAME}
  PRIVATE ${spdlog_DIR}/include)

target_link_libraries(${PIXELARIUM_UTILS_LIB_NAME}
  PRIVATE spdlog::spdlog_header_only)
