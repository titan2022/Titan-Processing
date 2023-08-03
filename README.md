# Titan Processing

## Building
### Linux
1. Install [nlohmann/json](https://github.com/nlohmann/json) using [vcpkg](https://github.com/Microsoft/vcpkg)
2. Run `cmake -B . -S . -DCMAKE_TOOLCHAIN_FILE=[PATH TO VCPKG]/scripts/buildsystems/vcpkg.cmake` to generate Makefile and link vcpkg with CMake (you can just run `cmake .` afterwards)
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