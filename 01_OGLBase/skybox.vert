#version 330 core

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;

// shader k�ls� param�terei - most a h�rom transzform�ci�s m�trixot k�l�n-k�l�n vessz�k �t
uniform mat4 MVP;

void main()
{
	gl_Position = (MVP * vec4( vs_in_pos, 1 )).xyww;	// [x,y,w,w] => homog�n oszt�s ut�n [x/w, y/w, 1]

	vs_out_pos = vs_in_pos;
}