//#ifndef HSL_OUTPUT_MODULE_H
//#define HSL_OUTPUT_MOUDLE_H
//
//#include "GraphicOutputModule.h"
//
//class HSLOutputModule : public GraphicOutputModule
//{
//public:
//	const static int RGB_MODE;
//
//	HSLOutputModule(const std::string& name, int colorMode);
//	void execute() override;
//
//	//WARNING: ONLY 1 HSLOutputModule will run properly at once
//	void mouseCallbackFunction(int event, int x, int y, int flags, void* userdata);
//private:
//	int colorMode;
//	cv::Mat mouseMatrix;
//	std::vector<double> lowerHSLBound;
//	std::vector<double> upperHSLBound;
//};
//
//#endif