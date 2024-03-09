cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(main)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option to build examples
option(BUILD_EXAMPLES "Build examples" ON)

# Option to build utils
option(BUILD_UTILS "Build utils" ON)

message("BUILD_EXAMPLES " ${BUILD_EXAMPLES})
message("BUILD_UTILS " ${BUILD_UTILS})

include_directories(${CMAKE_SOURCE_DIR}/include)

# build engine
add_subdirectory(src)

# example projects
if(BUILD_EXAMPLES)
    add_subdirectory(examples/test-game)
    add_subdirectory(examples/holojam)
endif()

# utils
if(BUILD_UTILS)
    add_subdirectory(utils)
endif()

# add engine headers to global include
target_include_directories(engine INTERFACE ${CMAKE_SOURCE_DIR}/src/engine)
