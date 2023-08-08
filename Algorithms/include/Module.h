#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <iostream>

/// <summary>
/// Abstract class to be loaded into VisionProcessor
/// </summary>
class Module
{
protected:
	Module(const std::string& i_name);
	std::string name;
public:
	inline const std::string& getName() { return name; }
	inline virtual void initialize() { }
	inline virtual void execute() { }
	//inline virtual void finalize() { }
};

#endif