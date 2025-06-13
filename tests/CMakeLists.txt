include(${PROJECT_SOURCE_DIR}/modules/googletest.cmake)

add_executable(libpixelarium_UnitTests
  lib/resources/test_resource.cpp)

target_include_directories(libpixelarium_UnitTests
  PUBLIC ${PROJECT_SOURCE_DIR}/lib
  PUBLIC ${OpenCV_INCLUDE_DIRS}
)


target_link_libraries(libpixelarium_UnitTests
  pixelariumresourcelib
  pixelariumimagelib
  GTest::gtest_main
  GTest::gmock)

include(GoogleTest)
gtest_discover_tests(libpixelarium_UnitTests)
