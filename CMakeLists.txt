cmake_minimum_required (VERSION 2.6)
project (RayTracing)

FIND_LIBRARY(OPENCV opencv_core)
FIND_LIBRARY(OPENCV_GUI opencv_highgui)
FIND_LIBRARY(OPENCV_IM opencv_imgproc)
FIND_LIBRARY(ASSIMP assimp)

SET(EXTRA_LIBS ${OPENCV} ${OPENCV_GUI} ${OPENCV_IM} ${ASSIMP})

FILE(GLOB CAMERA src/cameras/*.cpp)
FILE(GLOB MODEL src/models/*.cpp)
FILE(GLOB SCENE src/scenes/*.cpp)

set(SRCS src/main.cpp ${CAMERA} ${MODEL} ${SCENE})
add_executable(RayTracing ${SRCS})

TARGET_LINK_LIBRARIES(RayTracing ${EXTRA_LIBS})

FILE(GLOB GLSL objs/*)
FILE(COPY ${GLSL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/objs)