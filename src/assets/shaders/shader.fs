#version 460 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
uniform float ratio;
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1, vec2(1.0-TexCoord.x, TexCoord.y)),texture(texture2, TexCoord), ratio);
	//FragColor = texture(texture1, TexCoord)* vec4(ourColor, 1.0);
    //FragColor = texture(texture1, vec2(-TexCoord.x, TexCoord.y));
	//FragColor = texture(texture1, TexCoord);
}
