include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# From this documentation: https://google.github.io/googletest/quickstart-cmake.html
# For Windows: Prevent overriding the parent project's compiler/linker settings

if(WIN32)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)