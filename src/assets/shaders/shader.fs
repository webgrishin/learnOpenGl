#version 460 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos; 
//in vec2 TexCoord;
//uniform sampler2D texture1;
uniform vec3 objectColor;
uniform vec3 lampColor;
uniform vec3 lampPos;

void main()
{
	//Нормаль к поверхности
	vec3 norm = normalize(Normal);
	//Направление света
	vec3 lampDir = normalize(lampPos - FragPos);
	//Угол падения света к поверъности в радианах(коэффициент)
	float diff = max(dot(norm, lampDir), 0.0);
	//получаем силу освещения к точке на поверхности
	vec3 diffuse = diff * lampColor;

	//Фоновое освещение
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lampColor;
 
    FragColor = vec4(objectColor * (ambient + diffuse), 1.0);
	//FragColor = vec4(lampColor * objectColor, 1.0);

	//FragColor = texture(texture1, TexCoord);
}
