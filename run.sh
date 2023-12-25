#!/bin/bash

# Check if an argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <example_project>"
    exit 1
fi

# Name of the example project
example_project=$1

# Directory of the root project
root_directory="$(pwd)"

# Check if the build directory exists
build_directory="$root_directory/build"
if [ ! -d "$build_directory" ]; then
    echo "Error: Build directory not found. Please build the project first."
    exit 1
fi

# Directory of the example project
example_directory="$build_directory/examples/$example_project"
if [ ! -d "$example_directory" ]; then
    echo "Error: Example project '$example_project' not found."
    exit 1
fi

# Check if the executable exists
executable="$example_directory/$example_project"
if [ ! -f "$executable" ]; then
    echo "Error: Build does not exist for '$example_project'."
    exit 1
fi

running_directory="$root_directory/examples/$example_project"
if [ ! -d "$running_directory" ]; then
    echo "Error: Running directory does not exist for '$example_project'."
    exit 1
fi

cd $running_directory

# Run the executable
echo "Running $example_project..."
"$executable"

cd $root_directory
