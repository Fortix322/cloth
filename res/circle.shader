#shader vertex
#version 330 core

layout(location = 0) in vec3 l_pos;
layout(location = 1) in vec4 l_col;

uniform vec2 u_resolution;

out vec4 color;
out vec2 resolution;

void main()
{
	vec3 pos = l_pos;
	float aspect = u_resolution.y / u_resolution.x;
	//pos.x *= aspect;
	gl_Position = vec4(pos, 1.0f);
	color = l_col;
	resolution = u_resolution;
}

#shader fragment
#version 330 core

in vec4 color;
in vec2 resolution;

out vec4 o_color;

void main()
{
    vec3 circleColor = vec3(0.85, 0.35, 0.2);
    float thickness = 0.5;
    // Control with mouse
    // thickness = iMouse.x / iResolution.x;
    float fade = 0.005;

    // -1 -> 1 local space, adjusted for aspect ratio
    vec2 uv = gl_FragCoord.xy / resolution.xy * 2.0 - 1.0;
    float aspect = resolution.x / resolution.y;
    uv.x *= aspect;

    // Calculate distance and fill circle with white
    float distance = 1.0 - length(uv);
    vec3 color = vec3(smoothstep(0.0, fade, distance));
    color *= vec3(smoothstep(thickness + fade, thickness, distance));

    // Set output color
    o_color = vec4(color, 1.0);
    o_color.rgb *= circleColor;
}