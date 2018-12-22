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

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

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
	ProgramObject		m_program;			// shaderek programja
	//ProgramObject		m_programSkybox;	// skybox shaderek

	VertexArrayObject	m_vao;				// VAO objektum
	IndexBuffer			m_gpuBufferIndices;	// indexek
	ArrayBuffer			m_gpuBufferPos;		// pozíciók tömbje
	ArrayBuffer			m_gpuBufferTex;		// pozíciók tömbje
	ArrayBuffer			m_gpuBufferNormal;
	Texture2D			m_textureTable;

	ProgramObject		fence_prog;
	VertexArrayObject	fence_vao;
	IndexBuffer			fence_indices;
	ArrayBuffer			fence_pos;
	ArrayBuffer			fence_bufftex;
	ArrayBuffer			fence_norm;
	Texture2D			fence_tex;


	gCamera				m_camera;

	// nyers OGL azonosítók
	GLuint				m_skyboxTexture;
};

