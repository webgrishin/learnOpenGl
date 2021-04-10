/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#include "ShaderProgram.h"
using namespace glm;

// Представление отдельно взятой частицы и её состояния
struct Particle {
    vec2 Position, Velocity;
    vec4 Color;
    GLfloat Life;
    GLboolean isNew;
    GLfloat A;

    Particle(vec2 pos = vec2(0.0f), vec2 vel = vec2(0.0f), vec4 color = vec4(0.0f), GLfloat life = 0.0f, GLboolean isNew = false) : Position(pos), Velocity(vel), Color(color), Life(life), isNew(isNew) { };

    GLboolean isAlive(){
        return this->isNew && this->Life > 0.5f;
/*         return this->isNew && this->Life > (this->A-0.3f);
        if(this->isNew){
            if (this->A > 0.8f)
                return this->Life > 0.5f;
            // if (this->A > 0.5f)
                // return this->Life > 0.5f;
            return this->Life > this->A;
        }
        return false; */
    };   

};


// ParticleGenerator действует как контейнер для рендеринга большого количества частиц, 
// многократно порождая и обновляя частицы и убивая их через заданный промежуток времени
class ParticleGenerator
{
public:
    // Конструктор
    ParticleGenerator();
    // ParticleGenerator(RenderEngine::ShaderProgram shader, GLuint amount);
	
    // Обновляем все частицы
    void Update();
    void Update2();
    void Update3();
    void Update4();
    void CreateParticle();
    // void Update(GLfloat dt, GLuint newParticles);
	
    // Рендерим все частицы
    void Draw(mat4 projection, mat4 view);
private:
    // Состояние
    std::vector<Particle> particles;
    GLuint amount;
    GLboolean next = false;
    GLfloat life, tick, interval, speedFading, counter;
    GLuint frame, head = 0, tail = 0;

    // Рендеринг
    RenderEngine::ShaderProgram shader;
    GLuint VAO;
	
    // Инициализация буферов и вершинных атрибутов
    void init();
	
    // Возвращаем индекс первой незадействованной в данный момент частицы (при Life <= 0.0f) или 0, если таких частиц в данный момент нет
    unsigned int firstUnusedParticle();
	
    // Возрождение частиц
    void respawnParticle(Particle& particle);

    GLuint nPointsOfCircle;
    void drawCircle(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z =0.0f, GLfloat radius = 10.0f, GLuint numberOfSides = 6);
};

#endif