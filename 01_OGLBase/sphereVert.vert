#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos;
//in vec3 vs_in_norm;
in vec2 vs_in_tex0;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
//out vec3 vs_out_norm;
out vec2 vs_out_tex0;

// shader külsõ paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	vs_out_pos = vs_in_pos;
	//vs_out_norm = vs_in_norm;
	vs_out_tex0 = vs_in_tex0;
}