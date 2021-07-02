#version 460 core

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

layout (location = 0) in vec2 aPos;
layout (location = 1) in float aStartTime;
layout (location = 2) in vec2 aInitPos;
layout (location = 3) in float aParticleLifetime;
layout (location = 4) in float aScaleX;
layout (location = 5) in float aScaleY;
layout (location = 6) in float aInversion;
layout (location = 7) in vec3 aVerteciesCircle;

layout( xfb_buffer = 0, xfb_offset=0 ) out vec2 Position;   // To transform feedback
layout( xfb_buffer = 1, xfb_offset=0 ) out float StartTime; // To transform feedback
//layout( xfb_buffer = 2, xfb_offset=0 ) out vec2 InitPos; // To transform feedback

uniform float Time;
uniform float dT;     // Elapsed time between frames
uniform vec4 bC;
uniform vec4 dC;

uniform mat4 MVP;

out vec4 fragColor;

subroutine (RenderPassType)
void update() {

    // Update attr for next frame
    Position = aPos;
    StartTime = aStartTime;
	//InitPos = aInitPos;

    if( Time >= StartTime ) {

        float age = Time - StartTime;

        if( age > aParticleLifetime ) {
            // The particle is past it's lifetime, recycle.
            Position = aInitPos;
            //Position = vec2(0.5, gl_VertexID/10.0);
            StartTime = Time;
        } else{
        //} else if (age > aParticleLifetime/5.0){
            // The particle is alive, update.
			Position.y = Position.y + 0.7 * dT;
			Position.x = Position.x + sin(age * aScaleX) * exp(-age) * aScaleY * aInversion;
            //Почему не удаётся привести int to float?
        }
    }
}

subroutine (RenderPassType)
void render() {
    float age = Time - aStartTime;
    //if (age > aParticleLifetime/5.0){
	//fragColor = vec4(aPos/3, 0.0, 1.0);
	//fragColor = vec4(1.0, 1.0, 0.0, 1.0);
    //if (aPos.y==0.0)
        //fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    //fragColor = vec4(aInversion);
    fragColor = bC + dC/aParticleLifetime * age;
	//fragColor.a = aVerteciesCircle.z - age / aParticleLifetime;
	fragColor.a = aVerteciesCircle.z * (aParticleLifetime-age) * (1.0 - age / aParticleLifetime);
    //float scale = 1.0 - 0.2*age;
    //vec2 newPoint = aVerteciesCircle.xy * scale;
	//gl_Position = MVP * vec4(aPos+newPoint, 0.0, 1.0);
	gl_Position = MVP * vec4(aPos+aVerteciesCircle.xy, 0.0, 1.0);
    //}
}

void main()
{
	RenderPass();
}