#version 460 core
out vec4 FragColor;
//in vec2 TexCoord;
//uniform sampler2D texture1;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	//FragColor = texture(texture1, TexCoord);
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
 
    FragColor = vec4(ambient * objectColor, 1.0);
	//FragColor = vec4(lightColor * objectColor, 1.0);
}
