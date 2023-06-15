# Titan Processing Examples
Try out examples using the library
## Building
### For example scripts
Run `g++ -o E ./example/apriltag/detection.cpp -Llib/ -lTitanProcessing -L/usr/local/lib -I/usr/include/opencv4 -lopencv_core -lopencv_videoio -lopencv_highgui -lapriltag -Wl,-Rlib/`
### For example projects
Run `cmake . && make clean && make`
# Examples
## Networking
### No reply
Sends a packet to IP with one vector
### With reply
Same as no reply example but includes a return vector
## IMU
## April Tag
## Helper