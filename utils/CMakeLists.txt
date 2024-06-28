cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(utils)

include_directories(../include)

# Dependencies
include(FetchContent)

# Function to fetch and build an external project
function(fetch_and_build_project TARGET_NAME URL)
    FetchContent_Declare(
        ${TARGET_NAME}
        URL ${URL}
    )
    FetchContent_GetProperties(${TARGET_NAME})
    if (NOT ${TARGET_NAME}_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(${TARGET_NAME})
        set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE) # Example customization, adjust as needed
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # Example customization, adjust as needed
        add_subdirectory(${${TARGET_NAME}_SOURCE_DIR} ${${TARGET_NAME}_BINARY_DIR})
    endif()
endfunction()

# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    fetch_and_build_project(raylib https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz)
endif()

set(JSON_C_VERSION "0.17-20230812")
find_package(json-c QUIET)
if (NOT json-c_FOUND OR WIN32)
    fetch_and_build_project(json-c https://github.com/json-c/json-c/archive/refs/tags/json-c-${JSON_C_VERSION}.tar.gz)
endif()

# Set the directory containing source files
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Get all source files in the utils directory
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.c")

# Get all source files in the include directory
file(GLOB_RECURSE INCLUDE_SOURCE_FILES "../include/*.c")

# Loop over each source file and create an executable for each
foreach(SOURCE_FILE ${SOURCE_FILES})
    # Extract the name of the source file (excluding the extension)
    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    # Create the executable
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})
    # Link libraries
    target_link_libraries(${EXECUTABLE_NAME} raylib -lm json-c)
    if (NOT WIN32)
        target_compile_options(${EXECUTABLE_NAME} PUBLIC -Wno-deprecated-declarations)
    endif()
endforeach()
