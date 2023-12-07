#ifndef OBJECT_DETECTION_MODULE_HH
#define OBJECT_DETECTION_MODULE_HH

#include <opencv2/core/mat.hpp>
#include "Cone.h"
#include "Cube.h"
#include "Constants.h"
#include "ContourTest.h"
#include "ProcessingModule.h"
#include "Robot.h"


// Takes the aligned frame data and performs detection algorithms on the data to search for objects
class ObjectDetectionModule : public ProcessingModule
{
private:
	ContourTest contourDetector;
	//Current matrix
	//cv::Mat depthMatrix;
	//cv::Mat positionMatrix;
	//cv::Mat colorMatrix;
	//void translatePositions(std::vector<Object>& objects);

	//void reset();
public:
	ObjectDetectionModule(const std::string& name);

	void execute() override;
	//void finalize() override;

	//cv::Mat getData() override;

	//ObjectDetectionModule(const CameraFrame& alignedFrame);
	////For processes that must be done for all objects
	//void load(const CameraFrame& alignedFrame);
	//resets the current matrices to the original CameraFrame matrices

	//The search algorithms for corresponding objects 
	//template <typename T>
	//std::vector<T*> searchFor()
	//{
	//	contourDetector.Process<T>(colorMatrix);
	//	std::vector<cv::Point2i>& coordinates = *contourDetector.getContourCenters();
	//	std::vector<T*> results;
	//	for (auto point : coordinates)
	//	{
	//		Pixel pixel(point.x, point.y, originalFrame->xResolution, originalFrame->yResolution);
	//		T* object = new T(pixel, positionMatrix);
	//		results.push_back(object);

	//	}
	//	return results;
	//}
	/*std::vector<Robot*> searchForRobots();
	std::vector<Cone> searchForCones();
	std::vector<Cube> searchForCubes();*/
	//std::vector<Object*> searchForObjects();

	//void showColorFrame();
	//void showContourFrame();
	//void printCenterRGB();
};

#endif
