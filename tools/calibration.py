import configobj
import sys
import cv2
import numpy as np

config_path = sys.argv[1]
config = configobj.ConfigObj(config_path)

CHECKERBOARD = (6, 9)

criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
  
obj_points = []
img_points = []

obj = np.zeros((1, CHECKERBOARD[0] * CHECKERBOARD[1], 3), np.float32)
obj[0, :, :2] = np.mgrid[0:CHECKERBOARD[0], 0:CHECKERBOARD[1]].T.reshape(-1, 2)
  
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, int(config.get("width")))
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, int(config.get("height")))
  
while cap.isOpened():
    status, image = cap.read()
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
  
    ret, corners = cv2.findChessboardCorners(
        gray,
        CHECKERBOARD,
        cv2.CALIB_CB_ADAPTIVE_THRESH + cv2.CALIB_CB_FAST_CHECK + cv2.CALIB_CB_NORMALIZE_IMAGE
    )
    
    if ret == True:
        obj_points.append(obj)
        corners2 = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
        img_points.append(corners2)
        image = cv2.drawChessboardCorners(image, CHECKERBOARD, corners2, ret)
  
    cv2.imshow("Camera Calibration", image)
    if cv2.waitKey(1) == 27: # ESC key
        break

ret, matrix, distortion, r_vecs, t_vecs = cv2.calibrateCamera(obj_points, img_points, gray.shape[::-1], None, None)

config["focal_x"] = matrix[0][0]
config["focal_y"] = matrix[1][1]
config["center_x"] = matrix[0][2]
config["center_y"] = matrix[1][2]

config.write()
fin = open(config_path, "r").read()
fout = open(config_path, "w")
fout.write(fin.replace(" = ", "=")[:-1]) # Remove special .ini formatting

print("fx " + str(matrix[0][0]))
print("fy " + str(matrix[1][1]))
print("cx " + str(matrix[0][2]))
print("cy " + str(matrix[1][2]))