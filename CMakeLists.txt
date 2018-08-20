project(scriptbots)
cmake_minimum_required(VERSION 3.1)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE Debug)

if (WIN32 AND NOT GLUT_FOUND)
    find_library(GLUT_LIBRARY NAMES glut PATHS ${CMAKE_CURRENT_SOURCE_DIR})
    find_library(GLUT32_LIBRARY NAMES glut32 PATHS ${CMAKE_CURRENT_SOURCE_DIR})
    SET (LOCAL_GLUT32 1)
endif()

find_package(OpenMP)

if (OPENMP_FOUND)
    SET (HAVE_OPENMP 1)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()

set(VTK_DIR "~/data/VTK-7.1.1")
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories("include" ${CMAKE_CURRENT_BINARY_DIR} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

set( SB_SRCS
        src/main.cpp
        src/View.cpp
        src/GLView.cpp
        src/DWRAONBrain.cpp
        src/MLPBrain.cpp
        src/AssemblyBrain.cpp
        src/Agent.cpp
        src/World.cpp
        src/vmath.cpp
        src/VTKView.cpp
        src/NEAT/gene.cpp
        src/NEAT/genome.cpp
        src/NEAT/innovation.cpp
        src/NEAT/link.cpp
        src/NEAT/neat.cpp
        src/NEAT/NeatBrain.cpp
        src/NEAT/network.cpp
        src/NEAT/nnode.cpp
        src/NEAT/trait.cpp)

add_executable(scriptbots  ${SB_SRCS})


target_link_libraries(scriptbots ${VTK_LIBRARIES})

if (WIN32 AND NOT GLUT_FOUND)
    target_link_libraries(scriptbots ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLUT32_LIBRARY})
else()
    target_link_libraries(scriptbots ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
endif()
