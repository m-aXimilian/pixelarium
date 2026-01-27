set(UTILSLIBNAME pixelariumutilslib)

set(UTILSLIBSRC
  ILog.hpp
  SpdLogger.hpp
  SpdLogger.cpp
  PixelariumLogger.hpp
  PixelariumLogger.cpp
  simple_thread_pool.hpp
  simple_thread_pool.cpp)

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
