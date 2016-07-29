include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(stbi)
add_subdirectory(tinyxml)

INSTALL(FILES pmath.h DESTINATION include/ork/math)
INSTALL(FILES tinyxml/tinyxml.h DESTINATION include/ork/resource/tinyxml)
INSTALL(FILES stbi/stb_image.h DESTINATION include/ork/resource/stbi)
