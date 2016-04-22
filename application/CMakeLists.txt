cmake_minimum_required(VERSION 2.8)
project( SmartPool )
find_package( OpenCV REQUIRED )

add_executable( smartPool SmartPool.cpp ProjectionWindow.cpp MainWindow.cpp ObjectFinder.cpp)

#add_library ( projectionWindow ProjectionWindow.cpp)
#target_link_libraries( smartPool projectionWindow )

target_link_libraries( smartPool ${OpenCV_LIBS} )

