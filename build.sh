#!/bin/bash

target_flag=''
debug_flag='-DCMAKE_BUILD_TYPE=Release'
build_examples=true
build_utils=true
build_release=false

build_dir='build'

rocks_pref=''

# cmake build
while getopts 'dt:-:' flag; do
  case "${flag}" in
    t) target_flag="$OPTARG" ;;
    d) debug_flag='-DCMAKE_BUILD_TYPE=Debug'; ;;
    -)
      case "${OPTARG}" in
        no-examples) build_examples=false ;;
        no-utils) build_utils=false ;;
        release) build_release=true ;;
        *) exit 1 ;;
      esac;;
    *) exit 1 ;;
  esac
done

echo $debug_flag

if [[ $target_flag == "web" ]]; then
  build_dir+="-web"
  rocks_pref='emmake'
elif [[ $target_flag == "windows" ]]; then
  build_dir+="-windows"
fi

cmake_flags=("$debug_flag")
cmake_flags+=("-DCMAKE_POLICY_VERSION_MINIMUM=3.5")
if ! $build_examples; then
  cmake_flags+=("-DBUILD_EXAMPLE=OFF")
else
  cmake_flags+=("-DBUILD_EXAMPLE=ON")
fi
if ! $build_utils; then
  cmake_flags+=("-DBUILD_UTILS=OFF")
else
  cmake_flags+=("-DBUILD_UTILS=ON")
fi
if $build_release; then
  cmake_flags+=("-DBUILD_PATHS=OFF")
  build_dir+="-release"
else
  cmake_flags+=("-DBUILD_PATHS=ON")
fi
cmake_flags="${cmake_flags[@]}"

if [[ $target_flag == "web" ]]; then
  echo 'Building for Emscripten..'
  emcmake cmake . \
        $cmake_flags \
        -DPLATFORM=Web \
        -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" \
        -DCMAKE_EXECUTABLE_SUFFIX=".html" \
        -G Ninja \
        -B $build_dir
  cmake --build $build_dir
elif [[ $target_flag == "windows" ]]; then
  export PATH=~/my_msvc/opt/msvc/bin/x64:$PATH
  [ ! -d "$build_dir/" ] && mkdir $build_dir
  cd $build_dir
  CC=cl CXX=cl cmake \
        $cmake_flags \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=1 \
        ..
  make
  cd ..
else
  cmake $cmake_flags -G Ninja -B $build_dir
  cmake --build $build_dir
fi
