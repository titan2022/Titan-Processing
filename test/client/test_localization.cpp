#include <filesystem>
#include <gtest/gtest.h>

#include "apriltag/ApriltagDetector.hpp"
#include "apriltag/Localizer.hpp"
#include "physics/PoseFilter.hpp"
#include "util/Config.hpp"
#include "util/Vector3D.hpp"

namespace fs = std::filesystem;
using namespace titan;

fs::path imageDirectory = fs::path(fs::current_path().parent_path() / "test" / "client" / "util" / "img");

// Tests apriltag localization without field or robot offsets (single-camera)
TEST(LocalizationTest, CameraApriltagCentric)
{
    
}

// Tests apriltag localization with field offset but without camera-robot offset (single-camera)
TEST(LocalizationTest, SingleCameraFieldCentric)
{
    Config config(CONFIG_PATH, TAGS_PATH);

    // Expected poses
    std::vector<Vector3D> positions = {
        Vector3D{0, 0, 0}
    };
    std::vector<Vector3D> rotations = {
        Vector3D{0, 0, 0}
    };

    // Test images
    cv::VideoCapture stream((imageDirectory / "SingleCameraFieldCentric").c_str());

    int frameIterator = 0;
    auto poseHandler = [&](Vector3D &pos, Vector3D &rot) {
		EXPECT_DOUBLE_EQ(pos.getX(), positions[frameIterator].getX());
        EXPECT_DOUBLE_EQ(pos.getY(), positions[frameIterator].getY());
        EXPECT_DOUBLE_EQ(pos.getZ(), positions[frameIterator].getZ());

        EXPECT_DOUBLE_EQ(rot.getX(), rotations[frameIterator].getX());
        EXPECT_DOUBLE_EQ(rot.getY(), rotations[frameIterator].getY());
        EXPECT_DOUBLE_EQ(rot.getZ(), rotations[frameIterator].getZ());
	};

    PoseFilter filter(config);
	Localizer localizer(config, filter, poseHandler);

    std::thread localizerThread(&Localizer::threadMainloop, std::ref(localizer));

    Camera cam = config.cameras.at("Arducam");
    ApriltagDetector detector(stream, true, config, cam, localizer);
    detector.detect();

    localizerThread.join();
}

// Tests apriltag localization with field and robot offsets (single-camera)
TEST(LocalizationTest, SingleCameraRobotFieldCentric)
{

}