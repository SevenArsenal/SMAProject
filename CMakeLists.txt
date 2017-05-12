cmake_minimum_required(VERSION 2.8)
project( SMA_headset_V1 )

# Locate libraries and headers
find_package( OpenCV REQUIRED )
find_package(WiringPi REQUIRED)
find_package(Threads REQUIRED) 

# Include headers
include_directories(${WIRINGPI_INCLUDE_DIRS}) 


include_directories("${PROJECT_SOURCE_DIR}/lib")
add_subdirectory (lib)

add_definitions(-std=c++0x -L/home/pi/wiringPi -lwiringPi -lpthread)
add_executable( SMA_headset_V1 SMA_headset_V1.cpp )


target_link_libraries( SMA_headset_V1 ${OpenCV_LIBS} )
target_link_libraries( SMA_headset_V1 ${WIRINGPI_LIBRARIES} )
target_link_libraries( SMA_headset_V1 ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries( SMA_headset_V1 lib )

