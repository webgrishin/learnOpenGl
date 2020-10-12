#version 460 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//FragColor = mix(texture(texture1, vec2(-TexCoord.x, TexCoord.y)),texture(texture2, TexCoord), 0.8) ;1
	FragColor = texture(texture1, TexCoord)* vec4(ourColor, 1.0);
    //FragColor = texture(texture1, vec2(-TexCoord.x, TexCoord.y));
	//FragColor = texture(texture1, TexCoord);
}
