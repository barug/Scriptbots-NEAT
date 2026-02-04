cmake_minimum_required(VERSION 3.10)
project(scriptbots)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set (CMAKE_CXX_STANDARD 14)
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

# VTK is optional - use modern VTK 9.x approach
find_package(VTK QUIET COMPONENTS
    CommonCore
    CommonDataModel
    FiltersSources
    InteractionStyle
    RenderingCore
    RenderingOpenGL2
    RenderingFreeType
    RenderingContextOpenGL2
    ChartsCore
    ViewsContext2D
    ViewsInfovis
    InfovisLayout
    IOLegacy
)
if (VTK_FOUND)
    SET (HAVE_VTK 1)
    message(STATUS "VTK found (version ${VTK_VERSION}) - building with VTK support")
else()
    message(STATUS "VTK not found - building without VTK support")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories("include" ${CMAKE_CURRENT_BINARY_DIR} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

set( SB_SRCS
        src/main.cpp
        src/SimulationContext.cpp
        src/View.cpp
        src/GLView.cpp
        src/DWRAONBrain.cpp
        src/MLPBrain.cpp
        src/AssemblyBrain.cpp
        src/Agent.cpp
        src/World.cpp
        src/FoodSystem.cpp
        src/vmath.cpp
        src/NEAT/gene.cpp
        src/NEAT/genome.cpp
        src/NEAT/innovation.cpp
        src/NEAT/link.cpp
        src/NEAT/neat.cpp
        src/NEAT/NeatBrain.cpp
        src/NEAT/network.cpp
        src/NEAT/nnode.cpp
        src/NEAT/trait.cpp
        src/NEAT/Species.cpp
        src/settings.cpp)

# Add VTK sources only if VTK is found
if (VTK_FOUND)
    list(APPEND SB_SRCS
        src/VTKDashboard.cpp)
endif()

add_executable(scriptbots  ${SB_SRCS})

if (VTK_FOUND)
    target_link_libraries(scriptbots ${VTK_LIBRARIES})
    vtk_module_autoinit(
        TARGETS scriptbots
        MODULES ${VTK_LIBRARIES}
    )
endif()

if (WIN32 AND NOT GLUT_FOUND)
    target_link_libraries(scriptbots ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLUT32_LIBRARY})
elseif(APPLE)
    target_link_libraries(scriptbots ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
else()
    target_link_libraries(scriptbots ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
endif()

# Tests (optional, enable with -DBUILD_TESTS=ON)
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
