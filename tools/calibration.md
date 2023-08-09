# Calibration tools
## How to run
1. Print the pattern on letter sized paper
2. Check that dimentions of the squares or/and markers match the script (in cm)
3. Start the script with `python tools/TYPE_calibration.py PATH/TO/processing.cfg`
4. Hold the pattern on a flat surface in front of the camera
5. Make sure the camera recognizes the pattern and displays a colorful overlay
6. All changes will be saved to the specified config file
## What it does
This script utilizes the basic [camera calibration](https://docs.opencv.org/4.x/dc/dbb/tutorial_py_calibration.html) logic provided by the documentation. It uses the pattern to determine focal size of the camera in pixels for specified resolution in the config. The focal x and y values should be [close to each other](https://ksimek.github.io/2013/08/13/intrinsic/#:~:text=In%20practice%2C,number%20of%20reasons). The script also calibrates the optical center and distortion coefficients. The first four values are necessary for accurate Apriltag pose estimation, while all distortion coefficients may be left at 0.