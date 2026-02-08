# this will trigger the FetchContent
include(testgoogle)

add_executable(libpixelarium_UnitTests
  lib/resources/test_resource.cpp
  lib/utilities/test_simple_thread_pool.cpp)

target_link_libraries(libpixelarium_UnitTests
  pixelarium::lib::resources_static
  pixelarium::lib::imaging_static
  GTest::gtest_main
  GTest::gmock)

include(GoogleTest)
gtest_discover_tests(libpixelarium_UnitTests)
