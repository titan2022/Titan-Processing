# Calibration tool
## How to run
1. Print the pattern on letter sized paper
2. Start the script with `python tools/calibration.py PATH/TO/processing.cfg`
3. Hold the pattern on a flat surface in front of the camera
4. Make sure the camera recognizes the pattern and displays a colorful overlay
5. All changes will be saved to the specified config file
## What it does
This script utilizes the basic [camera calibration](https://docs.opencv.org/4.x/dc/dbb/tutorial_py_calibration.html) logic provided by the documentation. It uses the pattern to determine focal size of the camera in pixels for specified resolution in the config. The focal x and y values should be [close to each other](https://ksimek.github.io/2013/08/13/intrinsic/#:~:text=In%20practice%2C,number%20of%20reasons). The script also calibrates the optical center. All four values are necessary for accurate Apriltag pose estimation.