#!/bin/bash

# Default value for the project type flag
project_type=""

# Parse command-line arguments
while getopts ":t:" opt; do
  case $opt in
    t)
      project_type="$OPTARG"
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

# Shift option index so $1 now refers to the first non-option argument
shift $((OPTIND -1))

# Check if an argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 [-t <web|windows>] <example_project>"
    exit 1
fi

# Name of the example project
example_project=$1

# Directory of the root project
root_directory="$(pwd)"

# Check if the build directory exists
build_directory="$root_directory/build"
if [ "$project_type" == "web" ]; then
    build_directory="$root_directory/build-web"
fi
if [ "$project_type" == "windows" ]; then
    build_directory="$root_directory/build-windows"
fi
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

# Check if there's an HTML file with the project name
html_file="$example_directory/$example_project.html"
if [ -f "$html_file" ]; then
    echo "Found HTML file for $example_project. Opening Emscripten web server..."
    emrun "$html_file"
    exit 0
fi



executable=""
if [ "$project_type" == "windows" ]; then
    executable="$example_directory/$example_project.exe"
else
    executable="$example_directory/$example_project"
fi

# Check if the executable exists
if [ ! -f "$executable" ]; then
    echo "Error: Build does not exist for '$example_project'."
    exit 1
fi

# Check if the running directory exists
running_directory="$root_directory/examples/$example_project"
if [ ! -d "$running_directory" ]; then
    echo "Error: Running directory does not exist for '$example_project'."
    exit 1
fi

# Create symbolic links to Lua definitions in the running directory
output_directory="$running_directory/bin"
[ ! -d "$output_directory" ] && mkdir -p "$output_directory"
for item in "$root_directory/lua"/*; do
    name=$(basename -- "$item")
    if ! [ -e "$output_directory/$name" ]; then
        ln -s "$item" "$output_directory/$name"
    fi
done

# Copy engine DLL to running directory
if [ "$project_type" == "windows" ]; then
    cp "$build_directory/src/engine.dll" "$running_directory"
fi

# cd $running_directory

# Run the executable
echo "Running $example_project..."
"$executable"

# cd $root_directory
