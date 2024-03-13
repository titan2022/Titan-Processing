#!/bin/bash

# This script downloads opencv and opencv-contrib, and then recurses through all the headers and copies them to the include directory.
include=/home/titan/Projects/opencv-headers/include
mkdir -p $include

cd ~/Projects/opencv-headers
git clone https://github.com/opencv/opencv --branch 4.9.0 --depth=1
git clone https://github.com/opencv/opencv_contrib --branch 4.9.0 --depth=1

for 
