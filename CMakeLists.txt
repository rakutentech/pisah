cmake_minimum_required (VERSION 3.9)
project(berpecah)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# installing dependencies
include(FindPCRECPP REQUIRED)

# chceck if the compilation is done as an independent tool or as a submodule
get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
  set(STANDALONE_COMPILE off)
else(hasParent)
  set(STANDALONE_COMPILE on)
endif(hasParent)

include_directories(src)

add_subdirectory(src)

