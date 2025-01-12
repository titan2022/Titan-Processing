#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include "../../include/util/Config.hpp"
#include "util/Camera.hpp"

using namespace titan;

constexpr int CAM_ID = 1;
constexpr int CAM_CONFIG_INDEX = 0;

int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		std::cout << "1 argument was expected, " << argc - 1 << " were passed.\n";
		return 1;
	}
	const auto cam_name = argv[1];
	Config config(CONFIG_PATH, TAGS_PATH);
	Camera cam = config.cameras.at(cam_name);
	cv::VideoCapture stream = cam.openStream();

	while (stream.isOpened())
	{
		cv::Mat frame, out;
		stream >> frame;

		cv::undistort(frame, out, cam.cameraMat, cam.distCoeffs);

		cv::imshow("Apriltag Debug Window", out);
		if (cv::waitKey(1) == 27) // ESC key
		{ 
			break;
		}
	}

	return 0;
}