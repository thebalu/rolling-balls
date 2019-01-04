#include "sphere.h"
#include "iostream"



bool checkCollision(const Sphere &a, const Sphere &b) {
	if (!a.alive || !b.alive) return false;
	return (a.x - b.x)*(a.x - b.x) + (a.z - b.z) *(a.z - b.z) + (a.r - b.r) * (a.r - b.r) < (a.r + b.r) * (a.r + b.r);
}

void collide(Sphere & a, Sphere & b)
{

	if (!a.alive || !b.alive) return;

	glm::vec2 va(a.v_x, a.v_z), vb(b.v_x, b.v_z);
	glm::vec2 posa(a.x, a.z), posb(b.x, b.z);
	glm::vec2 new_va = va - (2 * b.mass()) / (a.mass() + b.mass()) * (glm::dot((va - vb), (posa - posb)) / (glm::length(posa - posb) * glm::length(posa - posb))) * (posa - posb);
	glm::vec2 new_vb = vb - (2 * a.mass()) / (a.mass() + b.mass()) * (glm::dot((vb - va), (posb - posa)) / (glm::length(posb - posa) * glm::length(posb - posa))) * (posb - posa);


	a.v_x = new_va.x;
	a.v_z = new_va.y;
	b.v_x = new_vb.x;
	b.v_z = new_vb.y;

	// to prevent sticking together, move them apart until they no longer collide
	while (checkCollision(a, b)) {
		a.move(0.01);
		b.move(0.01);
	}

	a.reset_rotation();
	b.reset_rotation();


}

void Sphere::move(float delta_time) {

	if (x - r <= -30.0) { v_x *= -1; reset_rotation(); x = -29.99 + r; }
	if (x + r >= 30.0) { v_x *= -1; reset_rotation(); x = 29.99 - r; }
	if (z + r >= 30.0) { v_z *= -1; reset_rotation(); z = 29.99 - r; }
	if (z - r <= -30.0) { v_z *= -1; reset_rotation(); z = -29.99 + r; }


	glm::vec3 vel(v_x, 0, v_z);
	if (glm::length(vel) > 0) {
		glm::vec3 acceleration = -4.0f * delta_time / mass() * glm::normalize(vel);
		if (glm::length(vel) < glm::length(acceleration)) {
			reset_rotation();
			vel = glm::vec3(0.0f);
			v_x = 0;
			v_z = 0;
			acceleration = glm::vec3(0.0f);
		}
		else {
			vel += acceleration;
			v_x = vel.x;
			v_z = vel.z;
		}
	}

	x += v_x * delta_time;
	z += v_z * delta_time;

	rot_z -= v_x * delta_time / r;
	rot_x += v_z * delta_time / r;
	rotate_dist += glm::vec3(v_x * delta_time, 0, v_z * delta_time);
	rot_angle = glm::length(rotate_dist) / r;

	if (v_x <= 0.01 && v_z <= 0.01) { stopped_since += delta_time;  std::cerr << stopped_since << std::endl; }
	else stopped_since = 0;

	if (stopped_since > 5.0 && !is_gold) alive = false; 

	//std::cerr << x << " " << z << "\n";
}