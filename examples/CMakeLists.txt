cmake_minimum_required(VERSION 2.6)

set(EXENAME ork-examples)

# Sources
include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/libraries" "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB SOURCE_FILES *.cpp)

add_definitions("-DORK_API=")

add_executable(${EXENAME} ${SOURCE_FILES})

#Added whole archive due to the "plug-in usage"
set(WHOLE_ORK_LIB -Wl,--whole-archive ork -Wl,--no-whole-archive)
target_link_libraries(${EXENAME} ${WHOLE_ORK_LIB})
