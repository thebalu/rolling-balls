#version 330 core

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;

out vec4 fs_out_col;

uniform samplerCube skyboxTexture;

void main()
{
	//fs_out_col = vec4( normalize(vs_out_pos), 1);
	fs_out_col = texture( skyboxTexture, (vs_out_pos) );

	/*
	// procedur�lis 1:
	fs_out_col = vec4( vs_out_pos, 1);

	// procedur�lis 2:
	const vec4 groundColor = vec4(0.5, 0.2, 0.2, 1);
	const vec4 skyColor = vec4(0.2, 0.3, 0.7, 1);

	fs_out_col = mix( groundColor, skyColor, (1 + vs_out_pos.y)/2 );
	*/
}