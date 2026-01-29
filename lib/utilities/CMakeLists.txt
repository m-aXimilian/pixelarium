set(UTILSLIBNAME pixelariumutilslib)

set(UTILSLIBSRC
  include/ILog.hpp
  include/SpdLogger.hpp
  include/PixelariumLogger.hpp
  include/simple_thread_pool.hpp
  SpdLogger.cpp
  PixelariumLogger.cpp
  simple_thread_pool.cpp)

add_library(${UTILSLIBNAME} STATIC ${UTILSLIBSRC})

add_library(pixelarium::lib::utilities_static ALIAS ${UTILSLIBNAME})

# won't work
# target_compile_options(${UTILSLIBNAME}
#   PRIVATE
#   "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
#   "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

target_include_directories(${UTILSLIBNAME}
  INTERFACE
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${spdlog_DIR}/include)

target_link_libraries(${UTILSLIBNAME}
  PUBLIC spdlog::spdlog_header_only)
