# Titan Processing

## Building
### Linux
1. Install [nlohmann/json](https://github.com/nlohmann/json) using [vcpkg](https://github.com/Microsoft/vcpkg)
   * If you want to install vcpkg using our binaries, do:
  ```bash
  curl -OL https://github.com/ethanc8/titanian-repo/raw/master/debian/pool/main/v/vcpkg/vcpkg_2023.12.10-2+titan2022_amd64.deb
  sudo apt install ./vcpkg_2023.12.10-2+titan2022_amd64.deb
  vcpkg install nlohmann-json
  ```
2. Run `cmake -B . -S . -DCMAKE_TOOLCHAIN_FILE=[PATH TO VCPKG]/scripts/buildsystems/vcpkg.cmake` (`cmake -B . -S . -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake` if installed via our binaries) to generate Makefile and link vcpkg with CMake (you can just run `cmake .` afterwards)
   * If you want to use `clangd` in your IDE, add `-DCMAKE_EXPORT_COMPILE_COMMANDS=1` to the end of the CMake commands
3. Run `make` to build
4. Done!
### Windows
WIP
### Build options
* `-DBUILD_EXAMPLES`: builds example files (found in `example/` folder)
* `-DNO_REALSENSE`: builds without RealSense code for compatiblity
## Using
WIP
## Other tools
- [Camset](https://github.com/azeam/camset)
- [calibration.py](tools/calibration.md)
## TODO:
- make a template CMake script
- publish binaries