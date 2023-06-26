
#include "../include/Object.h"
#include <string>

Object::Object()
{

}

Object::Object(Vector inputPosition) :
	position(inputPosition)
{

}

Vector& Object::getPosition()
{
	return position;
}

double Object::getDistance()
{
	return position.getMagnitude();
}

std::string Object::toString()
{
	return "Object: " +  position.toString();
}