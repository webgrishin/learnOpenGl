/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef FIRE_H
#define FIRE_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <time.h>
#define _USE_MATH_DEFINES

#include "ShaderProgram.h"
#include "MCircle.h"

using namespace glm;

class Fire
{
public:
    // Конструктор
    Fire(vec2 position, GLfloat width, GLuint wWdth, GLuint wHeight);
    void Draw(mat4 &projection, mat4 &view);
private:
    RenderEngine::ShaderProgram shader;
    GLint getRandomNumber(int min, int max);
    GLfloat _grapfXQR(float a, float x, float b);

    float timeNow, deltaT, particleLifetime;
    void update( float t );
    void render(mat4 &projection, mat4 &view);
    // GLuint VAO;
    GLuint VAOparticleArray[2];
    GLuint feedback[2];
    GLuint drawBuf;
    GLuint renderSub, updateSub;
    void initBuffers();
    GLfloat randFloat();
    void initFire(vec2 position, GLfloat width);
    std::vector<vec2> Pos;
    // std::vector<vec2> initPos;
    std::vector<GLfloat> LifeTime;
    std::vector<GLfloat> StartTime;
    std::vector<GLfloat> ScaleX;
    std::vector<GLfloat> ScaleY;
    std::vector<GLfloat> Inversion;
    // GLuint nPointsOfCircle;

    uint nParticles;
    MCircle circle;
};

#endif