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

uniform vec3 Ka = vec3(0.5, 0.5, 0.5);
uniform vec3 Kd = vec3(0.8, 0.5, 0.1);
uniform vec3 Ks = vec3(0.9, 0.6, 0);

uniform mat4 MVP;
uniform mat4 world;
uniform vec3 camera_pos;
void main()
{
	
	

	vec3 to_light = normalize(overhead_light_pos - vs_out_pos);
	vec3 normal = normalize(vs_out_norm);
	float cosa  = clamp(dot(normal, to_light), 0, 1);


	vec3 viewDir = normalize(camera_pos - vs_out_pos);
	vec3 reflectDir = reflect(-to_light, vs_out_norm);

	float sp = pow( clamp(dot(viewDir,reflectDir),0.0,1.0), 32);
	
	vec3 ambient = La * Ka;
	vec3 diffuse = cosa * Ld * Kd;
	vec3 specular = sp * Ls * Ks;
	

	fs_out_col = vec4 (texture(texImage, vs_out_tex0.st) * (ambient + diffuse + specular), 1);

}