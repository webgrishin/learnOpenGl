#ifndef LIGHTING_H
#define LIGHTING_H

#include <glm/glm.hpp>
#include <glad/glad.h> // содержит все объявления OpenGL-типов

//#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ShaderProgram.h"
#include "Camera.h"


using namespace glm;

// направленный свет
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//Фонарик
struct SpotLight {
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

//Точечный свет
struct KVvec3 {
    string key;
    vec3 value;
};

struct KVfloat {
    string key;
    float value;
};

struct PointLight {
	//Положение лампы
    KVvec3 position;
    
	//Параметры затухания света
    KVfloat constant;
    KVfloat linear;
    KVfloat quadratic;
	
    KVvec3 ambient;
    KVvec3 diffuse;
    KVvec3 specular;
};
/* struct Color {
    glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
}; */
#define NR_POINT_LIGHTS 4

namespace RenderEngine
{
    class Lighting
    {
    public:
        DirLight dirLight;
        SpotLight spotLight;
        GLfloat shininess = 32.0f;
        PointLight pointsLight[NR_POINT_LIGHTS];

        Lighting()
        {
            // координаты точечных источников света
            glm::vec3 pointLightPositions[NR_POINT_LIGHTS] = {
                glm::vec3(0.7f, 0.2f, 2.0f),
                glm::vec3(2.3f, -3.3f, -4.0f),
                glm::vec3(-4.0f, 2.0f, -12.0f),
                glm::vec3(0.0f, 0.0f, -3.0f)
            };

            glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
            glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
            glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
            glm::vec3 pointLightColors[NR_POINT_LIGHTS] = {red, green, blue, yellow};

            vec3 ambient = vec3(0.05f);
            vec3 specular = vec3(1.0f);
            float constant = 1.0f;
            float linear = 0.09f;
            float qaudratic = 0.032f;
            // точечные источники света
            for (unsigned int i = 0; i < NR_POINT_LIGHTS; i++)
            {
                string ni = to_string(i);
                pointsLight[i].position.key = "pointLights[" + ni + "].position";
                pointsLight[i].position.value = pointLightPositions[i];
                pointsLight[i].diffuse.key = "pointLights[" + ni + "].diffuse";
                pointsLight[i].diffuse.value = pointLightColors[i];
                pointsLight[i].ambient.key = "pointLights[" + ni + "].ambient";
                pointsLight[i].ambient.value = ambient;
                pointsLight[i].specular.key = "pointLights[" + ni + "].specular";
                pointsLight[i].specular.value = specular;
                pointsLight[i].constant.key = "pointLights[" + ni + "].constant";
                pointsLight[i].constant.value = constant;
                pointsLight[i].linear.key = "pointLights[" + ni + "].linear";
                pointsLight[i].linear.value = linear;
                pointsLight[i].quadratic.key = "pointLights[" + ni + "].quadratic";
                pointsLight[i].quadratic.value = qaudratic;
            }

            dirLight.direction = vec3(-0.2f, -1.0f, -0.3f);
            dirLight.ambient = ambient;
            dirLight.diffuse = vec3(0.8f, 0.8f, 0.8f);
            dirLight.specular = specular;

            spotLight.ambient = vec3(0.0f);
            spotLight.diffuse = vec3(1.0f);
            spotLight.specular = specular;
            spotLight.constant = constant;
            spotLight.linear = linear;
            spotLight.quadratic = qaudratic;
            spotLight.cutOff = cos(glm::radians(12.5f));
            spotLight.outerCutOff = cos(glm::radians(15.0f));
        }
        // точечные источники света
        void OnPointsLight(RenderEngine::ShaderProgram &shader)
        {
            for (int i = 0; i < NR_POINT_LIGHTS; i++)
            {
                shader.setVec3(pointsLight[i].position.key, pointsLight[i].position.value);
                shader.setVec3(pointsLight[i].ambient.key, pointsLight[i].ambient.value);
                shader.setVec3(pointsLight[i].diffuse.key, pointsLight[i].diffuse.value);
                shader.setVec3(pointsLight[i].specular.key, pointsLight[i].specular.value);
                shader.setFloat(pointsLight[i].constant.key, pointsLight[i].constant.value);
                shader.setFloat(pointsLight[i].linear.key, pointsLight[i].linear.value);
                shader.setFloat(pointsLight[i].quadratic.key, pointsLight[i].quadratic.value);
            }
        }
        // направленный свет
        void OnDirLight(RenderEngine::ShaderProgram &shader)
        {
            // свойства материалов
            shader.setFloat("material.shininess", shininess);
            shader.setVec3("dirLight.direction", dirLight.direction);
            // ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
            // ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
            // ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
            shader.setVec3("dirLight.ambient", dirLight.ambient);
            shader.setVec3("dirLight.diffuse", dirLight.diffuse);
            shader.setVec3("dirLight.specular", dirLight.specular);
        }
        // прожектор (фонарик)
        void OnSpotLight(RenderEngine::ShaderProgram &shader, RenderEngine::Camera &camera)
        {
            shader.setVec3("spotLight.position", camera.Position);
            shader.setVec3("spotLight.direction", camera.Front);
            shader.setVec3("spotLight.ambient", spotLight.ambient);
            shader.setVec3("spotLight.diffuse", spotLight.diffuse);
            shader.setVec3("spotLight.specular", spotLight.specular);
            shader.setFloat("spotLight.constant", spotLight.constant);
            shader.setFloat("spotLight.linear", spotLight.linear);
            shader.setFloat("spotLight.quadratic", spotLight.quadratic);
            shader.setFloat("spotLight.cutOff", spotLight.cutOff);
            shader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
        }

        ~Lighting()
        {
        }
    };
} // namespace RenderEngine

#endif //LIGHTING_H
