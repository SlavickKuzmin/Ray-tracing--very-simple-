#include "RayTracker.h"
#include <corecrt_math_defines.h>

RayTracker::RayTracker(int width, int height)
{
	this->screen = new Screen(width, height, RGBAColor(0.f,0.f,0.f));
	this->spheres = new std::vector<Sphere>();
	this->lights = new std::vector<Light>();
	this->reflect_depth_max = 0;
}

RayTracker::~RayTracker()
{
	delete this->screen;
	delete this->spheres;
	delete this->lights;
}

void RayTracker::AddSphere(const Sphere& sphere) const
{
	this->spheres->push_back(sphere);
}

void RayTracker::AddLight(const Light& light) const
{
	this->lights->push_back(light);
}

void RayTracker::SetDepth(int depth)
{
	this->reflect_depth_max = depth;
}

void RayTracker::DoTracking()
{
	const int width = this->GetScreen().GetWidth();
	const int height = this->GetScreen().GetHeight();
	const int fov = M_PI / 2.;

#pragma omp parallel for
	for (size_t j = 0; j < height; j++) {
		std::cout << "Process... " << j << " from " << height << std::endl;
#pragma omp parallel for
		for (size_t i = 0; i < width; i++) {
			float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
			float y = -(2 * ((height-j) + 0.5) / (float)height - 1) * tan(fov / 2.);
			Vec3f dir = Vec3f(x, y, -1).normalize();

			Vec3f c = cast_ray(Vec3f(0, 0, 0), dir);
			float max = std::max(c[0], std::max(c[1], c[2]));
			if (max > 1) c = c * (1. / max);

			this->GetScreen().SetPixel(i,j, c);
		}
	}
}

const Screen& RayTracker::GetScreen()
{
	return *(this->screen);
}

Vec3f RayTracker::cast_ray(const Vec3f& orig, const Vec3f& dir, int depth) {
	Vec3f point, N;
	Material material;

	if (depth > this->reflect_depth_max || !scene_intersect(orig, dir, *spheres, point, N, material)) {
		return Vec3f(0.2, 0.7, 0.8); // background color
	}

	Vec3f reflect_dir = Sphere::reflect(dir, N).normalize();
	Vec3f refract_dir = Sphere::refract(dir, N, material.refractive_index).normalize();
	Vec3f reflect_orig = reflect_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3; // offset the original point to avoid occlusion by the object itself
	Vec3f refract_orig = refract_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;
	Vec3f reflect_color = cast_ray(reflect_orig, reflect_dir, depth+1);
	Vec3f refract_color = cast_ray(refract_orig, refract_dir, depth + 1);

	float diffuse_light_intensity = 0, specular_light_intensity = 0;
	for (size_t i = 0; i < lights->size(); i++) {
		Vec3f light_dir = (lights->operator[](i).position - point).normalize();
		float light_distance = (lights->operator[](i).position - point).norm();

		Vec3f shadow_orig = light_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3; // checking if the point lies in the shadow of the lights[i]
		Vec3f shadow_pt, shadow_N;
		Material tmpmaterial;
		if (scene_intersect(shadow_orig, light_dir, *spheres, shadow_pt, shadow_N, tmpmaterial) && (shadow_pt - shadow_orig).norm() < light_distance)
			continue;

		diffuse_light_intensity += lights->operator[](i).intensity * std::max(0.f, light_dir * N);
		specular_light_intensity += powf(std::max(0.f, -Sphere::reflect(-light_dir, N) * dir), material.specular_exponent) * lights->operator[](i).intensity;
	}
	return material.diffuse_color * diffuse_light_intensity * material.albedo[0] + Vec3f(1., 1., 1.) * specular_light_intensity * material.albedo[1] + reflect_color * material.albedo[2] + refract_color * material.albedo[3];
}