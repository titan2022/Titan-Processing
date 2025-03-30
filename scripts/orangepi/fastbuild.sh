#!/bin/bash

source ~/miniforge3/etc/profile.d/conda.sh
conda activate Titan-Processing

mkdir -p build
cd build

export CC=clang
export CXX=clang++
export CXXFLAGS="-pthread"

# Debug/development build
cmake -B . -S .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DBUILD_EXAMPLES=1 -DCMAKE_BUILD_TYPE=RelWithDebInfo -GNinja

# Actually run the build
ninja
