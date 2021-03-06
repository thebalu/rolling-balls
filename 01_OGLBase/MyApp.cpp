#include "MyApp.h"

#include <math.h>
#include <vector>
#include <cstdlib>
#include <random>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>

CMyApp::CMyApp(void)
{
	//m_camera.SetView(glm::vec3(70.0, 70.0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}


CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

bool CMyApp::Init()
{
	
	glClearColor(0.125f, 0.25f, 0.25f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)

	
	initTable();
	initFence();
	initSphere();

	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	randomBalls(10);
	// mi iranyitjuk a 0. golyot, ne legyen kezdosebessege
	balls[0].v_x = 0;
	balls[0].v_z = 0;
	balls[0].is_gold = true;

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

	//std::cerr << "Update\n";
	

	glm::vec2 cam_dir = glm::normalize( glm::vec2( (m_camera.GetAt() - m_camera.GetEye()).x, (m_camera.GetAt() - m_camera.GetEye()).z));
	glm::vec2 right_dir(-cam_dir.y, cam_dir.x);
	glm::vec2 my_vel(balls[0].v_x, balls[0].v_z);
	my_vel = my_vel + 0.3f * goFw * cam_dir;
	my_vel = my_vel + 0.3f * goRight * right_dir;

	balls[0].v_x = my_vel.x;
	balls[0].v_z =  my_vel.y;

	balls[0].reset_rotation();

	for (int i = 0; i < balls.size()-1; ++i) {
		for (int j = i+1; j < balls.size(); ++j) {
			if (checkCollision(balls[i], balls[j])) {
				std::cerr << "collision "<<i<<" "<<j<< " \n";
				collide(balls[i], balls[j]);
			}
		}
	}
	for (int i = 0; i < balls.size(); ++i) {
	/*	if (balls[i].x - balls[i].r <= -30.0 || balls[i].x + balls[i].r >= 30.0) balls[i].v_x *= -1;
		if (balls[i].z - balls[i].r <= -30.0 || balls[i].z + balls[i].r >= 30.0) balls[i].v_z *= -1;*/
		if(balls[i].stopped_since <= 6.0)
			balls[i].move(delta_time);
	}
	

	m_overhead_light = glm::vec3(20 * cosf(last_time/1000.0), 10, 20 * sinf(last_time/1000.0));
	//m_overhead_light = glm::vec3(20 , 10, 20 );

	m_camera.Update(delta_time);
	
	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	

	// asztal
	table_vao.Bind();
	table_prog.Use();
	table_prog.SetTexture("texImage", 0, table_tex);
	table_prog.SetUniform("MVP", m_camera.GetViewProj());
	table_prog.SetUniform("world", glm::mat4(1.0f));
	table_prog.SetUniform("worldIT", glm::mat4(1.0f));
	table_prog.SetUniform("overhead_light_pos", m_overhead_light);
	table_prog.SetUniform("camera_pos", m_camera.GetEye());
	table_prog.SetUniform("ball_light_pos", glm::vec3(balls[0].x, balls[0].r, balls[0].z));

	//std::cerr << "cam:" << m_camera.GetEye().x << " " << m_camera.GetEye().z << std::endl;
	glDrawElements(GL_TRIANGLES, 8, GL_UNSIGNED_INT, nullptr);

	renderFence();

	sphere_vao.Bind();
	sphere_prog.Use();

	//for (auto b : balls) renderSphere(b);
	for (int i = 0; i < balls.size(); ++i) renderSphere(balls[i]);
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_w:
		goFw = 1.0f;
		break;
	case SDLK_s:
		goFw = -1.0f;
		break;
	case SDLK_a:
		goRight = -1.0f;
		break;
	case SDLK_d:
		goRight = 1.0f;
		break;
	default:
		break;
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_w:
	case SDLK_s:
		goFw = 0.0f;
		break;
	case SDLK_a:
	case SDLK_d:
		goRight = 0.0f;
		break;
	}
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

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}






