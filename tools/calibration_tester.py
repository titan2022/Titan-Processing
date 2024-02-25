import configobj
import sys
import cv2
import numpy as np

config_path = sys.argv[1]
config = configobj.ConfigObj(config_path)

def configint(key):
    return float(config.get(key))

mtx = np.matrix([[configint("focal_x"), 0, configint("center_x")],
                 [0, configint("focal_y"), configint("center_y")],
                 [0, 0, 1]])
dist = np.matrix([configint("k_1"), configint("k_2"), configint("p_1"), configint("p_2"), configint("k_3")])

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, int(config.get("width")))
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, int(config.get("height")))

while cap.isOpened():
    status, image = cap.read()
    key = cv2.waitKey(1)

    if key == 27: # ESC key
        break
    
    dst = cv2.undistort(image, mtx, dist, None, mtx)
    cv2.imshow("Undistorted", dst)