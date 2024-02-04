cmake_minimum_required(VERSION 3.12)
project(test-game)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SRC_FILES ${SRC_DIR}/*.c)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE engine)

if ("${PLATFORM}" STREQUAL "Web")
    set(EMSCRIPTEN TRUE)
else()
    set(EMSCRIPTEN FALSE)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(EXAMPLE_DIR "${CMAKE_SOURCE_DIR}/examples/${PROJECT_NAME}")
set(PRELOAD_FOLDERS
    "${EXAMPLE_DIR}/mods@/mods"
    "${EXAMPLE_DIR}/scenes/bin@/scenes/bin"
)

foreach(folder ${PRELOAD_FOLDERS})
    list(APPEND PRELOAD "--preload-file ${folder}")
endforeach()

string(REPLACE ";" " " PRELOAD "${PRELOAD}")

# Web Configurations
if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    # Since WASM is used, ALLOW_MEMORY_GROWTH has no extra overheads
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s ALLOW_MEMORY_GROWTH=1 --no-heap-copy")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES
        COMPILE_FLAGS "-sMAIN_MODULE=1"
        LINK_FLAGS "-sMAIN_MODULE=1 ${PRELOAD}"
        SUFFIX ".html"
    )
endif()

# build scenes
add_subdirectory(scenes)
