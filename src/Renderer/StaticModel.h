#ifndef STATIC_H
#define STATIC_H

#include <glm/glm.hpp>
#include <glad/glad.h> // содержит все объявления OpenGL-типов

//#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"


using namespace glm;

// направленный свет
struct DirLight {
    vec3 position;
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
/* struct Desc {
    vec3 color;
    vec3 position;
}; */
/* enum NameColors{
    red, green, blue, yellow
}; */
/* struct PointsLight{
    Desc red = {
        vec3 (1.0f, 0.0f, 0.0f),
        vec3(0.7f, 0.2f, 2.0f)
    };
    Desc green = {
        vec3 (0.0f, 1.0f, 0.0f),
        vec3(2.3f, -0.3f, -0.5f)
    };
    Desc blue = {
        vec3 (0.0f, 0.0f, 1.0f),
        vec3(-1.0f, 2.0f, -1.0f)
    };
    Desc yellow = {
        vec3 (0.0f, 1.0f, 1.0f),
        vec3(0.0f, 0.0f, -1.0f)
    };
}; */

/* vec3 colors[NR_POINT_LIGHTS] = {
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 0.0f)
};

vec3 points[NR_POINT_LIGHTS] = {
                glm::vec3(0.0f, 1.0f, 2.0f),
                glm::vec3(-2.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, -2.0f),
                glm::vec3(2.0f, 1.0f, 0.0f)
}; */

namespace RenderEngine
{
    class StaticModel
    {
    public:
        DirLight dirLight;
        SpotLight spotLight;
        GLfloat shininess = 32.0f;
        PointLight pointsLight[NR_POINT_LIGHTS];
        //Пол
        GLuint planeVAO;
        GLuint floorTexture;
        //Лампы
        GLuint lampVAO;

