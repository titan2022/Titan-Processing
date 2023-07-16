#ifndef OBJECT_DETECTION_HH
#define OBJECT_DETECTION_HH

#include "Cone.h"
#include "Robot.h"
#include "Cube.h"
#include "Constants.h"
#include "ContourTest.h"
#include "ProcessingModule.h"
#include <opencv2/core/mat.hpp>

using cv::Mat;

// Takes the aligned frame data and performs detection algorithms on the data to search for objects
class ObjectDetection : public ProcessingModule
{
private:
	static const std::vector<Requirements> requirements;
	static const std::vector<Outputs> outputs;
	ContourTest contourDetector;
	//Current matrix
	//cv::Mat depthMatrix;
	//cv::Mat positionMatrix;
	//cv::Mat colorMatrix;
	//void translatePositions(std::vector<Object>& objects);

	//void reset();
public:
	ObjectDetection();

	void initialize() override;
	void execute() override;
	inline const std::vector<Requirements>& getRequirements() const { return requirements; }

	//ObjectDetection(const CameraFrame& alignedFrame);
	////For processes that must be done for all objects
	//void load(const CameraFrame& alignedFrame);
	//resets the current matrices to the original CameraFrame matrices

	//The search algorithms for corresponding objects 
	template <typename T>
	std::vector<T*> searchFor()
	{
		contourDetector.Process<T>(colorMatrix);
		//std::vector<cv::Point2i>& coordinates = *contourDetector.getContourCenters();
		//std::vector<T*> results;
		//for (auto point : coordinates)
		//{
		//	Pixel pixel(point.x, point.y, originalFrame->xResolution, originalFrame->yResolution);
		//	T* object = new T(pixel, positionMatrix);
		//	results.push_back(object);

		//}
		//return results;
	}
	/*std::vector<Robot*> searchForRobots();
	std::vector<Cone> searchForCones();
	std::vector<Cube> searchForCubes();*/
	//std::vector<Object*> searchForObjects();

	//void showColorFrame();
	//void showContourFrame();
	//void printCenterRGB();
};

#endif
