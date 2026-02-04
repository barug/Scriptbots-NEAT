# Tests CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Test sources - core logic only (no graphics dependencies)
set(TESTABLE_SRCS
    ${CMAKE_SOURCE_DIR}/src/Agent.cpp
    ${CMAKE_SOURCE_DIR}/src/FoodSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/vmath.cpp
    ${CMAKE_SOURCE_DIR}/src/settings.cpp
    ${CMAKE_SOURCE_DIR}/src/DWRAONBrain.cpp
    ${CMAKE_SOURCE_DIR}/src/MLPBrain.cpp
    ${CMAKE_SOURCE_DIR}/src/AssemblyBrain.cpp
    ${CMAKE_SOURCE_DIR}/src/NEAT/gene.cpp
    ${CMAKE_SOURCE_DIR}/src/NEAT/genome.cpp
    ${CMAKE_SOURCE_DIR}/src/NEAT/innovation.cpp
    ${CMAKE_SOURCE_DIR}/src/NEAT/link.cpp
    ${CMAKE_SOURCE_DIR}/src/NEAT/neat.cpp
    ${CMAKE_SOURCE_DIR}/src/NEAT/NeatBrain.cpp
    ${CMAKE_SOURCE_DIR}/src/NEAT/network.cpp
    ${CMAKE_SOURCE_DIR}/src/NEAT/nnode.cpp
    ${CMAKE_SOURCE_DIR}/src/NEAT/trait.cpp
    ${CMAKE_SOURCE_DIR}/src/NEAT/Species.cpp
)
# Note: World.cpp excluded due to VTK/graphics dependencies

# Unit tests executable
add_executable(unit_tests
    test_agent.cpp
    test_agent_extended.cpp
    test_neat_brain.cpp
    test_settings.cpp
    test_species.cpp
    test_helpers.cpp
    test_vmath.cpp
    test_genome.cpp
    test_food_system.cpp
    ${TESTABLE_SRCS}
)

target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(unit_tests
    GTest::gtest_main
)

# Add compile definition to disable VTK in tests
target_compile_definitions(unit_tests PRIVATE TESTING_MODE)

include(GoogleTest)
gtest_discover_tests(unit_tests)
