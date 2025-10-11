add_subdirectory(utilities)
add_subdirectory(imaging)
add_subdirectory(rendering)
add_subdirectory(resources)
add_subdirectory(app)

# we need this to propagate include directories to the parent scope
# appending the public interface directories
# to the therefore prepared variable
list(APPEND PIXELARIUM_INCLUDE_DIR
  ${CMAKE_SOURCE_DIR}/lib/rendering
  ${CMAKE_SOURCE_DIR}/lib/imaging)


set(PIXELARIUM_INCLUDE_DIR ${PIXELARIUM_INCLUDE_DIR} PARENT_SCOPE)
