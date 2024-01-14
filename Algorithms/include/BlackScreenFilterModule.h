#ifndef BLACK_SCREEN_FILTER_MODULE_H
#define BLACK_SCREEN_FILTER_MODULE_H

#include "HSLFilterModule.h"

class BlackScreenFilterModule : public HSLFilterModule
{
public:
	BlackScreenFilterModule();
	virtual void execute() override;
protected:
	static const vector<double> blackHSLThreshold;


};

#endif