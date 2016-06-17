cmake_minimum_required(VERSION 2.6)

set(EXENAME ork-test)

# Sources
include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/libraries" "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB SOURCE_FILES *.cpp)

add_definitions("-DORK_API=")

add_executable(${EXENAME} ${SOURCE_FILES})
target_link_libraries(${EXENAME} ork)

