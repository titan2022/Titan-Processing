#!/bin/bash

# We expect that you've already set up FRC-2024-Vision on the affected Jetson.

source ~/.venvs/FRC-2024-Vision/bin/activate

# Install GCC 13
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt -y install gcc-13 g++-13
mkdir -p ~/.local/bin/gcc-13
ln -s $(which gcc-13) ~/.local/bin/gcc-13/cc
ln -s $(which gcc-13) ~/.local/bin/gcc-13/gcc
ln -s $(which g++-13) ~/.local/bin/gcc-13/c++
ln -s $(which g++-13) ~/.local/bin/gcc-13/g++
export PATH="$HOME/.local/bin/gcc-13:$PATH"

# Install a newer CMake
wget -qO- https://raw.githubusercontent.com/ethanc8/titanian-repo/master/installers/cmake-bionic-arm64.sh | bas

# Install Ninja
sudo apt install ninja-build

# Install vcpkg
mkdir -p ~/titanian-download && cd ~/titanian-download
wget https://raw.githubusercontent.com/ethanc8/titanian-repo/master/debian/pool/main/v/vcpkg/vcpkg_2023.12.10-2+titan2022+jnano_arm64.deb
sudo apt -y install ./vcpkg_2023.12.10-2+titan2022+jnano_arm64.deb

# Install nlohmann/json
vcpkg install nlohmann-json
