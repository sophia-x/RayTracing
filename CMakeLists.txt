cmake_minimum_required (VERSION 3.1)
project (RayTracing)

set (CMAKE_CXX_STANDARD 11)

add_compile_options(-Werror -Wextra -O3)

FIND_LIBRARY(OPENCV opencv_core)
FIND_LIBRARY(OPENCV_GUI opencv_highgui)
FIND_LIBRARY(OPENCV_IM opencv_imgproc)
FIND_LIBRARY(ASSIMP assimp)

SET(EXTRA_LIBS ${OPENCV} ${OPENCV_GUI} ${OPENCV_IM} ${ASSIMP})

FILE(GLOB CAMERA src/cameras/*.cpp)
FILE(GLOB MODEL src/models/*.cpp)
FILE(GLOB SCENE src/scenes/*.cpp)
FILE(GLOB UTIL src/utils/*.cpp)
FILE(GLOB DEMO src/demo/*.cpp)

set(SRCS src/main.cpp ${CAMERA} ${MODEL} ${SCENE} ${UTIL} ${DEMO})
add_executable(RayTracing ${SRCS})

TARGET_LINK_LIBRARIES(RayTracing ${EXTRA_LIBS})

FILE(GLOB OBJS objs/*)
FILE(COPY ${OBJS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/objs)

FILE(GLOB TEXTURE textures/*)
FILE(COPY ${TEXTURE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/textures)