cmake_minimum_required(VERSION 3.16)
project(libmd C)
set(CMAKE_INSTALL_PREFIX /usr)

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/debug)
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/debug)

#set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_DEBUG_POSTFIX "_d")

include_directories("./include")

add_subdirectory(src)
add_subdirectory(tests)
