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
#include <vector>

#include "Brush.h"
#include "ShaderProgram.h"

using namespace glm;

class Fire
{
public:
    // Конструктор
    Fire(vec2 position, GLfloat width, GLuint wWdth, GLuint wHeight);
    void Draw(mat4 &projection, mat4 &view);
private:
    RenderEngine::ShaderProgram shader;
    std::vector<BrushGenerator> brushes;
    int getRandomNumber(int min, int max);
};

#endif