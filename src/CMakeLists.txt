cmake_minimum_required(VERSION 3.12)
project(engine)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Dependencies
include(FetchContent)

set(BUILD_PATHS)

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

set(LUA_VERSION 5.4.5)
find_package(lua_static ${LUA_VERSION} QUIET)
if (NOT lua_static_FOUND)
    fetch_and_build_project(lua_static https://github.com/walterschell/Lua/archive/refs/tags/v${LUA_VERSION}.tar.gz)
endif()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/engine/*.c")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} raylib -lm lua_static json-c)
target_compile_options(${PROJECT_NAME} PUBLIC -Wno-deprecated-declarations)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

