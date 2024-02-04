cmake_minimum_required(VERSION 3.12)
project(MainMenu)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
# Set the source files directory
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Get a list of all .c files in the source directory
file(GLOB SRC_FILES ${SRC_DIR}/*.c)

if ("${PLATFORM}" STREQUAL "Web")
    set(EMSCRIPTEN TRUE)
else()
    set(EMSCRIPTEN FALSE)
endif()

# Create a shared library from each source file
foreach(SRC_FILE ${SRC_FILES})
    # Extract the file name without extension
    get_filename_component(SRC_NAME ${SRC_FILE} NAME_WE)
    
    # Create a shared library from the source file
    add_library(${SRC_NAME} SHARED ${SRC_FILE})
    target_link_libraries(${SRC_NAME} PRIVATE raylib engine)

    # Optionally, you can set additional properties for the shared library
    if (EMSCRIPTEN)
        set_target_properties(${SRC_NAME} PROPERTIES
            # COMPILE_FLAGS "-sSIDE_MODULE=1 -shared -sLINKABLE=1 -sEXPORT_ALL=1 -sSINGLE_FILE=1"
            LINKER_FLAGS "-sSIDE_MODULE=1 -shared -sLINKABLE=1 -sEXPORT_ALL=1 -sSINGLE_FILE=1"
            # SUFFIX ".so"
        )
    endif()
    # For example, you can set the version, soname, etc.
    # set_target_properties(${SRC_NAME} PROPERTIES
    #     VERSION 1.0.0
    #     SOVERSION 1
    # )
endforeach()
