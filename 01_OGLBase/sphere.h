#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>



struct Sphere {

	Sphere() {}
	//Sphere(float x_, float z_, float r_) : x(x_), z(z_), r(r_), v_x(0), v_z(0) {}
	Sphere(float x_, float z_, float r_, float v_x_, float v_z_, int mat_) : x(x_), z(z_), r(r_), v_x(v_x_), v_z(v_z_), rot_z(0.0),
		rot_x(0.0), material(mat_), rotate_dist(glm::vec3(0, 0, 0)), last_rot(glm::rotate(0.0f, glm::vec3(1, 0, 0))) , is_gold(false) {}

	float x, z, r;

	float v_x, v_z;
	int material;

	float rot_z, rot_x, rot_angle;
	glm::vec3 rotate_dist;
	glm::mat4 last_rot;
	bool is_gold;

	void move(float delta_time) {

		if (x - r <= -30.0) { v_x *= -1; reset_rotation(); x = -29.99 + r; }
		if (x + r >= 30.0) { v_x *= -1; reset_rotation(); x = 29.99 - r; }
		if (z + r >= 30.0) { v_z *= -1; reset_rotation(); z = 29.99 - r; }
		if (z - r <= -30.0) { v_z *= -1; reset_rotation(); z = -29.99 + r; }


		glm::vec3 vel(v_x, 0, v_z);
		if (glm::length(vel) > 0) {
			glm::vec3 acceleration = -3.0f * delta_time / mass() * glm::normalize(vel);
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

		//std::cerr << x << " " << z << "\n";
	}

	glm::mat4 rot_matrix() const {
		if (v_x == 0 && v_z == 0 || rotate_dist == glm::vec3(0, 0, 0))  return last_rot;

		glm::vec3 rot_axis = glm::cross(glm::vec3(0, 1, 0), rotate_dist);
		return glm::rotate(rot_angle, rot_axis)* last_rot;
	}

	void reset_rotation() {
		last_rot = rot_matrix();
		rot_x = 0;
		rot_z = 0;
		rotate_dist = glm::vec3(0, 0, 0);
		rot_angle = 0;
	}
	float mass() const { return material * r * r * r; }
};



bool checkCollision(const Sphere &a, const Sphere &b);
void collide(Sphere &a, Sphere &b);