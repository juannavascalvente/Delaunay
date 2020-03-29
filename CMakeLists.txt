# Compiler flags
set(CMAKE_CXX_STANDARD 14)
cmake_minimum_required(VERSION 3.15)

# Project name
project(Delaunay)

# Common flags.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

# Paths.
set(BUILD_PATH .)

# Build options
if (BUILD_DEBUG)
    message("Building DEBUG mode")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
elseif (BUILD_COVERAGE)
    message("Building OVERAGE mode")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -coverage")
elseif (BUILD_PROFILING)
    message("Building PROFILING mode")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
else (BUILD_RELEASE)
    message("Building RELEASE mode")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
endif (BUILD_DEBUG)


################################################ Source code ##########################################################
file(GLOB DELAUNAY_HEADER_FILES
        ${BUILD_PATH}/include
        ${BUILD_PATH}/include/Dcel
        ${BUILD_PATH}/include/Gabriel
        ${BUILD_PATH}/include/Store
        ${BUILD_PATH}/include/Triangulation
        ${BUILD_PATH}/include/Visual
        ${BUILD_PATH}/include/Voronoi
        ${BUILD_PATH}/statistics
        ${BUILD_PATH}/test
        ${BUILD_PATH}/visual
        ${BUILD_PATH}/visual/Commands
        ${BUILD_PATH}/visual/Display
        ${BUILD_PATH}/visual/Display/Displayable
        )

file(GLOB DELAUNAY_SRC_FILES
        ${BUILD_PATH}/src/*.cpp
        ${BUILD_PATH}/src/Dcel/*.cpp
        ${BUILD_PATH}/src/Gabriel/*.cpp
        ${BUILD_PATH}/src/Store/*.cpp
        ${BUILD_PATH}/src/Triangulation/*.cpp
        ${BUILD_PATH}/src/Visual/*.cpp
        ${BUILD_PATH}/src/Voronoi/*.cpp
        ${BUILD_PATH}/statistics/*.cpp
        ${BUILD_PATH}/test/*.cpp
        ${BUILD_PATH}/visual/*.cpp
        ${BUILD_PATH}/visual/Commands/*.cpp
        ${BUILD_PATH}/visual/Display/*.cpp
        ${BUILD_PATH}/visual/Display/Displayable/*.cpp
        )

################################################### GTest #############################################################
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


################################################ Executable ###########################################################
add_executable(Delaunay ${DELAUNAY_SRC_FILES})
target_link_libraries(Delaunay LINK_PUBLIC m pthread ${GTEST_LIBRARIES} -lglut -lGL)
target_include_directories(Delaunay
        PUBLIC
        ${DELAUNAY_HEADER_FILES}

)
