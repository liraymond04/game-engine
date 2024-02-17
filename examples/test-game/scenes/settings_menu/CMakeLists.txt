cmake_minimum_required(VERSION 3.12)
project(SettingsMenu)

# Set the source files directory
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
    GET_FILENAME_COMPONENT(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PARENT_DIR}/bin)
endif()

# Get a list of all .c files in the source directory
file(GLOB SRC_FILES ${SRC_DIR}/*.c)

# Create a shared library from each source file
foreach(SRC_FILE ${SRC_FILES})
    # Extract the file name without extension
    get_filename_component(SRC_NAME ${SRC_FILE} NAME_WE)
    
    # Create a shared library from the source file
    add_library(${SRC_NAME} SHARED ${SRC_FILE})
    target_link_libraries(${SRC_NAME} PRIVATE raylib engine)

    if (WIN32)
        set_target_properties(${SRC_NAME}
            PROPERTIES
                PREFIX "lib"
                LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        )
    endif()
    
    # Optionally, you can set additional properties for the shared library
    # For example, you can set the version, soname, etc.
    # set_target_properties(${SRC_NAME} PROPERTIES
    #     VERSION 1.0.0
    #     SOVERSION 1
    # )
endforeach()
