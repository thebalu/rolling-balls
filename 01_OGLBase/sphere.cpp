#include "sphere.h"



bool checkCollision(const Sphere &a, const Sphere &b) {
	return (a.x - b.x)*(a.x - b.x) + (a.z - b.z) *(a.z - b.z) + (a.r - b.r) * (a.r - b.r) < (a.r + b.r) * (a.r + b.r);
}

void collide(Sphere & a, Sphere & b)
{


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
