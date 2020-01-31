#include "nCollide.h"   // My header

namespace nCollide
{
	glm::vec3 closest_point_on_plane(const glm::vec3& point, const glm::vec3& n, float d)
	{
		float t = glm::dot(n, point) - d;
		return point - t * n;
	}

	int intersect_moving_sphere_plane(const glm::vec3& c, float r, const glm::vec3& v, const glm::vec3& n, float d, float& t, glm::vec3& q)
	{
		float dist = glm::dot(n, c) - d;
		if (glm::abs(dist) <= r)
		{
			// sphere is already overlapping the plane.
			// set time of intersection to 0 and q to sphere center
			t = 0.f;
			q = c;
			return -1;
		}
		else
		{
			float denom = glm::dot(n, v);
			if (denom * dist >= 0.f)
			{
				// no intersection as sphere moving parallel to or away from plane
				return 0;
			}
			else
			{
				// sphere is moving towards the plane
				// use +r in computations if sphere in front of plane, else -r
				float rad = dist > 0.f ? r : -r;
				t = (r - dist) / denom;
				q = c + (v * t) - (n * rad);
				return t <= 1.f ? 1 : 0;
			}
		}
	}

	int intersect_ray_sphere(const glm::vec3& point, const glm::vec3& dir, const glm::vec3& center, float radius, float& t, glm::vec3& q)
	{
		glm::vec3 m = point - center;
		float b = glm::dot(m, dir);
		float c = glm::dot(m, m) - radius * radius;
		// exit if r's origin outside s (c > 0) and r pointing away from s (b > 0)
		if (c > 0.f && b > 0.f) return 0;
		float discr = b * b - c;
		// a negative discriminant corresponds to ray missing sphere
		if (discr < 0.f) return 0;
		// ray now found to intersect sphere, compute against smallest t value of intersection
		t = -b - sqrt(discr);
		// if t is negative, ray started inside sphere, so clamp t to zero
		if (t < 0.f) t = 0.f;
		q = point + dir * t;
		return 1;
	}

	int intersect_moving_sphere_sphere(
		const glm::vec3& c0, float r0, const glm::vec3& v0,
		const glm::vec3& c1, float r1, const glm::vec3& v1, float& t)
	{
		glm::vec3 s = c1 - c0;  // sphere center separation
		glm::vec3 v = v1 - v0;  // relative motion of sphere 1 w.r.t. stationary s0
		float r = r1 + r0; // sum of sphere radii
		float c = glm::dot(s, s) - r * r;
		if (c < 0.f)
		{
			// spheres initially overlapping, exit early
			t = 0.f;
			return -1;
		}
		float a = glm::dot(v, v);
		if (a < FLT_EPSILON) return 0; // spheres not moving relative to eachother
		float b = glm::dot(v, s);
		if (b >= 0.f) return 0; // spheres not moving towards eachother
		float d = b * b - a * c;
		if (d < 0.f) return 0;  // no real root, so spheres do not intersect

		t = (-b - glm::sqrt(d)) / a;
		return t < 1.f ? 1 : 0;
	}
}