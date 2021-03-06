# Compiler flags
set(CMAKE_CXX_STANDARD 14)
cmake_minimum_required(VERSION 3.13)

# Project name
project(Delaunay)

# Common flags.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

# Paths.
set(BUILD_PATH .)

add_definitions(-DBASEFOLDER="${CMAKE_SOURCE_DIR}")

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


########################################### Visual Source code #########################################################
file(GLOB DELAUNAY_VISUAL_HEADER_FILES
        ${BUILD_PATH}/statistics
        ${BUILD_PATH}/test/inc
        ${BUILD_PATH}/visual
        ${BUILD_PATH}/visual/Commands
        ${BUILD_PATH}/include/Visual
        ${BUILD_PATH}/include/Visual/Display
        ${BUILD_PATH}/include/Visual/Display/Displayable
        )

file(GLOB DELAUNAY_VISUAL_SRC_FILES
        ${BUILD_PATH}/statistics/*.cpp
        ${BUILD_PATH}/test/src/Api/*.cpp
        ${BUILD_PATH}/test/src/Equivalence/*.cpp
        ${BUILD_PATH}/test/src/Fast/*.cpp
        ${BUILD_PATH}/test/src/Profiling/*.cpp
        ${BUILD_PATH}/test/src/Integration/*.cpp
        ${BUILD_PATH}/test/src/*.cpp
        ${BUILD_PATH}/visual/*.cpp
        ${BUILD_PATH}/visual/Commands/*.cpp
        ${BUILD_PATH}/src/Visual/*.cpp
        ${BUILD_PATH}/src/Visual/Display/*.cpp
        ${BUILD_PATH}/src/Visual/Display/Displayable/*.cpp
        )

################################################ Source code ##########################################################
file(GLOB DELAUNAY_HEADER_FILES
        ${BUILD_PATH}/include
        ${BUILD_PATH}/include/Config
        ${BUILD_PATH}/include/Dcel
        ${BUILD_PATH}/include/Figures
        ${BUILD_PATH}/include/Figures/Generators
        ${BUILD_PATH}/include/Gabriel
        ${BUILD_PATH}/include/InOut
        ${BUILD_PATH}/include/InOut/Reader
        ${BUILD_PATH}/include/InOut/Writer
        ${BUILD_PATH}/include/Store
        ${BUILD_PATH}/include/Triangulation
        ${BUILD_PATH}/include/Voronoi
        ${BUILD_PATH}/lib/inc
        )

file(GLOB DELAUNAY_SRC_FILES
        ${BUILD_PATH}/src/*.cpp
        ${BUILD_PATH}/src/Config/*.cpp
        ${BUILD_PATH}/src/Dcel/*.cpp
        ${BUILD_PATH}/src/Figures/*.cpp
        ${BUILD_PATH}/src/Figures/Generators/*.cpp
        ${BUILD_PATH}/src/Gabriel/*.cpp
        ${BUILD_PATH}/src/InOut/*.cpp
        ${BUILD_PATH}/src/InOut/Reader/*.cpp
        ${BUILD_PATH}/src/InOut/Writer/*.cpp
        ${BUILD_PATH}/src/Store/*.cpp
        ${BUILD_PATH}/src/Triangulation/*.cpp
        ${BUILD_PATH}/src/Voronoi/*.cpp
        ${BUILD_PATH}/lib/src/*.cpp
        )

################################################### GTest #############################################################
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


################################################### OpenGL #############################################################
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
message(${OPENGL_LIBRARIES})
message("00000000000000000000000000000000000000")
message(${GLUT_LIBRARY})

################################################ Executable ###########################################################
add_executable(Delaunay ${DELAUNAY_SRC_FILES} ${DELAUNAY_VISUAL_SRC_FILES})
target_link_libraries(Delaunay LINK_PUBLIC m pthread ${GTEST_LIBRARIES} -lglut -lGLU -lGL)
target_include_directories(Delaunay
        PUBLIC
        ${DELAUNAY_HEADER_FILES}
        ${DELAUNAY_VISUAL_HEADER_FILES}
)


################################################ Library ###########################################################
file(GLOB DELAUNAY_LIB_HEADER_FILES
        ${BUILD_PATH}/lib/inc
        )

file(GLOB DELAUNAY_LIB_SRC_FILES
        ${BUILD_PATH}/lib/src/*.cpp
        )

add_library(DelaunayLib ${DELAUNAY_SRC_FILES} ${DELAUNAY_LIB_SRC_FILES})

target_include_directories(DelaunayLib
        PUBLIC
        ${DELAUNAY_HEADER_FILES}
        ${DELAUNAY_LIB_HEADER_FILES}

        )
