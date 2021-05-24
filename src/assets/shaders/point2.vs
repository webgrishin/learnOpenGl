#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in float aStartTime;

uniform float Time;
uniform float ParticleLifetime;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

/*
out VS_OUT {
    vec4 fragColor;
} vs_out;
*/
out vec4 fragColor;

void main()
{
	fragColor = aColor;
	if( Time > aStartTime ) {
		float age = Time - aStartTime;
		if( age < ParticleLifetime ) {
			fragColor.a = 1.0 - age / ParticleLifetime;
		}
	}
	//vs_out.fragColor = aColor;
	gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
}