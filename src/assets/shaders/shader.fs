#version 460 core
out vec4 FragColor;

struct Material {
	//цвет фона и рассеянного света
	sampler2D diffuse;
	//цвет зеркального блика
    sampler2D specular;
	//рассеивание/радиус зеркального блика
    float shininess;
}; 

struct DirLight {
	//вектор направления источника света
    vec3 direction;
	
	//Интенсивность/цвет Фонового освещения
    vec3 ambient;
	//Интенсивность/цвет рассеивания
    vec3 diffuse;
	//Интенсивность/цвет отраженная
    vec3 specular;
};

struct PointLight {
	//Положение лампы
    vec3 position;
    
	//Параметры затухания света
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
	//Параметры фонарика
    vec3 position;
	//направление, вдоль которого направлен прожектор.
    vec3 direction;
	//Угол отсечки
    float cutOff;
	//Угол внешнего радиуса, для затухания
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

struct AttenuationLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 4

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

//Вспомогательные функции
	//Фоновый+рассеянный+отраженный свет
vec3 _calcLight(DirLight light, vec3 normal, vec3 viewDir);
    //Затухание
float _calcAttenuation(AttenuationLight light, vec3 fragPos);
// Прототипы функций
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
	//Направление взгляда
	vec3 viewDir = normalize(viewPos - FragPos);

    // фаза 1: направленное освещение
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: точечные источники света
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // фаза 3: прожектор
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
}

vec3 _calcLight(DirLight light, vec3 normal, vec3 viewDir){
    // diffuse shading (диффузное затенение). Угол падения света к поверхности в радианах(коэффициент)
    float diff = max(dot(normal, light.direction), 0.0);
    // specular shading (отраженное затенение). Вектор отражения вдоль нормальной оси
    vec3 reflectDir = reflect(-light.direction, normal);
	//Угол зеркального отражения(отраженная составляющая), shininess степень - это значение блеска свечения
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // совмещаем результаты
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

float _calcAttenuation(AttenuationLight light, vec3 fragPos){
    float distance = length(light.position - fragPos);
    return 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
}

// вычисляем цвет при использовании направленного света.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//Направление света
    light.direction = normalize(-light.direction);
	return _calcLight(light, normal, viewDir);
}

// вычисляем цвет при использовании точечного источника света.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//Направление света
    vec3 lightDir = normalize(light.position - fragPos);

	DirLight dirLight;
	dirLight.direction = lightDir;
	dirLight.ambient = light.ambient;
	dirLight.diffuse = light.diffuse;
	dirLight.specular = light.specular;

    vec3 color = _calcLight(dirLight, normal, viewDir);

	AttenuationLight attLight;
	attLight.position = light.position;
	attLight.constant = light.constant;
	attLight.linear = light.linear;
	attLight.quadratic = light.quadratic;

	float attenuation = _calcAttenuation(attLight, fragPos);

    return color * attenuation;
}

// вычисляем цвет при использовании прожектора.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//вектор, указывающий направление от фрагмента к источнику света.
    vec3 lightDir = normalize(light.position - fragPos);

	DirLight dirLight;
	dirLight.direction = lightDir;
	dirLight.ambient = light.ambient;
	dirLight.diffuse = light.diffuse;
	dirLight.specular = light.specular;

    vec3 color = _calcLight(dirLight, normal, viewDir);

	AttenuationLight attLight;
	attLight.position = light.position;
	attLight.constant = light.constant;
	attLight.linear = light.linear;
	attLight.quadratic = light.quadratic;

	float attenuation = _calcAttenuation(attLight, fragPos);

    // интенсивность прожектора (мягкие края)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    return color * attenuation * intensity;
}
