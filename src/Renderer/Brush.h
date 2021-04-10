/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef BRUSH_GENERATOR_H
#define BRUSH_GENERATOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#include "ShaderProgram.h"
using namespace glm;

// Представление отдельно взятой частицы и её состояния
struct Particle {
    vec2 Position;
    vec4 Color;
    GLfloat Life;
    GLboolean isNew;
    GLfloat A;

    Particle(vec2 pos = vec2(0.0f), vec2 vel = vec2(0.0f), vec4 color = vec4(0.0f), GLfloat life = 0.0f, GLboolean isNew = false) : Position(pos), Color(color), Life(life), isNew(isNew) { };

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

class BrushGenerator
{
public:
    // Конструктор
    BrushGenerator(RenderEngine::ShaderProgram &shader, vec2 position,  GLfloat length, GLfloat scaleX, GLfloat scaleY, GLint inversion);
	
    // Обновляем все частицы
    void Update();
	
    // Рендерим все частицы
    void Draw(mat4 &projection, mat4 &view);
private:
    // Состояние
    std::vector<Particle> particles;
    GLuint amount = 0;
    GLfloat life, tick, interval, counter;
    GLuint head = 0;

    // Рендеринг
    RenderEngine::ShaderProgram &shader;
    GLuint VAO;
	
    // Инициализация буферов и вершинных атрибутов
    void init(vec2 position, GLfloat length, GLfloat scaleX, GLfloat scaleY, GLint inversion);
	
    // Возрождение частиц
    void respawnParticle(Particle& particle);

    GLuint nPointsOfCircle;
    void drawCircle(GLfloat x, GLfloat y, GLfloat, GLfloat radius, GLuint numberOfSides);
};

#endif