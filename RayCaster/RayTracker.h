#pragma once

#include "Color.h"
#include "Geometry.h"
#include "Sphere.h"
#include "Screen.h"
#include "Light.h"

class RayTracker
{
public:
	RayTracker(int width, int height);
	~RayTracker();
	void AddSphere(const Sphere& sphere) const;
	void AddLight(const Light& light) const;
	void SetDepth(int depth);
	void DoTracking();
	const Screen& GetScreen();
	Vec3f cast_ray(const Vec3f& orig, const Vec3f& dir, int depth = 0);
private:
	Screen* screen;
	std::vector<Sphere> *spheres;
	std::vector<Light> *lights;
	int reflect_depth_max;
};