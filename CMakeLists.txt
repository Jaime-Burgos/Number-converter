cmake_minimum_required (VERSION 3.10.0)
project (TextConverter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt5Core)

add_executable(TextConverter
    src/main.cpp
)

target_link_libraries(TextConverter Qt5::Core)
