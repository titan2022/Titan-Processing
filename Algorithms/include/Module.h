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
	std::string name;
public:
	inline std::string getName() { return name; }
	inline virtual void initialize() { }
	inline virtual void execute() { }
	inline virtual void finalize() { }
};

std::ostream& operator<< (std::ostream& prev, Module mod)
{
	return prev << mod.getName();
}

#endif