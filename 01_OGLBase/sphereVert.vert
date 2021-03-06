#version 330 core

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
//in vec3 vs_in_norm;
in vec2 vs_in_tex0;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex0;

// shader k�ls� param�terei - most a h�rom transzform�ci�s m�trixot k�l�n-k�l�n vessz�k �t
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;
void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );
	vs_out_pos  = (world * vec4( vs_in_pos, 1 )).xyz;
	//vs_out_norm = (worldIT * vec4( vs_in_norm, 0 )).xyz;
	//vs_out_norm = (worldIT*vec4(vs_out_pos,1)).xyz;
	vs_out_norm =(world * vec4( vs_in_pos, 0 )).xyz;
	vs_out_tex0 = vs_in_tex0;
}