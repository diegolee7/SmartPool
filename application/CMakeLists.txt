cmake_minimum_required(VERSION 2.8)
project( SmartPool )
find_package( OpenCV REQUIRED )

file(GLOB .
    "*.h"
    "*.cpp"
)

add_executable( smartPool SmartPool.cpp )

add_library ( projectionWindow ProjectionWindow.cpp)
target_link_libraries( smartPool LINK_PUBLIC projectionWindow )

add_library ( mainWindow MainWindow.cpp)
target_link_libraries( smartPool LINK_PUBLIC mainWindow )

add_library ( objectFinder ObjectFinder.cpp)
target_link_libraries( smartPool LINK_PUBLIC objectFinder )

target_link_libraries( smartPool ${OpenCV_LIBS} )