void CMyApp::renderFence()
{
	fence_vao.Bind();
	fence_prog.Use();
	fence_prog.SetTexture("texImage", 0, fence_tex);

	fence_prog.SetUniform("overhead_light_pos", m_overhead_light);
	fence_prog.SetUniform("camera_pos", m_camera.GetEye());
	fence_prog.SetUniform("ball_light_pos", glm::vec3(balls[0].x, balls[0].r, balls[0].z));

	glm::mat4 fenceTransform = glm::translate(glm::vec3(0, 2, 31)) * glm::scale(glm::vec3(32, 2, 1));
	fence_prog.SetUniform("MVP", m_camera.GetViewProj() * fenceTransform);
	fence_prog.SetUniform("world", fenceTransform);
	fence_prog.SetUniform("worldIT", glm::inverse(glm::transpose(fenceTransform)));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	fenceTransform = glm::translate(glm::vec3(0, 2, -31)) * glm::scale(glm::vec3(32, 2, 1));
	fence_prog.SetUniform("MVP", m_camera.GetViewProj() * fenceTransform);
	fence_prog.SetUniform("world", fenceTransform);
	fence_prog.SetUniform("worldIT", glm::inverse(glm::transpose(fenceTransform)));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	fenceTransform = glm::translate(glm::vec3(31, 2, 0)) * glm::scale(glm::vec3(1, 2, 30));
	fence_prog.SetUniform("MVP", m_camera.GetViewProj() * fenceTransform);
	fence_prog.SetUniform("world", fenceTransform);
	fence_prog.SetUniform("worldIT", glm::inverse(glm::transpose(fenceTransform)));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	fenceTransform = glm::translate(glm::vec3(-31, 2, 0)) * glm::scale(glm::vec3(1, 2, 30));
	fence_prog.SetUniform("MVP", m_camera.GetViewProj() * fenceTransform);
	fence_prog.SetUniform("world", fenceTransform);
	fence_prog.SetUniform("worldIT", glm::inverse(glm::transpose(fenceTransform)));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
}

void CMyApp::initFence()
{

	fence_prog.AttachShaders({
		{GL_VERTEX_SHADER, "fenceVert.vert"},
		{GL_FRAGMENT_SHADER, "fenceFrag.frag"}
		});

	fence_prog.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_normal" },
		{ 2, "vs_in_tex0"}
		});

	fence_prog.LinkProgram();


	// cube, needs to be transformed
	fence_pos.BufferData(
		std::vector< glm::vec3 > {

		// front
		glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(-1.0f, 1.0f, 1.0f),
			glm::vec3(-1.0f, -1.0f, 1.0f),
			glm::vec3(1.0f, -1.0f, 1.0f),

			// back
			glm::vec3(1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),

			// left
			glm::vec3(-1.0f, 1.0f, 1.0f),
			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, 1.0f),

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
		0, 1, 2,
			0, 2, 3,

			6, 5, 4,
			7, 6, 4,

			8, 9, 10,
			10, 11, 8,

			14, 13, 12,
			14, 12, 15,

			16, 17, 18,
			18, 19, 16,

			22, 21, 20,
			22, 20, 23
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
			{ CreateAttribute  <0, glm::vec3, 0, sizeof(glm::vec3)>, fence_pos },
		{ CreateAttribute  <1, glm::vec3, 0, sizeof(glm::vec3)>, fence_norm },
		{ CreateAttribute  <2, glm::vec2, 0, sizeof(glm::vec2)>, fence_bufftex }
		},
		fence_indices
	);
}

void CMyApp::initTable()
{
	table_prog.AttachShaders({
	{ GL_VERTEX_SHADER, "myVert.vert"},
	{ GL_FRAGMENT_SHADER, "myFrag.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	table_prog.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_normal" },
		{ 2, "vs_in_tex0"}
		});


	table_prog.LinkProgram();

	table_pos.BufferData(
		std::vector<glm::vec3>{
		// h�ts� lap
		glm::vec3(30, 0, 30),
			glm::vec3(-30, 0, 30),
			glm::vec3(-30, 0, -30),
			glm::vec3(30, 0, -30)

	}
	);

	table_tex.FromFile("tex_table.jpg");

	// �s a primit�veket alkot� cs�cspontok indexei (az el�z� t�mb�kb�l) - triangle list-el val� kirajzol�sra felk�sz�lve
	table_indices.BufferData(
		std::vector<int>{
		0, 2, 1,
			0, 3, 2
	}
	);

	table_bufftex.BufferData(
		std::vector<glm::vec2>{
		glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0)
	}
	);

	table_norm.BufferData(
		std::vector<glm::vec3>{
		glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0)
	}
	);


	// geometria VAO-ban val� regisztr�l�sa
	table_vao.Init(
		{
			{ CreateAttribute  <0, glm::vec3, 0, sizeof(glm::vec3)>, table_pos },
		{ CreateAttribute  <1, glm::vec3, 0, sizeof(glm::vec3)>, table_norm },	// 1-es attrib�tum "l�nyeg�ben" glm::vec3-ak sorozata �s az adatok az m_gpuBufferNormal GPU pufferben vannak
		{ CreateAttribute  <2, glm::vec2, 0, sizeof(glm::vec2)>, table_bufftex }

		},
		table_indices
	);
}

