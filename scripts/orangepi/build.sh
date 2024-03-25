#!/bin/bash

rm -r build
mkdir build && cd build
cmake -B . -S .. -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake -GNinja
ninja
