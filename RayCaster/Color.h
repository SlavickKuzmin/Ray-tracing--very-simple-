#pragma once

#include <algorithm>
#include "Geometry.h"

typedef unsigned char byte;

class RGBAColor 
{
public:
	RGBAColor();
	RGBAColor(byte r, byte g, byte b);
	RGBAColor(byte r, byte g, byte b, byte a);
	RGBAColor(float r, float g, float b);
	RGBAColor(Vec3f colorVector);
	byte r;
	byte g;
	byte b;
	byte a;
	static const float& ByteToFloat(const byte& given_byte);
	static const byte& FloatToByte(const float& color);
};