        StaticModel()
        {
            initLightings();
            initFloor();
            initLamps();
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
            // shader.setVec3("dirLight.direction", dirLight.direction);
            shader.setVec3("dirLight.position", dirLight.direction);
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
        void FloorDraw(RenderEngine::ShaderProgram &shader)
        {
            glBindVertexArray(planeVAO);

            shader.setFloat("material.shininess", shininess);
            shader.setInt("material.diffuse", 0);
            // shader.setInt("material.diffuse", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floorTexture);
            //todo Правильно ли делать смещение после масштабирования?
            //todo Почему вместе с камерой двигается модель, сидно если пол отрисовать в другом шейдере
            //todo Почему мы двигаем модель, а не камеру?
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            shader.setMat4("model", model);
            // ourShader.setMat4("model",  glm::mat4(1.0f));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
        void LampsDraw(RenderEngine::ShaderProgram &lampShader, mat4 &projection, mat4 &view, RenderEngine::ShaderProgram &stancilShader)
        {
            // также отрисовываем объект лампы
            // GLfloat scale1 = 0.2, scale2= 0.3;

            // а теперь мы отрисовывает столько ламп, сколько у нас есть точечных источников света
            glBindVertexArray(lampVAO);
            mat4 model;
            for (GLuint i = 0; i < NR_POINT_LIGHTS; i++)
            {
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0x01);
                lampShader.use();
                lampShader.setMat4("projection", projection);
                lampShader.setMat4("view", view);
                lampShader.setVec3("fragColor", pointsLight[i].diffuse.value);
                model = glm::mat4(1.0f);
                model = glm::translate(model, pointsLight[i].position.value);
                model = glm::scale(model, glm::vec3(0.2f));
                lampShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                glStencilMask(0x00);
                // glDisable(GL_DEPTH_TEST); //Смысла нет, просто экономия ресурсов
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                stancilShader.use();
                stancilShader.setMat4("projection", projection);
                stancilShader.setMat4("view", view);
                model = glm::scale(model, glm::vec3(1.2f));
                stancilShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            glStencilMask(0xFF);
            // glEnable(GL_DEPTH_TEST);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);

            lampShader.use();
            lampShader.setMat4("projection", projection);
            lampShader.setMat4("view", view);
            lampShader.setVec3("fragColor", vec3(1.0f));
            model = glm::mat4(1.0f);
            model = glm::translate(model, dirLight.position);
            // model = glm::translate(model, dirLight.direction * vec3(-3.0f));
            model = glm::scale(model, glm::vec3(0.2f)); // куб, меньшего размера
            lampShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(0);
        }

        ~StaticModel()
        {
        }

    protected:
        void initFloor()
        {
            floorTexture = TextureFromFile("assets/textures/metal.jpg");
            float planeVertices[] = {
                // координаты          // текстурные координаты (обратите внимание, что мы устанавливаем их значения больше единицы (в сочетании с GL_REPEAT в качестве режима наложения текстур). Это приведет к повторению текстуры пола)
                5.0f, 0.0f, 5.0f,   0.0f,  1.0f,  0.0f,  2.0f, 0.0f,
                -5.0f, 0.0f, 5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
                -5.0f, 0.0f, -5.0f, 0.0f,  1.0f,  0.0f,  0.0f, 2.0f,

                5.0f, 0.0f, 5.0f, 0.0f,  1.0f,  0.0f,  2.0f, 0.0f,
                -5.0f, 0.0f, -5.0f, 0.0f,  1.0f,  0.0f,  0.0f, 2.0f,
                5.0f, 0.0f, -5.0f, 0.0f,  1.0f,  0.0f,  2.0f, 2.0f};
            unsigned int planeVBO;
            glGenVertexArrays(1, &planeVAO);
            glGenBuffers(1, &planeVBO);
            glBindVertexArray(planeVAO);
            glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
            glBindVertexArray(0);
        }
        void initLamps()
        {
            GLfloat vertices[] = {
                // координаты
                -0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, 0.5f, -0.5f,
                0.5f, 0.5f, -0.5f,
                -0.5f, 0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f, -0.5f, 0.5f,
                0.5f, -0.5f, 0.5f,
                0.5f, 0.5f, 0.5f,
                0.5f, 0.5f, 0.5f,
                -0.5f, 0.5f, 0.5f,
                -0.5f, -0.5f, 0.5f,

                -0.5f, 0.5f, 0.5f,
                -0.5f, 0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, 0.5f,
                -0.5f, 0.5f, 0.5f,

                0.5f, 0.5f, 0.5f,
                0.5f, 0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, 0.5f,
                0.5f, 0.5f, 0.5f,

                -0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, 0.5f,
                0.5f, -0.5f, 0.5f,
                -0.5f, -0.5f, 0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f, 0.5f, -0.5f,
                0.5f, 0.5f, -0.5f,
                0.5f, 0.5f, 0.5f,
                0.5f, 0.5f, 0.5f,
                -0.5f, 0.5f, 0.5f,
                -0.5f, 0.5f, -0.5f};

            //2. настраиваем VAO света (VBO остается неизменным; вершины те же и для светового объекта, который также является 3D-кубом)
            GLuint lampVBO;
            glGenVertexArrays(1, &lampVAO);
            glBindVertexArray(lampVAO);
            glGenBuffers(1, &lampVBO);

            glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
            glEnableVertexAttribArray(0);
        }
        void initLightings()
        {
            // координаты точечных источников света
            vec3 pointsTmp[NR_POINT_LIGHTS] = {
                glm::vec3(0.0f, 1.0f, 2.0f),
                glm::vec3(-2.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, -2.0f),
                glm::vec3(2.0f, 1.0f, 0.0f)};

            glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
            glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
            glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
            vec3 colors[NR_POINT_LIGHTS] = {red, green, blue, yellow};

            vec3 ambientDir = vec3(0.75f);
            // vec3 ambientPoints = vec3(0.9) - vec3(0.65f);
            vec3 specular = vec3(1.0f);
            float constant = 1.0f;
            float linear = 0.09f;
            float qaudratic = 0.032f;
            // точечные источники света
            for (GLuint i = 0; i < NR_POINT_LIGHTS; i++)
            {
                string ni = to_string(i);
                pointsLight[i].position.key = "pointLights[" + ni + "].position";
                pointsLight[i].position.value = pointsTmp[i];
                pointsLight[i].diffuse.key = "pointLights[" + ni + "].diffuse";
                pointsLight[i].diffuse.value = colors[i];
                pointsLight[i].ambient.key = "pointLights[" + ni + "].ambient";
                pointsLight[i].ambient.value = vec3(0.0f);
                pointsLight[i].specular.key = "pointLights[" + ni + "].specular";
                pointsLight[i].specular.value = specular;
                pointsLight[i].constant.key = "pointLights[" + ni + "].constant";
                pointsLight[i].constant.value = constant;
                pointsLight[i].linear.key = "pointLights[" + ni + "].linear";
                pointsLight[i].linear.value = linear;
                pointsLight[i].quadratic.key = "pointLights[" + ni + "].quadratic";
                pointsLight[i].quadratic.value = qaudratic;
            }

            // dirLight.position = vec3(1.2f, 1.0f, 2.0f);
            dirLight.position = vec3(2.0f, 2.0f, 2.0f);
            dirLight.direction = vec3(-0.2f, -1.0f, -0.3f);
            dirLight.ambient = ambientDir;
            dirLight.diffuse = vec3(0.1f);
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
    };
} // namespace RenderEngine

#endif //STATIC_H
