#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include "../../include/util/Config.hpp"
#include "util/Camera.hpp"

using namespace titan;

int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		std::cout << "1 argument was expected, " << argc - 1 << " were passed.\n";
		return 1;
	}
	const char *cam_name = argv[1];
	Config config(CONFIG_PATH, TAGS_PATH);
	Camera cam = config.cameras.at(cam_name);
	cv::VideoCapture stream = cam.openStream();

	while (stream.isOpened())
	{
		cv::Mat frame;
		stream >> frame;

		cv::Mat out;
		cv::undistort(frame, out, cam.cameraMat, cam.distCoeffs);

		cv::imshow("Apriltag Debug Window", out);
		if (cv::waitKey(1) == 27) // ESC key
		{
			break;
		}
	}

	return 0;
}