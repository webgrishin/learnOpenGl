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
	//���� ����������� ��� ������� ���������
    vec3 ambient;
	// ���� ����������� ��� ���������� ���������
    vec3 diffuse;
	//���� ����������� �����
    vec3 specular;
	//�����������/������ ����������� �����
    float shininess;
}; 
uniform Material material;

struct Light {
    vec3 position;
	//�������������/���� �������� ���������
    vec3 ambient;
	//�������������/���� �����������
    vec3 diffuse;
	//�������������/���� ����������
    vec3 specular;
};
 
uniform Light light;

void main()
{
	//������������� �������� ���������
	//float ambientStrength = 0.1;
	// ������� ������������
    vec3 ambient = light.ambient * material.ambient;

	vec3 norm = normalize(Normal);
	//����������� �����
	vec3 lightDir = normalize(light.position - FragPos);
	//���� ������� ����� � ����������� � ��������(�����������)
	float diff = max(dot(norm, lightDir), 0.0);
	//�������� ���� ��������� � ����� �� �����������
	vec3 diffuse = light.diffuse * (material.diffuse * diff);

	// ������������ ���������
	//������������� ���������
	//float specularStrength = 0.5;
	//����������� �������
	vec3 viewDir = normalize(viewPos - FragPos);
	//������ ��������� ����� ���������� ���
	vec3 reflectDir = reflect(-lightDir, norm);
	//���� ����������� ���������(���������� ������������), 32 ������� - ��� �������� ������ ��������
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//vec3 specular = specularStrength * spec * lampColor;
	vec3 specular = (spec * material.specular) * light.specular; 
 
    FragColor = vec4(ambient + diffuse + specular, 1.0);

    //FragColor = vec4(objectColor * (ambient + diffuse + specular), 1.0);
	//FragColor = vec4(lampColor * objectColor, 1.0);

	//FragColor = texture(texture1, TexCoord);
}
