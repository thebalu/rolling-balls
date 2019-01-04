#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex0;

out vec4 fs_out_col;
uniform sampler2D texImage;

// light properties
uniform vec3 overhead_light_pos;
uniform vec3 ball_light_pos;

uniform vec3 La = vec3(0.3, 0.4, 0.9);
uniform vec3 Ld = vec3(0.3, 0.8, 0.2);
uniform vec3 Ls = vec3(1.0, 0.2, 0.1);

uniform vec3 LBd = vec3(1.0, 0.843, 0.0);
uniform vec3 LBs = vec3(1.0, 0.843, 0.0);

uniform vec3 Ka = vec3(0.5, 0.5, 0.5);
uniform vec3 Kd = vec3(0.7, 0.7, 0.7);
uniform vec3 Ks = vec3(0.7, 0.7, 0.7);

uniform mat4 MVP;
//uniform mat4 world;
uniform vec3 camera_pos;

uniform int is_gold = 0;


void main()
{
	vec3 to_light = normalize(overhead_light_pos - vs_out_pos);
	vec3 normal = normalize(vs_out_norm);
	float cosa  = clamp(dot(normal, to_light), 0, 1);

	
	vec3 ball_to_light = normalize(ball_light_pos - vs_out_pos);
	float ball_cosa  = clamp(dot(normal, ball_to_light), 0, 1);
	vec3 ball_reflectDir = reflect(-ball_to_light, normal);


	vec3 viewDir = normalize(camera_pos - vs_out_pos);
	vec3 reflectDir = reflect(-to_light, normal);

	float sp = pow( clamp(dot(viewDir,reflectDir),0.0,1.0), 6);
	float ball_sp = pow( clamp(dot(viewDir,ball_reflectDir),0.0,1.0), 32);


	vec3 ambient = La ;
	vec3 diffuse = (cosa * Ld * Kd + ball_cosa * LBd * Kd)*0.5;
	vec3 specular = (sp * Ls * Ks + ball_sp * LBs * Ks)*0.5;
	

	fs_out_col = vec4 (texture(texImage, vs_out_tex0.st) * (ambient + diffuse + specular), 1);
	if(is_gold == 1) {
		vec4 gold_ball = texture(texImage, vs_out_tex0.st) * vec3(1.0, 0.843, 0.0);
		fs_out_col = gold_ball; //vec4(gold_ball  ,1);
	}
	//fs_out_col = vec4 (cosa, 0, -,1);
}