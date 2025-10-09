include(FetchContent)

FetchContent_Declare(
  libCZI
  GIT_REPOSITORY https://github.com/ZEISS/libczi.git
  GIT_TAG        main
)

if(NOT libCZI_POPULATED)
  message(STATUS "Fetching libCZI")

  set(LIBCZI_BUILD_CZICMD                           OFF CACHE BOOL "Don't build commandline libCZI." FORCE)
  set(LIBCZI_BUILD_DYNLIB                           OFF CACHE BOOL "Don't build libCZI as a shared library." FORCE)
  set(LIBCZI_BUILD_UNITTESTS                        OFF CACHE BOOL "Don't build libCZI unit tests." FORCE)
  set(LIBCZI_BUILD_PREFER_EXTERNALPACKAGE_RAPIDJSON OFF CACHE BOOL "Use internal rapidjson." FORCE)
  set(LIBCZI_DO_NOT_SET_MSVC_RUNTIME_LIBRARY        ON  CACHE BOOL "Consumer sets the runtime." FORCE)
  set(ADDITIONAL_LIBS_REQUIRED_FOR_ATOMIC           ""  CACHE STRING "Additional atomic libs included." FORCE)

  FetchContent_MakeAvailable(libCZI)
endif()

FetchContent_GetProperties(libCZI)
set(LIBCZI_INCLUDE_DIR "${libczi_SOURCE_DIR}/Src/libCZI")
message(STATUS ${LIBCZI_INCLUDE_DIR})
