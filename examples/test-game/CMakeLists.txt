cmake_minimum_required(VERSION 3.12)
project(test-game)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SRC_FILES ${SRC_DIR}/*.c)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE engine)

# build scenes
add_subdirectory(scenes)
