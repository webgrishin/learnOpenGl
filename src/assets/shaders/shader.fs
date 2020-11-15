#version 460 core
out vec4 FragColor;
struct Material {
	sampler2D diffuse;
	//цвет зеркального блика
    vec3 specular;
	//рассеивание/радиус зеркального блика
    float shininess;
}; 
struct Light {
    vec3 position;
	//Интенсивность/цвет Фонового освещения
    vec3 ambient;
	//Интенсивность/цвет рассеивания
    vec3 diffuse;
	//Интенсивность/цвет отраженная
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	vec3 color = texture(material.diffuse, TexCoords).rgb;
	// Фоновая состовляющая
	vec3 ambient = light.ambient * color;

	//Рассеяный свет
	vec3 norm = normalize(Normal);
	//Направление света
	vec3 lightDir = normalize(light.position - FragPos);
	//Угол падения света к поверъности в радианах(коэффициент)
	float diff = max(dot(norm, lightDir), 0.0);
	//получаем силу освещения к точке на поверхности
	vec3 diffuse = light.diffuse * diff * color;

	// Составляющая отражения
	//Направление взгляда
	vec3 viewDir = normalize(viewPos - FragPos);
	//вектор отражения вдоль нормальной оси
	vec3 reflectDir = reflect(-lightDir, norm);
	//Угол зеркального отражения(отраженная составляющая), 32 степень - это значение блеска свечения
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular); 
 
    FragColor = vec4(ambient + diffuse + specular, 1.0);

}
