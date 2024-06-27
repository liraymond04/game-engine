cmake_minimum_required(VERSION 3.12)
project(holojam)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(RELEASE_DIR "${RELEASE_ROOT_DIR}/${PROJECT_NAME}")
set(RELEASE_EXECUTABLE_DIR "${RELEASE_DIR}")
set(RELEASE_SCENE_DIR "${RELEASE_DIR}/scenes")
set(RELEASE_MOD_DIR "${RELEASE_DIR}/mods")
set(RELEASE_LUA_DIR "${RELEASE_DIR}/lua")
set(RELEASE_RES_DIR "${RELEASE_DIR}")

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
set(LUA_PATH "${PROJECT_ROOT}/lua")

# set build paths
if(BUILD_PATHS AND NOT EMSCRIPTEN)
    target_compile_definitions(engine PRIVATE BUILD_PATHS=ON)
    target_compile_definitions(engine PRIVATE RRES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")
    target_compile_definitions(engine PRIVATE SCENE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/scenes/bin/")
    target_compile_definitions(engine PRIVATE MOD_PATH="${CMAKE_CURRENT_SOURCE_DIR}/mods/")
    target_compile_definitions(engine PRIVATE LUA_PATH="./lua/")
endif()

if (EMSCRIPTEN)
    # set emscripten build paths
    target_compile_definitions(engine PRIVATE BUILD_PATHS=ON)
    target_compile_definitions(engine PRIVATE RRES_PATH="/")
    target_compile_definitions(engine PRIVATE SCENE_PATH="/scenes/")
    target_compile_definitions(engine PRIVATE MOD_PATH="/mods/")
    target_compile_definitions(engine PRIVATE LUA_PATH="/lua/")

    # Compile and link dynamic library for mod scene
    add_library(web_scene SHARED "scenes/mod_scene/mod_scene.c")
    set_target_properties(web_scene PROPERTIES SUFFIX ".wasm")
    target_link_libraries(web_scene PRIVATE raylib engine)
    target_link_libraries(${PROJECT_NAME} PRIVATE web_scene)
endif()

# Send runtime-loaded files to virtual FS
set(EXAMPLE_DIR "${CMAKE_SOURCE_DIR}/examples/${PROJECT_NAME}")
set(EXAMPLE_BUILD_DIR "${CMAKE_BINARY_DIR}/examples/${PROJECT_NAME}")
set(PRELOAD_FOLDERS
    "${EXAMPLE_DIR}/mods"
    # "${EXAMPLE_DIR}/scenes/bin"
    "${LUA_PATH}"
)
set(PRELOAD_DEST
    "/mods"
    # "/scenes/bin"
    "/lua"
)

set(PRELOAD_FILES
    "${EXAMPLE_BUILD_DIR}/libweb_scene.wasm"
    "${EXAMPLE_DIR}/out.rres"
    "${EXAMPLE_DIR}/out.rres.json"
)
set(PRELOAD_FILES_DEST
    "/scenes/libmod_scene.wasm"
    "/out.rres"
    "/out.rres.json"
)

# Web Configurations
if (EMSCRIPTEN)
    # Convert list to string arguments
    list(LENGTH PRELOAD_FOLDERS PRELOAD_FOLDERS_LENGTH)
    math(EXPR LAST_INDEX "${PRELOAD_FOLDERS_LENGTH} - 1")
    foreach(index RANGE 0 ${LAST_INDEX})
        list(GET PRELOAD_FOLDERS ${index} folder)
        list(GET PRELOAD_DEST ${index} dest)

        set(combined_path "${folder}@${dest}")

        list(APPEND PRELOAD "--preload-file ${combined_path}")

        file(GLOB_RECURSE FILES_IN_DIRECTORY "${folder}/*")
        foreach(FILE_PATH ${FILES_IN_DIRECTORY})
            set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_DEPENDS ${FILE_PATH})
        endforeach()
    endforeach()

    list(LENGTH PRELOAD_FILES PRELOAD_FILES_LENGTH)
    math(EXPR LAST_INDEX "${PRELOAD_FILES_LENGTH} - 1")
    foreach(index RANGE 0 ${LAST_INDEX})
        list(GET PRELOAD_FILES ${index} file)
        list(GET PRELOAD_FILES_DEST ${index} dest)

        set(combined_path "${file}@${dest}")

        list(APPEND PRELOAD "--preload-file ${combined_path}")

        set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_DEPENDS ${file})
    endforeach()

    string(REPLACE ";" " " PRELOAD "${PRELOAD}")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    # Since WASM is used, ALLOW_MEMORY_GROWTH has no extra overheads
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s ALLOW_MEMORY_GROWTH=1 --no-heap-copy")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES
        COMPILE_FLAGS "-sMAIN_MODULE=2"
        LINK_FLAGS "-sMAIN_MODULE=2 ${PRELOAD}"
        SUFFIX ".html"
    )
else()
    # build scenes
    add_subdirectory(scenes)
endif()

set(EXEC_EXT "")

if (WIN32)
    set(EXEC_EXT ".exe")
endif()

if(NOT BUILD_PATHS)
    # Copy game executable to release directory
    file(MAKE_DIRECTORY ${RELEASE_EXECUTABLE_DIR})
    set(FILE_NAME ${PROJECT_NAME}${EXEC_EXT})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}
        ${RELEASE_EXECUTABLE_DIR}/${FILE_NAME}
    )

    # Copy mods folder to release directory
    file(MAKE_DIRECTORY ${RELEASE_MOD_DIR})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/mods
        ${RELEASE_MOD_DIR}
    )

    # Copy lua folder to release directory
    file(MAKE_DIRECTORY ${RELEASE_LUA_DIR})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_ROOT}/lua
        ${RELEASE_LUA_DIR}
    )

    # Copy resource files to release directory
    file(MAKE_DIRECTORY ${RELEASE_RES_DIR})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/out.rres
        ${RELEASE_RES_DIR}/out.rres
    )
    file(MAKE_DIRECTORY ${RELEASE_RES_DIR})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/out.rres.json
        ${RELEASE_RES_DIR}/out.rres.json
    )
endif()
