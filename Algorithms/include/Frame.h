#ifndef FRAME_H
#define FRAME_H

/// <summary>
/// Abstract class for all frames
/// </summary>
class Frame
{
public:
	virtual size_t getXResolution() const = 0;
	virtual size_t getYResolution() const = 0;

	virtual bool hasDepth() const = 0;
	virtual bool hasColor() const = 0;
	virtual bool hasPosition() const = 0;
};

#endif