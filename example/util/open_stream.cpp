#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "../../include/util/Config.hpp"
#include "util/Camera.hpp"

using namespace titan;

int main(int argc, char const *argv[])
{
	Config config(CONFIG_PATH, TAGS_PATH);

	if (argc != 2)
	{
		std::cout << "1 argument was expected, " << argc - 1 << " were passed.\n";
		return 1;
	}
	const auto cam_name = argv[1];
	Camera cam = config.cameras.at(cam_name);
	auto stream = cam.openStream();

	while (true)
	{
		cv::Mat frame;
		stream >> frame;

		cv::imshow("Debug Window", frame);
		if (cv::waitKey(1) == 27) // ESC key
		{
			break;
		}
	}

	return 0;
}