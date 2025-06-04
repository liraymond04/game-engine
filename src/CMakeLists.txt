cmake_minimum_required(VERSION 3.12)
project(engine)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Dependencies
include(FetchContent)

set(BUILD_PATHS)
set(RELEASE_ENGINE_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLUA_USE_POSIX -DLUA_USE_DLOPEN")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLUA_USE_POSIX -DLUA_USE_DLOPEN")
endif()

# Function to fetch and build an external project
function(fetch_and_build_project TARGET_NAME URL)
    FetchContent_Declare(
        ${TARGET_NAME}
        URL ${URL}
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_GetProperties(${TARGET_NAME})
    if (NOT ${TARGET_NAME}_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(${TARGET_NAME})
        set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE) # Example customization, adjust as needed
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # Example customization, adjust as needed
    endif()
endfunction()

function(fetch_and_build_project_commit TARGET_NAME GITHUB_URL COMMIT_HASH)
    FetchContent_Declare(
        ${TARGET_NAME}
        GIT_REPOSITORY ${GITHUB_URL}
        GIT_TAG ${COMMIT_HASH}
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_GetProperties(${TARGET_NAME})
    if (NOT ${TARGET_NAME}_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(${TARGET_NAME})
        set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE) # Example customization
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)   # Example customization
    endif()
endfunction()

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    fetch_and_build_project(raylib https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz)
endif()

set(JSON_C_VERSION "0.17-20230812")
find_package(json-c QUIET)
if (NOT json-c_FOUND OR WIN32)
    fetch_and_build_project(json-c https://github.com/json-c/json-c/archive/refs/tags/json-c-${JSON_C_VERSION}.tar.gz)
endif()

set(LUA_VERSION 5.4.7)
find_package(lua_static ${LUA_VERSION} QUIET)
if (NOT lua_static_FOUND)
    fetch_and_build_project_commit(lua_static https://github.com/walterschell/Lua 3ed55a56eaa05c9221f40b3c07d0e908eb1067b0)
endif()

if(NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(ZLOG_VERSION 1.2.18)
    find_package(zlog)
    if (NOT zlog_FOUND)
        fetch_and_build_project_commit(zlog https://github.com/liraymond04/zlog cebb12ac56a1e18c3411e765ed8397a159732387)
    endif()
endif()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/engine/*.c")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(DATACHANNEL-WASM_VERSION 0.3.5)
    find_package(datachannel-wasm ${DATACHANNEL-WASM_VERSION} QUIET)
    if (NOT datachannel-wasm_FOUND)
        FetchContent_Declare(datachannel-wasm
            GIT_REPOSITORY https://github.com/liraymond04/datachannel-wasm.git
            GIT_TAG v${DATACHANNEL-WASM_VERSION}
            GIT_SUBMODULES_RECURSE ON
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )

        FetchContent_GetProperties(datachannel-wasm)
        if (NOT datachannel-wasm_POPULATED)
            set(FETCHCONTENT_QUIET NO)
            FetchContent_MakeAvailable(datachannel-wasm)
            set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE) # Example customization, adjust as needed
            set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # Example customization, adjust as needed
        endif()
    endif()
    target_link_libraries(${PROJECT_NAME} datachannel-wasm)
else()
    set(CMAKE_PROJECT_VERSION 1.0.0)
    set(LIBDATACHANNEL_VERSION 0.22.6)
    find_package(libdatachannel ${LIBDATACHANNEL_VERSION} QUIET)
    if (NOT libdatachannel_FOUND)
        FetchContent_Declare(libdatachannel
            GIT_REPOSITORY https://github.com/paullouisageneau/libdatachannel.git
            GIT_TAG a81daaca4ca420be200ce9c9feb1cc76c8e3aa46
            GIT_SUBMODULES_RECURSE ON
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )

        FetchContent_GetProperties(libdatachannel)
        if (NOT libdatachannel_POPULATED)
            set(FETCHCONTENT_QUIET NO)
            FetchContent_MakeAvailable(libdatachannel)
            set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE) # Example customization, adjust as needed
            set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # Example customization, adjust as needed
        endif()
    endif()
    target_link_libraries(${PROJECT_NAME} datachannel)
endif()

if(NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    target_link_libraries(${PROJECT_NAME} zlog)
endif()

target_link_libraries(${PROJECT_NAME} raylib -lm lua_static json-c uuid pthread)
if (NOT WIN32)
    target_compile_options(${PROJECT_NAME} PUBLIC -Wno-deprecated-declarations)
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

