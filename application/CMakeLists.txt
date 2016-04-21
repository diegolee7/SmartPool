cmake_minimum_required(VERSION 2.8)
project( SmartPool )
find_package( OpenCV REQUIRED )

file(GLOB .
    "*.h"
    "*.cpp"
)

add_library ( projectionWindow ProjectionWindow.cpp)
add_executable( smartPool SmartPool.cpp )

target_link_libraries( smartPool LINK_PUBLIC projectionWindow )
target_link_libraries( smartPool ${OpenCV_LIBS} )

