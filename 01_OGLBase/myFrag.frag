#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex0;
out vec4 fs_out_col;
uniform sampler2D texImage;

// light properties
uniform vec3 overhead_light_pos;

uniform vec3 La = vec3(0.1, 0.2, 0.4);
uniform vec3 Ld = vec3(0.1, 0.8, 0.1);
uniform vec3 Ls = vec3(1.0, 0.2, 0.1);
uniform mat4 MVP;
uniform mat4 world;
uniform vec3 camera_pos;
void main()
{
	
	vec4 olp = vec4(overhead_light_pos,1);
	//vec4 olp = vec4(40,5,10,1);

	vec3 to_light = normalize(olp.xyz- vs_out_pos);
	float cosa  = clamp(dot(vs_out_norm, to_light), 0, 1);

	vec3 diffuse = cosa * Ld;

	vec3 viewDir = -normalize(camera_pos - vs_out_pos);
	vec3 reflectDir = reflect(to_light, vs_out_norm);

	float sp = pow( clamp(dot(viewDir,reflectDir),0.0,1.0), 32);
	vec3 specular = sp * Ls;
	

	fs_out_col = vec4 (texture(texImage, vs_out_tex0.st) * (diffuse + La + specular), 1);
	//fs_out_col = vec4 (( diffuse + specular), 1);
//	fs_out_col = vec4 (specular, 1);
	//fs_out_col = vec4(overhead_light_pos,1.0);
}