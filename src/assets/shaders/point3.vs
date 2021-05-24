#version 460 core

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

layout (location = 0) in vec2 aPos;
layout (location = 1) in float aStartTime;
//layout (location = 2) in vec2 aIndexPosition;

layout( xfb_buffer = 0, xfb_offset=0 ) out vec2 Position;   // To transform feedback
layout( xfb_buffer = 1, xfb_offset=0 ) out float StartTime; // To transform feedback
//layout( xfb_buffer = 2, xfb_offset=0 ) out uint IndexPosition; // To transform feedback

uniform float scaleX;
uniform float scaleY;
uniform float Time;
uniform float dT;     // Elapsed time between frames
uniform float ParticleLifetime;

uniform mat4 MVP;

out vec4 fragColor;

subroutine (RenderPassType)
void update() {

    // Update attr for next frame
    Position = aPos;
    StartTime = aStartTime;
	//IndexPosition = aIndexPosition;

    if( Time >= StartTime ) {

        float age = Time - StartTime;

        if( age > ParticleLifetime ) {
            // The particle is past it's lifetime, recycle.
            Position = vec2(0.0);
            StartTime = Time;
        } else {
            // The particle is alive, update.
			Position.y = Position.y + dT;
			//Position.x = Position.x + dT;
			Position.x = Position.x + sin(age * scaleX) * exp(-age*2) * scaleY;
        }
    }
}

subroutine (RenderPassType)
void render() {
    float age = Time - aStartTime;
	fragColor = vec4(1.0, 1.0, 0.0, 1.0);
	fragColor.a = 1.0 - age / ParticleLifetime;
	gl_Position = MVP * vec4(aPos, 0.0, 1.0);
}

void main()
{
	RenderPass();
}