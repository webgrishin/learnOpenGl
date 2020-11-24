#version 460 core
out vec4 FragColor;

struct Material {
	//���� ���� � ����������� �����
	sampler2D diffuse;
	//���� ����������� �����
    sampler2D specular;
	//�����������/������ ����������� �����
    float shininess;
}; 

struct DirLight {
	//������ ����������� ��������� �����
    vec3 direction;
	
	//�������������/���� �������� ���������
    vec3 ambient;
	//�������������/���� �����������
    vec3 diffuse;
	//�������������/���� ����������
    vec3 specular;
};

struct PointLight {
	//��������� �����
    vec3 position;
    
	//��������� ��������� �����
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
	//��������� ��������
    vec3 position;
	//�����������, ����� �������� ��������� ���������.
    vec3 direction;
	//���� �������
    float cutOff;
	//���� �������� �������, ��� ���������
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

//��������������� �������
	//�������+����������+���������� ����
vec3 _calcLight(DirLight light, vec3 normal, vec3 viewDir);
    //���������
float _calcAttenuation(AttenuationLight light, vec3 fragPos);
// ��������� �������
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
	//����������� �������
	vec3 viewDir = normalize(viewPos - FragPos);

    // ���� 1: ������������ ���������
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: �������� ��������� �����
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // ���� 3: ���������
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
}

vec3 _calcLight(DirLight light, vec3 normal, vec3 viewDir){
    // diffuse shading (��������� ���������). ���� ������� ����� � ����������� � ��������(�����������)
    float diff = max(dot(normal, light.direction), 0.0);
    // specular shading (���������� ���������). ������ ��������� ����� ���������� ���
    vec3 reflectDir = reflect(-light.direction, normal);
	//���� ����������� ���������(���������� ������������), shininess ������� - ��� �������� ������ ��������
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // ��������� ����������
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

float _calcAttenuation(AttenuationLight light, vec3 fragPos){
    float distance = length(light.position - fragPos);
    return 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
}

// ��������� ���� ��� ������������� ������������� �����.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//����������� �����
    light.direction = normalize(-light.direction);
	return _calcLight(light, normal, viewDir);
}

// ��������� ���� ��� ������������� ��������� ��������� �����.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//����������� �����
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

// ��������� ���� ��� ������������� ����������.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//������, ����������� ����������� �� ��������� � ��������� �����.
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

    // ������������� ���������� (������ ����)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    return color * attenuation * intensity;
}
