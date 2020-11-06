#version 460 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos; 
//in vec2 TexCoord;
//uniform sampler2D texture1;
//uniform vec3 objectColor;
//uniform vec3 lampColor;
uniform vec3 viewPos;

struct Material {
	//цвет поверхности при фоновом освещении
    vec3 ambient;
	// цвет поверхности при рассеянном освещении
    vec3 diffuse;
	//цвет зеркального блика
    vec3 specular;
	//рассеивание/радиус зеркального блика
    float shininess;
}; 
uniform Material material;

struct Light {
    vec3 position;
	//Интенсивность/цвет Фонового освещения
    vec3 ambient;
	//Интенсивность/цвет рассеивания
    vec3 diffuse;
	//Интенсивность/цвет отраженная
    vec3 specular;
};
 
uniform Light light;

void main()
{
	//Интенсивность Фонового освещения
	//float ambientStrength = 0.1;
	// Фоновая состовляющая
    vec3 ambient = light.ambient * material.ambient;

	vec3 norm = normalize(Normal);
	//Направление света
	vec3 lightDir = normalize(light.position - FragPos);
	//Угол падения света к поверъности в радианах(коэффициент)
	float diff = max(dot(norm, lightDir), 0.0);
	//получаем силу освещения к точке на поверхности
	vec3 diffuse = light.diffuse * (material.diffuse * diff);

	// Составляющая отражения
	//Интенсивность отражения
	//float specularStrength = 0.5;
	//Направление взгляда
	vec3 viewDir = normalize(viewPos - FragPos);
	//вектор отражения вдоль нормальной оси
	vec3 reflectDir = reflect(-lightDir, norm);
	//Угол зеркального отражения(отраженная составляющая), 32 степень - это значение блеска свечения
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//vec3 specular = specularStrength * spec * lampColor;
	vec3 specular = (spec * material.specular) * light.specular; 
 
    FragColor = vec4(ambient + diffuse + specular, 1.0);

    //FragColor = vec4(objectColor * (ambient + diffuse + specular), 1.0);
	//FragColor = vec4(lampColor * objectColor, 1.0);

	//FragColor = texture(texture1, TexCoord);
}
