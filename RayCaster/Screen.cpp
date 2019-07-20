#include "Screen.h"

Screen::Screen(int w, int h) : width(w), height(h)
{
	this->pixels = new RGBAColor[w * h];

	for (int i = 0; i < this->width; i++) {
		for (int j = 0; j < this->height; j++) {
			int index = j * this->width + i;

			this->pixels[index].r = RGBAColor::FloatToByte(i / float(this->width));    //red pixel
			this->pixels[index].g = RGBAColor::FloatToByte(j / float(this->height));      //green
			this->pixels[index].b = 0;      //blue
			this->pixels[index].a = 255;    //alpha
		}
	}
}

Screen::Screen(int w, int h, const RGBAColor& fillColor) : width(w), height(h)
{
	this->pixels = new RGBAColor[w * h];

	for (int i = 0; i < this->width; i++) {
		for (int j = 0; j < this->height; j++) {
			int index = j * this->width + i;

			this->pixels[index].r = fillColor.r;
			this->pixels[index].g = fillColor.g;
			this->pixels[index].b = fillColor.b;
			this->pixels[index].a = fillColor.a;
		}
	}
}

Screen::~Screen()
{
	delete[] this->pixels;
}

void Screen::SetPixel(int x, int y, const RGBAColor& color) const
{
	unsigned int index = y * this->width + x;
	this->pixels[index] = color;
}

const int& Screen::GetWidth() const
{
	return this->width;
}

const int& Screen::GetHeight() const
{
	return this->height;
}

const RGBAColor* Screen::GetScreen() const
{
	return this->pixels;
}
