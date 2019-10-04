cmake_minimum_required(VERSION 3.10)
project(viewer)

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
SET(BUILD_SHARED_LIBS OFF)
##SET(CMAKE_EXE_LINKER_FLAGS "-static")

set(CMAKE_CXX_STANDARD 11)

set(DCMTK_DIR "/home/jishin/app/dcmtk/install1/lib/cmake/dcmtk")
message("DCMTK_DIR=" ${DCMTK_DIR})

find_package(DCMTK REQUIRED)



add_executable(viewer main.cpp)

target_link_libraries(viewer PUBLIC ${DCMTK_LIBRARIES} )
##target_link_libraries(viewer PUBLIC ${DCMTK_LIBRARIES} "-static")

target_include_directories(viewer PUBLIC ${DCMTK_INCLUDE_DIR} )
