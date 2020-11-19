#version 460 core
out vec4 FragColor;
struct Material {
	//цвет фона и рассеянного света
	sampler2D diffuse;
	//цвет зеркального блика
    sampler2D specular;
	//цвет зеркального блика
    sampler2D emission;
	//рассеивание/радиус зеркального блика
    float shininess;
}; 
struct Light {
    // vec3 position;
	//Интенсивность/цвет Фонового освещения
    vec3 ambient;
	//Интенсивность/цвет рассеивания
    vec3 diffuse;
	//Интенсивность/цвет отраженная
    vec3 specular;

	//Параметры затухания света
	float constant;
    float linear;
    float quadratic;
	
	//Параметры фонарика
	//Положение лампы
	vec3 position;
	//направление лампы
    vec3 direction;
	//Угол отсечки
    float cutOff;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

vec3 calculate_emission();
void lighting(vec3 lightDir);

void main()
{
	//Направление света
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
    
if(theta > light.cutOff) 
{       
  // Выполняем вычисления освещения
  lighting(lightDir);
}
else // в противном случае, используем ambient-свет, чтобы вне прожектора сцена не была польностью темной
  FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);


}
void lighting(vec3 lightDir){
	vec3 color = texture(material.diffuse, TexCoords).rgb;
	// Фоновая состовляющая
	vec3 ambient = light.ambient * color;

	//Рассеяный свет
	vec3 norm = normalize(Normal);
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
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	//Затухание
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
	light.quadratic * (distance * distance));

    // FragColor = vec4(ambient*attenuation + diffuse*attenuation + specular*attenuation + calculate_emission(), 1.0);
    FragColor = vec4((ambient + diffuse + specular)*attenuation + calculate_emission(), 1.0);

}

vec3 calculate_emission()
{
	vec3 show = step(vec3(1.0), vec3(1.0) - texture(material.specular, TexCoords).rgb);
	return texture(material.emission, TexCoords).rgb * show;
}
