#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "gCamera.h"

#include "ProgramObject.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "TextureObject.h"


struct Sphere {

	Sphere() {}
	//Sphere(float x_, float z_, float r_) : x(x_), z(z_), r(r_), v_x(0), v_z(0) {}
	Sphere(float x_, float z_, float r_, float v_x_, float v_z_) : x(x_), z(z_), r(r_), v_x(v_x_), v_z(v_z_), rot_z(0.0), rot_x(0.0) {}

	float x, z, r;

	float v_x, v_z;

	float rot_z, rot_x;
	void move(float delta_time) {
		x += v_x * delta_time;
		z += v_z * delta_time;

		rot_z -= v_x * delta_time / r;
		rot_x += v_z * delta_time  / r;

		//std::cerr << x << " " << z << "\n";
	}

	float mass() const { return r * r; }
};

bool checkCollision(const Sphere &a, const Sphere &b);
void collide( Sphere &a,  Sphere &b);

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void initFence();
	void initTable();
	void Clean();

	void Update();
	void Render();

	void renderFence();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
protected:
	// segédeljárások
	void TextureFromFileAttach(const char* filename, GLuint role) const;

	// shaderekhez szükséges változók
	//ProgramObject		m_programSkybox;	// skybox shaderek
	
	ProgramObject		table_prog;			// shaderek programja
	VertexArrayObject	table_vao;				// VAO objektum
	IndexBuffer			table_indices;	// indexek
	ArrayBuffer			table_pos;		// pozíciók tömbje
	ArrayBuffer			table_bufftex;		// pozíciók tömbje
	ArrayBuffer			table_norm;
	Texture2D			table_tex;

	ProgramObject		fence_prog;
	VertexArrayObject	fence_vao;
	IndexBuffer			fence_indices;
	ArrayBuffer			fence_pos;
	ArrayBuffer			fence_bufftex;
	ArrayBuffer			fence_norm;
	Texture2D			fence_tex;

	ProgramObject		sphere_prog;
	VertexArrayObject	sphere_vao;
	IndexBuffer			sphere_indices;
	ArrayBuffer			sphere_pos;
	ArrayBuffer			sphere_bufftex;
	//ArrayBuffer			sphere_norm;
	Texture2D			sphere_tex;
	void initSphere();
	glm::vec3 sphere_getUV(float u, float v);
	void renderSphere(Sphere ball);
	std::vector<Sphere> balls;
	void randomBalls(int n);
	float goFw = 0.0f, goRight = 0.0f;


	gCamera				m_camera;

	// nyers OGL azonosítók
	GLuint				m_skyboxTexture;
};

