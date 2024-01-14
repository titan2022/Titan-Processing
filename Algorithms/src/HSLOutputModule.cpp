//
//#include "HSLOutputModule.h"
//#include <opencv2/highgui/highgui.hpp>
//
//const int HSLOutputModule::RGB_MODE = 1;
//
//HSLOutputModule::HSLOutputModule(const std::string& name, int colorMode) : 
//	colorMode(colorMode), GraphicOutputModule(name)
//{
//	cv::namedWindow(name + " HSLOutputModule Graphic Output");
//	//cv::setMouseCallback(name + " HSLOutputModule Graphic Output", mouseCallbackFunction, 0);
//}
//
//void HSLOutputModule::execute()
//{
//	cv::imshow(name + " HSLOutputModule Graphic Output", *(inputMatrices[0]));
//	cv::waitKey(1);
//}
//
////void HSLOutputModule::mouseCallbackFunction(int event, int x, int y, int flags, void* userdata)
////{
////	cv::Mat pixel(graphicOutput, cv::Rect(x, y, cv::Size(1, 1)));
////	cv::Mat hsl;
////
////	cv::blur(pixel, hsl, cv::Size(5, 5), cv::Point(-1, -1));
////	mouseMatrix = cv::Mat(500, 500, pixel.type(), cv::Scalar(hsl.at<cv::Vec3f>(-1, -1)));
////
////	cv::cvtColor(hsl, hsl, cv::COLOR_BGR2HLS);
////
////	cv::imshow(name + " HSLOutputModule Mouse Output", mouseMatrix);
////	cv::waitKey(1);
////}