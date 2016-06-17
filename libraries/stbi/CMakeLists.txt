set(LIBNAME "stb_image")

# Sources
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB SOURCE_FILES *.cpp)

# Create a static library
add_library(${LIBNAME} STATIC ${SOURCE_FILES})

link_directories(${CMAKE_CURRENT_BINARY_DIR})
