# Make test executable

include_directories(.)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB LANGTESTS "lang/*.cpp")

add_executable(test main.cpp 
                    "${LANGTESTS}")
