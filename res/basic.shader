#shader vertex
#version 330 core

layout(location = 0) in vec3 l_pos;
layout(location = 1) in vec4 l_col;

uniform vec2 u_resolution;

out vec4 color;

void main()
{
	vec3 pos = l_pos;
	float aspect = u_resolution.y / u_resolution.x;
	pos.x *= aspect;
	gl_Position = vec4(pos, 1.0f);
	color = l_col;
}

#shader fragment
#version 330 core

in vec4 color;

out vec4 o_color;

void main()
{
	o_color = color;
}