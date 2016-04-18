cmake_minimum_required(VERSION 2.8)
project( SmartPool )
find_package( OpenCV REQUIRED )

add_executable( smartPool smartPool.cpp )
target_link_libraries( smartPool ${OpenCV_LIBS} )
