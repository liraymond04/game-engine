cmake_minimum_required(VERSION 3.12)
project(Scenes)

# Set the output directory for shared libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Find all subdirectories in the current source directory
file(GLOB SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} */CMakeLists.txt)

# Include each subdirectory with a CMakeLists.txt file
foreach(subdir ${SUBDIRS})
    get_filename_component(project_name ${subdir} DIRECTORY)
    add_subdirectory(${project_name})
endforeach()
