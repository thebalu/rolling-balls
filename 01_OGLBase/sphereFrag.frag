#version 330 core

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;
in vec2 vs_out_tex0;
out vec4 fs_out_col;
uniform sampler2D texImage;

void main()
{
	fs_out_col = vec4(vs_out_pos,1);
}