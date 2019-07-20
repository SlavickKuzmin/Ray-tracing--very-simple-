#include "Sphere.h"

Sphere::~Sphere()
{
}

bool scene_intersect(const Vec3f& orig, const Vec3f& dir, const std::vector<Sphere>& spheres, Vec3f& hit, Vec3f& N, Material& material) {
	float spheres_dist = std::numeric_limits<float>::max();
	for (size_t i = 0; i < spheres.size(); i++) {
		float dist_i;
		if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
			spheres_dist = dist_i;
			hit = orig + dir * dist_i;
			N = (hit - spheres[i].center).normalize();
			material = spheres[i].material;
		}
	}
	float checkerboard_dist = std::numeric_limits<float>::max();
	if (fabs(dir.y) > 1e-3) {
		float d = -(orig.y + 4) / dir.y; // the checkerboard plane has equation y = -4
		Vec3f pt = orig + dir * d;
		if (d > 0 && fabs(pt.x) < 10 && pt.z<-10 && pt.z>-30 && d < spheres_dist) {
			checkerboard_dist = d;
			hit = pt;
			N = Vec3f(0, 1, 0);
			material.diffuse_color = (int(.5 * hit.x + 1000) + int(.5 * hit.z)) & 1 ? Vec3f(1, 1, 1) : Vec3f(0, 0, 0);
			material.diffuse_color = material.diffuse_color * .3;
		}
	}
	return std::min(spheres_dist, checkerboard_dist) < 1000;
}

Vec3f Sphere::reflect(const Vec3f& I, const Vec3f& N) {
	return I - N * 2.f * (I * N);
}

Vec3f Sphere::refract(const Vec3f& I, const Vec3f& N, const float& refractive_index) { // Snell's law
	float cosi = -std::max<float>(-1.f, std::min<float>(1.f, I * N));
	float etai = 1, etat = refractive_index;
	Vec3f n = N;
	if (cosi < 0) { // if the ray is inside the object, swap the indices and invert the normal to get the correct result
		cosi = -cosi;
		std::swap(etai, etat); n = -N;
	}
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	return k < 0 ? Vec3f(0, 0, 0) : I * eta + n * (eta * cosi - sqrtf(k));
}