cmake_minimum_required (VERSION 2.6)
project (RayTracing)

FIND_LIBRARY(OPENCV opencv)
FIND_LIBRARY(OPENCV_GUI opencv_highgui)
FIND_LIBRARY(OPENCV_IM opencv_imgproc)
SET(EXTRA_LIBS ${OPENCV} ${OPENCV_GUI} ${OPENCV_IM})

FILE(GLOB CAMERA src/cameras/*.cpp)
FILE(GLOB MODEL src/models/*.cpp)

set(SRCS src/main.cpp ${CAMERA} ${MODEL})
add_executable(RayTracing ${SRCS})

TARGET_LINK_LIBRARIES(RayTracing ${EXTRA_LIBS})