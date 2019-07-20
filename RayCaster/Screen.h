#pragma once

#include "Color.h"

class Screen
{
public:
	// Default constructor
	// Default color = black
	Screen(int w, int h);
	Screen(int width, int height, const RGBAColor& fillColor);
	~Screen();
	void SetPixel(int x, int y, const RGBAColor& color) const;
	const int& GetWidth() const;
	const int& GetHeight() const;
	const RGBAColor* GetScreen() const;
private:
	int width;
	int height;
	RGBAColor* pixels;
};

