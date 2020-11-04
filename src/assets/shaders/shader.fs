#version 460 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos; 
//in vec2 TexCoord;
//uniform sampler2D texture1;
uniform vec3 objectColor;
uniform vec3 lampColor;
uniform vec3 lampPos;
uniform vec3 viewPos;

void main()
{
	//������� � �����������
	vec3 norm = normalize(Normal);
	//����������� �����
	vec3 lampDir = normalize(lampPos - FragPos);
	//���� ������� ����� � ����������� � ��������(�����������)
	float diff = max(dot(norm, lampDir), 0.0);
	//�������� ���� ��������� � ����� �� �����������
	vec3 diffuse = diff * lampColor;

	//������������� �������� ���������
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lampColor;

	//������������� ���������
	float specularStrength = 0.5;
	//����������� �������
	vec3 viewDir = normalize(viewPos - FragPos);
	//������ ��������� ����� ���������� ���
	vec3 reflectDir = reflect(-lampDir, norm);
	//���� ����������� ���������(���������� ������������), 32 ������� - ��� �������� ������ ��������
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lampColor;  
 
    FragColor = vec4(objectColor * (ambient + diffuse + specular), 1.0);
	//FragColor = vec4(lampColor * objectColor, 1.0);

	//FragColor = texture(texture1, TexCoord);
}
