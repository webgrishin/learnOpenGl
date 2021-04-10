#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 fragVex;

out vec4 fragColor;

void main()
{
	fragColor = fragVex * aPos.z;
	gl_Position = projection * view * model * vec4(aPos.xy, 0.0, 1.0);
}