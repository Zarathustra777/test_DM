cmake_minimum_required(VERSION 3.26)
project(test_DM)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(test_DM main.cpp
        headers.h
        pixelsmap.h
        pixelsmap.cpp
        utils.cpp

)

target_link_libraries(test_DM  ${OpenCV_LIBS} )
