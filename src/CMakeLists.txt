cmake_minimum_required(VERSION 3.12)
project(engine)

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

if ("${PLATFORM}" STREQUAL "Web")
    set(EMSCRIPTEN TRUE)
else()
    set(EMSCRIPTEN FALSE)
endif()

file(GLOB SRC_FILES "${SRC_DIR}/engine/*.c" "${SRC_DIR}/engine/*/*.c")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

find_static_library(luajit-5.1 LUAJIT)
find_static_library(json-c JSON)

target_link_libraries(${PROJECT_NAME} raylib pthread -lm ${LUAJIT} ${JSON})

# Web Configurations
if (EMSCRIPTEN)
    set(ASSETS "${CMAKE_SOURCE_DIR}/assets")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
