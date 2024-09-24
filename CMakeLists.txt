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

if ("${PLATFORM}" STREQUAL "Web")
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} --cflags
        OUTPUT_VARIABLE EM_CFLAGS
        COMMAND_ERROR_IS_FATAL ANY
    )
    string(STRIP "${EM_CFLAGS}" EM_CFLAGS)
    find_package(Python3 REQUIRED)
    message("Python: ${Python3_EXECUTABLE}")
    set(ENV{EXTRA_FLAGS} "${EM_CFLAGS}")
    execute_process(
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/fix_compile_commands.py
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()
