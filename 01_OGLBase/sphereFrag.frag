#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec2 vs_out_tex0;
out vec4 fs_out_col;
uniform sampler2D texImage;

void main()
{
	fs_out_col = vec4(vs_out_pos,1);
}