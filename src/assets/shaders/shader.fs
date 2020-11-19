#version 460 core
out vec4 FragColor;
struct Material {
	//���� ���� � ����������� �����
	sampler2D diffuse;
	//���� ����������� �����
    sampler2D specular;
	//���� ����������� �����
    sampler2D emission;
	//�����������/������ ����������� �����
    float shininess;
}; 
struct Light {
    // vec3 position;
	//�������������/���� �������� ���������
    vec3 ambient;
	//�������������/���� �����������
    vec3 diffuse;
	//�������������/���� ����������
    vec3 specular;

	//��������� ��������� �����
	float constant;
    float linear;
    float quadratic;
	
	//��������� ��������
	//��������� �����
	vec3 position;
	//����������� �����
    vec3 direction;
	//���� �������
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
	//����������� �����
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
    
if(theta > light.cutOff) 
{       
  // ��������� ���������� ���������
  lighting(lightDir);
}
else // � ��������� ������, ���������� ambient-����, ����� ��� ���������� ����� �� ���� ���������� ������
  FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);


}
void lighting(vec3 lightDir){
	vec3 color = texture(material.diffuse, TexCoords).rgb;
	// ������� ������������
	vec3 ambient = light.ambient * color;

	//��������� ����
	vec3 norm = normalize(Normal);
	//���� ������� ����� � ����������� � ��������(�����������)
	float diff = max(dot(norm, lightDir), 0.0);
	//�������� ���� ��������� � ����� �� �����������
	vec3 diffuse = light.diffuse * diff * color;

	// ������������ ���������
	//����������� �������
	vec3 viewDir = normalize(viewPos - FragPos);
	//������ ��������� ����� ���������� ���
	vec3 reflectDir = reflect(-lightDir, norm);
	//���� ����������� ���������(���������� ������������), 32 ������� - ��� �������� ������ ��������
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	//���������
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
