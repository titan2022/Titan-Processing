import cv2

aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_1000)
board = cv2.aruco.CharucoBoard((5, 7), 1, 0.6, aruco_dict)
board_img = board.generateImage((2550, 3300), 0, 75)

cv2.imwrite("./out/charuco_5x7.png", board_img)
print("Done")