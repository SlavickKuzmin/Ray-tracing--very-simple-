#pragma once

#include "Geometry.h"
#include "Material.h"
#include <algorithm>

class Sphere
{
public:
	Vec3f center;
	float radius;
	Material material;

	Sphere(const Vec3f& c, const float& r, Material &material) : center(c), radius(r) 
	{
		// take ownership.
		this->material = material;
	}
	~Sphere();

	bool ray_intersect(const Vec3f& orig, const Vec3f& dir, float& t0) const {
		Vec3f L = center - orig;
		float tca = L * dir;
		float d2 = L * L - tca * tca;
		if (d2 > radius * radius) return false;
		float thc = sqrtf(radius * radius - d2);
		t0 = tca - thc;
		float t1 = tca + thc;
		if (t0 < 0) t0 = t1;
		if (t0 < 0) return false;
		return true;
	}

	static Vec3f reflect(const Vec3f& I, const Vec3f& N);
	static Vec3f refract(const Vec3f& I, const Vec3f& N, const float& refractive_index);
};

bool scene_intersect(const Vec3f& orig, const Vec3f& dir, const std::vector<Sphere>& spheres, Vec3f& hit, Vec3f& N, Material& material);