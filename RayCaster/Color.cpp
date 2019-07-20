#include "Color.h"

RGBAColor::RGBAColor() : r(0), g(0), b(0), a(0) { }

RGBAColor::RGBAColor(byte r, byte g, byte b) : r(r), g(g), b(b), a(255) { }

RGBAColor::RGBAColor(byte r, byte g, byte b, byte a) : RGBAColor(r,g,b)
{
	this->a = 255;
}

RGBAColor::RGBAColor(float r, float g, float b)
{
	this->r = RGBAColor::FloatToByte(r);
	this->g = RGBAColor::FloatToByte(g);
	this->b = RGBAColor::FloatToByte(b);
	this->a = 255;
}

RGBAColor::RGBAColor(Vec3f colorVector)
{
	this->r = RGBAColor::FloatToByte(colorVector[0]);
	this->g = RGBAColor::FloatToByte(colorVector[1]);
	this->b = RGBAColor::FloatToByte(colorVector[2]);
	this->a = 255;
}

const float& RGBAColor::ByteToFloat(const byte& given_byte)
{
	return (1.0f / 255) * given_byte;
}

const byte& RGBAColor::FloatToByte(const float& color)
{
	return floor(color >= 1.0 ? 255 : color * 256.0);
}
