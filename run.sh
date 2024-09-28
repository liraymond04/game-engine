#!/bin/bash

# Default value for the project type flag
project_type=""

# Parse command-line arguments
while (( "$#" )); do
  case "$1" in
    -t)
      if [ -n "$2" ] && [ "${2:0:1}" != "-" ]; then
        project_type="$2"
        shift 2
      else
        echo "Option -t requires an argument." >&2
        exit 1
      fi
      ;;
    --)
      shift
      break
      ;;
    -*)
      echo "Invalid option: $1" >&2
      exit 1
      ;;
    *)
      example_project="$1"
      shift
      ;;
  esac
done

# Check if the example project name is provided
if [ -z "$example_project" ]; then
    echo "Usage: $0 [-t <web|windows>] <example_project>"
    exit 1
fi

# Directory of the root project
root_directory="$(pwd)"

# Determine the build directory based on the project type
build_directory="$root_directory/build"
if [ "$project_type" == "web" ]; then
    build_directory="$root_directory/build-web"
elif [ "$project_type" == "windows" ]; then
    build_directory="$root_directory/build-windows"
fi

# Check if the build directory exists
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

# Determine the executable based on the project type
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
    cp "$build_directory/src/engine.dll" "./"
fi

# cd $running_directory

# Check if the logs directory exists
logs_directory="$root_directory/logs"
if [ ! -d "$logs_directory" ]; then
    mkdir $logs_directory
fi

# Run the executable
echo "Running $example_project..."
"$executable"

# cd $root_directory
