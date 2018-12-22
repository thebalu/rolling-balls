#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}


CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	//
	// shaderek betöltése
	//

	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
	});

	fence_prog.AttachShaders({
		{GL_VERTEX_SHADER, "fenceVert.vert"},
		{GL_FRAGMENT_SHADER, "fenceFrag.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				
		{ 1, "vs_in_normal" },			
		{ 2, "vs_in_tex0"}
	});

	
	m_program.LinkProgram();

	fence_prog.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_normal" },
		{ 2, "vs_in_tex0"}
		});

	fence_prog.LinkProgram();

	
	// table surface

	m_gpuBufferPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(30, 0, 30),
		glm::vec3(-30, 0, 30),
		glm::vec3(-30, 0, -30),
		glm::vec3(30, 0, -30)

	}
	);

	m_textureTable.FromFile("tex_table.jpg");

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_gpuBufferIndices.BufferData(
		std::vector<int>{
			0,2,1,
			0,3,2
	}
	);

	m_gpuBufferTex.BufferData(
		std::vector<glm::vec2>{
			glm::vec2(0,0),
			glm::vec2(0,1),
			glm::vec2(1,1),
			glm::vec2(1,0)
		}
	);

	m_gpuBufferNormal.BufferData(
		std::vector<glm::vec3>{
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0)
	}
	);


	// geometria VAO-ban való regisztrálása
	m_vao.Init(
		{
			{ CreateAttribute  <0, glm::vec3, 0, sizeof(glm::vec3)>, m_gpuBufferPos },
			{ CreateAttribute  <1, glm::vec3, 0, sizeof(glm::vec3)>, m_gpuBufferNormal },	// 1-es attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_gpuBufferNormal GPU pufferben vannak
			{ CreateAttribute  <2, glm::vec2, 0, sizeof(glm::vec2)>, m_gpuBufferTex }

		},
		m_gpuBufferIndices
	);


	// fence

	// cube, needs to be transformed
	fence_pos.BufferData(
		std::vector< glm::vec3 > {

			// front
			glm::vec3( 1.0f , 1.0f  , 1.0f),
			glm::vec3(-1.0f , 1.0f  , 1.0f),
			glm::vec3(-1.0f ,-1.0f  , 1.0f),
			glm::vec3( 1.0f ,-1.0f  , 1.0f),
	
			// back
			glm::vec3(1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),

			// left
			glm::vec3(-1.0f, 1.0f, 1.0f),
			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f,-1.0f, -1.0f),
			glm::vec3(-1.0f,-1.0f, 1.0f),

			// right
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f, 1.0f),


			// top
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f, 1.0f, 1.0f),

			// bot
			glm::vec3(1.0f, -1.0f, 1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, 1.0f)
		}
	);

	fence_indices.BufferData(
		std::vector<GLuint> {
			0,1,2,
			0,2,3,

			6,5,4,
			7,6,4,

			8,9,10,
			10,11,8,

			14,13,12,
			14,12,15,

			16,17,18,
			18,19,16,

			22,21,20,
			22,20,23
		}
	);

	fence_norm.BufferData(
		std::vector<glm::vec3>{
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),

			glm::vec3(0, 0, -1),
			glm::vec3(0, 0, -1),
			glm::vec3(0, 0, -1),
			glm::vec3(0, 0, -1),

			glm::vec3(-1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec3(-1, 0, 0),

			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),


			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),

			glm::vec3(0, -1, 0),
			glm::vec3(0, -1, 0),
			glm::vec3(0, -1, 0),
			glm::vec3(0, -1, 0)

		}
	);

	fence_bufftex.BufferData(
		std::vector<glm::vec2> {
			glm::vec2(0,0),
			glm::vec2(1,0),
			glm::vec2(1,1),
			glm::vec2(0,1),

			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1),

			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1),

			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1),

			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1),

			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1)
	}
	);

	fence_tex.FromFile("tex_fence.jpg");

	fence_vao.Init(
		{
			{ CreateAttribute  <0, glm::vec3, 0, sizeof(glm::vec3)>, fence_pos},
			{ CreateAttribute  <1, glm::vec3, 0, sizeof(glm::vec3)>, fence_norm},	
			{ CreateAttribute  <2, glm::vec2, 0, sizeof(glm::vec2)>, fence_bufftex}
		},
		fence_indices
	);


	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	return true;
}

void CMyApp::Clean()
{
	//glDeleteTextures(1, &m_skyboxTexture);
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// kocka
	m_vao.Bind();

	m_program.Use();

	// fõ kocka
	m_program.SetTexture("texImage", 0, m_textureTable);
	m_program.SetUniform("MVP", m_camera.GetViewProj());
	glDrawElements(GL_TRIANGLES, 8, GL_UNSIGNED_INT, nullptr);

	fence_vao.Bind();
	fence_prog.Use();
	fence_prog.SetTexture("texImage", 0, fence_tex);

	glm::mat4 fenceTransform = glm::translate(glm::vec3(0,1,31)) * glm::scale(glm::vec3(32, 1, 1));
	fence_prog.SetUniform("MVP", m_camera.GetViewProj() * fenceTransform);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	fenceTransform = glm::translate(glm::vec3(0, 1, -31)) * glm::scale(glm::vec3(32, 1, 1));
	fence_prog.SetUniform("MVP", m_camera.GetViewProj() * fenceTransform);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	fenceTransform = glm::translate(glm::vec3(31, 1, 0)) * glm::scale(glm::vec3(1, 1, 30));
	fence_prog.SetUniform("MVP", m_camera.GetViewProj() * fenceTransform);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	fenceTransform = glm::translate(glm::vec3(-31, 1, 0)) * glm::scale(glm::vec3(1, 1, 30));
	fence_prog.SetUniform("MVP", m_camera.GetViewProj() * fenceTransform);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}