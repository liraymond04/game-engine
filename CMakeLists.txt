cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(main)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# build engine
add_subdirectory(src)

# example projects
add_subdirectory(examples/test-game)

# add engine headers to global include
target_include_directories(engine INTERFACE ${CMAKE_SOURCE_DIR}/src/engine)
