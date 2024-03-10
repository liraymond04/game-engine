cmake_minimum_required(VERSION 3.12)
project(holojam)

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

# Get the path to /project-root/lua
set(LUA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../lua")

# Send runtime-loaded files to virtual FS
set(EXAMPLE_DIR "${CMAKE_SOURCE_DIR}/examples/${PROJECT_NAME}")
set(PRELOAD_FOLDERS
    "${EXAMPLE_DIR}/mods@/mods"
    "${EXAMPLE_DIR}/scenes/bin@/scenes/bin"
    "${LUA_PATH}@/bin"
)

# Convert list to string arguments
foreach(folder ${PRELOAD_FOLDERS})
    list(APPEND PRELOAD "--preload-file ${folder}")
endforeach()
string(REPLACE ";" " " PRELOAD "${PRELOAD}")

# Web Configurations
if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    # Since WASM is used, ALLOW_MEMORY_GROWTH has no extra overheads
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s ALLOW_MEMORY_GROWTH=1 --no-heap-copy")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES
        COMPILE_FLAGS "-sMAIN_MODULE=2"
        LINK_FLAGS "-sMAIN_MODULE=2 ${PRELOAD}"
        SUFFIX ".html"
    )
    # Compile and link dynamic library for mod scene
    add_library(web_scene SHARED "scenes/mod_scene/mod_scene.c")
    target_link_libraries(web_scene PRIVATE raylib engine)
    target_link_libraries(${PROJECT_NAME} PRIVATE web_scene)
else()
    # build scenes
    add_subdirectory(scenes)
endif()
