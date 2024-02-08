cmake_minimum_required(VERSION 3.12)
project(engine)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

find_package(json-c QUIET) # QUIET or REQUIRED
if (NOT json-c_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    json-c
    URL https://github.com/json-c/json-c/archive/refs/tags/json-c-0.17-20230812.tar.gz
  )
  FetchContent_GetProperties(json-c)
  if (NOT json-c_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(json-c)
    set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${json-c_SOURCE_DIR} ${json-c_BINARY_DIR})
  endif()
endif()

set(LUA_VERSION 5.4.5)
find_package(lua_static ${LUA_VERSION} QUIET) # QUIET or REQUIRED
if (NOT lua_static_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    lua_static
    URL https://github.com/walterschell/Lua/archive/refs/tags/v${LUA_VERSION}.tar.gz
  )
  FetchContent_GetProperties(lua_static)
  if (NOT lua_static_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(lua_static)
    set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${lua_static_SOURCE_DIR} ${lua_static_BINARY_DIR})
  endif()
endif()

function(find_static_library LIB_NAME OUT)

    if (WIN32 OR MSVC)
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
    elseif (UNIX)
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    endif()      

    find_library(
        FOUND_${LIB_NAME}_STATIC
        ${LIB_NAME}
        )

    if (FOUND_${LIB_NAME}_STATIC)
        get_filename_component(ABS_FILE ${FOUND_${LIB_NAME}_STATIC} ABSOLUTE)
    else()
        message(SEND_ERROR "Unable to find library ${LIB_NAME}")
    endif()

    set(${OUT} ${ABS_FILE} PARENT_SCOPE)

endfunction()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SRC_FILES "${SRC_DIR}/engine/*.c" "${SRC_DIR}/engine/*/*.c")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} raylib pthread -lm lua_static json-c -ldl)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

