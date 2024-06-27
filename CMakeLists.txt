cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(main)

# Set directories
set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(RELEASE_ROOT_DIR "${PROJECT_ROOT}/release")
if ("${PLATFORM}" STREQUAL "Web")
    set(RELEASE_ROOT_DIR "${PROJECT_ROOT}/release-web")
elseif(WIN32)
    set(RELEASE_ROOT_DIR "${PROJECT_ROOT}/release-windows")
endif()

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option to build examples
option(BUILD_EXAMPLE "Build examples" ON)

# Option to build utils
option(BUILD_UTILS "Build utils" ON)

message("BUILD_EXAMPLES " ${BUILD_EXAMPLES})
message("BUILD_UTILS " ${BUILD_UTILS})

include_directories(${CMAKE_SOURCE_DIR}/include)

# build engine
add_subdirectory(src)

# example projects
if(BUILD_EXAMPLE)
    if ("${PLATFORM}" STREQUAL "Web")
        add_subdirectory(examples/holojam)
    else()
        add_subdirectory(examples/test-game)
        add_subdirectory(examples/holojam)
    endif()
endif()

# utils
if(BUILD_UTILS)
    if ("${PLATFORM}" STREQUAL "Web")
    else()
        add_subdirectory(utils)
    endif()
endif()

# add engine headers to global include
target_include_directories(engine INTERFACE ${CMAKE_SOURCE_DIR}/src/engine)