void CMyApp::initSphere()
{
	sphere_prog.AttachShaders({
	{ GL_VERTEX_SHADER, "sphereVert.vert"},
	{ GL_FRAGMENT_SHADER, "sphereFrag.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	sphere_prog.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_tex0"}
		});


	sphere_prog.LinkProgram();

	std::vector<glm::vec3> pos;
	std::vector<glm::vec2> tx;
	for (int j = 0; j <= 20; ++j) {
		for (int i = 0; i <= 20; ++i) {
			float u = i / 20.0;
			float v = j / 20.0;
			pos.push_back(sphere_getUV(u, v));
			tx.push_back(glm::vec2(u, v));
		}
	}

	std::vector<GLuint> indices;
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 20; ++j)
		{
			indices.push_back((i)+(j)   * (21));
			indices.push_back((i + 1) + (j)   * (21));
			indices.push_back((i)+(j + 1) * (21));
			indices.push_back( (i + 1) + (j)   * (21));
			indices.push_back( (i + 1) + (j + 1) * (21));
			indices.push_back( (i)+(j + 1) * (21));
		}

	sphere_pos.BufferData(pos);
	sphere_indices.BufferData(indices);
	sphere_bufftex.BufferData(tx);
	sphere_tex1.FromFile("tex_beachball.jpg");
	sphere_tex2.FromFile("tex_football.jpg");
	sphere_tex3.FromFile("tex_ball12.jpg");

	sphere_vao.Init(
		{
		{ CreateAttribute  <0, glm::vec3, 0, sizeof(glm::vec3)>, sphere_pos },
		{ CreateAttribute  <1, glm::vec2, 0, sizeof(glm::vec2)>, sphere_bufftex}

		},
		sphere_indices
	);

}


glm::vec3	CMyApp::sphere_getUV(float u, float v)
{
	u *= 2 * 3.1415f;
	v *= 3.1415f;
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	float r = 1;
	return glm::vec3(r*cu*sv, r*cv, r*su*sv);
}

void CMyApp::renderSphere(Sphere ball)
{
	//std::cerr << "rendering ball " << ball.r << std::endl;
	glm::mat4 sphereTrans;
	if (!ball.alive) {
		if (ball.stopped_since > 6.0) return;

		sphereTrans = glm::translate(glm::vec3(ball.x, ball.r * (5.5 -ball.stopped_since ) * 2, ball.z))
			* ball.rot_matrix()
			* glm::scale(glm::vec3(ball.r, ball.r, ball.r));
	}
	else
	{
		sphereTrans = glm::translate(glm::vec3(ball.x, ball.r, ball.z))
			* ball.rot_matrix()
			* glm::scale(glm::vec3(ball.r, ball.r, ball.r));

	}

	sphere_prog.SetUniform("MVP", m_camera.GetViewProj()*sphereTrans);
	sphere_prog.SetUniform("world", sphereTrans);
	sphere_prog.SetUniform("worldIT", glm::inverse(glm::transpose(sphereTrans)));
	sphere_prog.SetUniform("overhead_light_pos", m_overhead_light);
	sphere_prog.SetUniform("camera_pos", m_camera.GetEye());
	sphere_prog.SetUniform("ball_light_pos", glm::vec3(balls[0].x, balls[0].r, balls[0].z));


	sphere_prog.SetUniform("is_gold", int(ball.is_gold));

	switch (ball.material)
	{
	case 1: sphere_prog.SetTexture("texImage", 0, sphere_tex1); break;
	case 2: sphere_prog.SetTexture("texImage", 0, sphere_tex2); break;
	case 3: sphere_prog.SetTexture("texImage", 0, sphere_tex3); break;
	default:
		break;
	}
	glDrawElements(GL_TRIANGLES, 6 * 20 * 20, GL_UNSIGNED_INT, nullptr);
}

void CMyApp::randomBalls(int n)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis_pos(-27.0, 27.0);
	std::uniform_real_distribution<> dis_r(1.0, 3.0);
	std::uniform_real_distribution<> dis_v(-10.0, 10.0);
	std::uniform_int_distribution<>  dis_mat(1, 3);
	bool good = false;
	Sphere s;
	for (int i = 0; i < n; ++i) {
		good = false;
		while (!good) {
			good = true;
			s = Sphere(dis_pos(gen), dis_pos(gen), dis_r(gen), dis_v(gen), dis_v(gen), dis_mat(gen) );
			for (auto other : balls) if (checkCollision(s, other)) good = false;
		}
		balls.push_back(s);
	}
}

