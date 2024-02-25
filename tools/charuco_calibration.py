import configobj
import sys
import cv2

config_path = sys.argv[1]
config = configobj.ConfigObj(config_path)

SQUARE_LENGTH = 3.2 * 0.01
MARKER_LENGTH = 1.9 * 0.01

aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_1000)
board = cv2.aruco.CharucoBoard((5, 7), SQUARE_LENGTH, MARKER_LENGTH, aruco_dict)
arucoParams = cv2.aruco.DetectorParameters()

counter, corners_list, id_list = [], [], []

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, int(config.get("width")))
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, int(config.get("height")))

image_size = None

image_count = 0

while cap.isOpened():
    status, image = cap.read()
    key = cv2.waitKey(1)

    if not status:
        continue

    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    image_size = gray.shape
    corners, ids, rejected = cv2.aruco.detectMarkers(
        image,
        aruco_dict,
        parameters=arucoParams
    )

    if len(corners) == 0:
        cv2.imshow("Camera Calibration", image)
        if key == 27: # ESC key
            break
        continue

    resp, charuco_corners, charuco_ids = cv2.aruco.interpolateCornersCharuco(
        markerCorners=corners,
        markerIds=ids,
        image=gray,
        board=board
    )

    if not resp:
        continue

    cv2.aruco.drawDetectedCornersCharuco(image, charuco_corners, charuco_ids, (255, 0, 0))

    if key == 32: # Space key
        if resp > 20:
            corners_list.append(charuco_corners)
            id_list.append(charuco_ids)
            image_count += 1
            print("Image added: " + str(image_count))
    elif key == 27: # ESC key
        break
  
    cv2.imshow("Camera Calibration", image)

ret, matrix, distortion, rvecs, tvecs = cv2.aruco.calibrateCameraCharuco(
    charucoCorners=corners_list, 
    charucoIds=id_list,
    board=board, 
    imageSize=image_size, 
    cameraMatrix=None, 
    distCoeffs=None
)


config["focal_x"] = matrix[0][0]
config["focal_y"] = matrix[1][1]
config["center_x"] = matrix[0][2]
config["center_y"] = matrix[1][2]

config["k_1"] = distortion[0][0]
config["k_2"] = distortion[0][1]
config["p_1"] = distortion[0][2]
config["p_2"] = distortion[0][3]
config["k_3"] = distortion[0][4]

config.write()
fin = open(config_path, "r").read()
fout = open(config_path, "w")
fout.write(fin.replace(" = ", "=")[:-1]) # Remove special .ini formatting

print("fx " + str(matrix[0][0]))
print("fy " + str(matrix[1][1]))
print("cx " + str(matrix[0][2]))
print("cy " + str(matrix[1][2]))