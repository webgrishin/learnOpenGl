/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "particle_generator.h"

ParticleGenerator::ParticleGenerator()
{
    this->init();
}
/* ParticleGenerator::ParticleGenerator(RenderEngine::ShaderProgram shader, GLuint amount)
    : shader(shader), amount(amount)
{
    this->init();
} */

void ParticleGenerator::Update2()
{
    GLfloat dt = this->tick;
    GLuint newParticles = 1;
    // Добавляем новые частицы
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle]);
    }
    // Обновляем все частицы
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        p.Life -= dt;    // уменьшаем время жизни
        p.Color.a -= dt; // * 2.5f;
                         /*         if (p.Life > 0.0f)
        {   // если частица жива, то обновляем её
            // p.Position -= p.Velocity * dt;
            p.Color.a -= dt;// * 2.5f;
        }*/
    }
}
void ParticleGenerator::CreateParticle()
{
    this->counter += this->tick;
    if (this->counter > this->interval)
    {
        if (this->head < this->amount)
        {
            this->respawnParticle(this->particles[this->head]);
            this->head++;
            this->counter = 0.0f;
        }
        else if (this->tail == this->amount)
        {
            this->tail = this->head = 0;
            this->respawnParticle(this->particles[this->head]);
            this->head++;
            this->counter = 0.0f;
        }
    }
}
void ParticleGenerator::Update4()
{
    if (this->counter > this->interval)
    {
        if (this->head >= this->amount)
            this->head = 0;

        this->respawnParticle(this->particles[this->head]);
        this->head++;
        this->counter = 0.0f;
    }

    for (GLuint i = 0; i < this->amount; i++)
    {
        Particle &p = this->particles[i];
        if (p.isAlive())
        {
            p.Life -= this->tick;
            // p.Color.a = p.Life;
            p.Color.a -= this->tick;
        }
    }

    this->counter += this->tick;
}
void ParticleGenerator::Update3()
{
    if (this->counter > this->interval)
    {
        if (this->head < this->amount)
        {
            this->respawnParticle(this->particles[this->head]);
            this->head++;
            this->counter = 0.0f;
        }
        else if (this->tail == this->amount)
        {
            this->tail = this->head = 0;
            this->respawnParticle(this->particles[this->head]);
            this->head++;
            this->counter = 0.0f;
        }
    }

    for (GLuint i = this->tail; i < this->head; i++)
    {
        Particle &p = this->particles[i];
        // GLfloat k = (this->head - this->tail);
        p.Life -= this->tick;
        p.Color.a -= this->tick;

        if (p.Life <= 0.0f)
        {
            this->tail++;
            // this->next = true;
        }
    }
    this->counter += this->tick;
}
void ParticleGenerator::Update()
{
    for (GLuint i = this->tail; i < this->head; i++)
    {
        Particle &p = this->particles[i];
        // GLfloat k = (this->head - this->tail);
        p.Life -= this->tick;
        p.Color.a -= this->tick;
        if (p.Life <= 0.0f)
            this->tail++;
    }

    // Добавляем новые частицы
    if (this->head - this->tail < this->frame && this->head < this->amount)
    {
        this->respawnParticle(this->particles[this->head]);
        this->head++;
    }
    else if (this->tail == this->amount)
        this->tail = this->head = 0;
}
void ParticleGenerator::respawnParticle(Particle &particle)
{
    // float random = ((rand() % 100) - 50) / 10.0f;
    // float rColor = 0.5f + ((rand() % 100) / 100.0f);
    // particle.Position = object.Position + random;
    // particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    // particle.Color.a = 1.0f;
    particle.Color.a = particle.A;
    particle.Life = this->life;
    particle.isNew = true;
    // particle.Velocity = object.Velocity * 0.1f;
}

// Рендеринг всех частиц
void ParticleGenerator::Draw(mat4 projection, mat4 view)
{
    // Используем аддитивный режим смешивания для придания эффекта свечения
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    this->shader.setMat4("projection", projection);
    this->shader.setMat4("view", view);
    glBindVertexArray(this->VAO);
    mat4 model;
    // for (Particle particle : this->particles)
    for (GLuint i = 0; i < this->amount; i++)
    {
        // if (particle.Life > 0.0f)
        // {
            // this->shader.SetVector2f("offset", particle.Position);
            // this->shader.SetVector4f("color", particle.Color);
            // glBindVertexArray(this->VAO);
            // glDrawArrays(GL_TRIANGLES, 0, 6);
            // glBindVertexArray(0);

            // this->shader.setVec4("fragColor", particle.Color);
            this->shader.setVec4("fragVex", this->particles[i].Color);
            model = translate(mat4(1.0f), vec3(this->particles[i].Position, 0.0f));
            shader.setMat4("model", model);
            // glDrawArrays(GL_POINTS, 0, 1);
            // glDrawArrays(GL_POINTS, 0, this->nPointsOfCircle);
            glDrawArrays(GL_TRIANGLE_FAN, 0, this->nPointsOfCircle);
            // glDrawArrays(GL_TRIANGLES, 0, 3);
        // }
    }
    glBindVertexArray(0);

    // Не забываем сбросить режим смешивания к изначальным настройкам
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    // this->frame = 10;
    this->life = 1.0f;

    GLfloat absoluteHeightPx = 800.0f;//высота окна в пикселях
    GLfloat relativeHeightPart = 2.0f;//Высота окна в частях
    GLfloat x = 0.0f, y = -1.0f;//координаты начала графика
    GLfloat onePxPart = relativeHeightPart / absoluteHeightPx; //шаг смещения (количество частей в одном пикселе)
    std::cout << onePxPart << std::endl;

    GLfloat length = 3.0f;

    GLfloat scaleX = 5.0f;//1-5; сжатие графика(количество периодов)
    GLfloat scaleY = 0.1f;//0.1-1.0 масштабирование по Y
    GLfloat scale = scaleY * scaleX;
    GLfloat offset;//разряжённость (сколько точек в графике)
    if (scale < 1.0f){
        offset = 20.0f;
        this->interval = 0.04f; //Временная задержка(Скорость пульсации)
    }
    else{
        offset = 15 - ((scale - 1) * 10.0f/4.0f);
        this->interval = 0.03f; //Временная задержка(Скорость пульсации)
    }
    
    // offset = 20.0f - (((scaleY - 0.1f) * 10.0f) * 15.0f/9.0f);
    std::cout << offset << std::endl;
    // offset = 15.0f;

    GLfloat period = this->interval * 100000.0f;
    // this->counter = 1.0f;
    this->counter = 0.0f;
    this->tick = this->life / period;
    // this->speedFading = 1.0f / 50.0f;
    // this->speedFading = 1.0f/this->frame;
    // Настройка свойств меша и атрибутов
    shader.create("assets/shaders/point.vs", "assets/shaders/point.fs");

    GLfloat realLength = length / onePxPart;

    vec4 bC = vec4(1.0f, 1.0f, 0.0f, 1.0f);//цвет начала графика
    vec4 eC = vec4(1.0f, 0.0f, 0.0f, 1.0f);// цвет конца графика
    vec4 dC = (eC - bC) / realLength;//шаг смещения цвета
    GLfloat dA = 1.0f / realLength; //Снижение празрачности на 1 px
    onePxPart *= offset;
    dC *= offset;
    dA *= offset;
    // std::cout << (dA < onePxPart) << std::endl;

    // GLfloat dA = onePxPart / 2.0f;
    GLfloat maxA = length;// * 1.0f/3.0f;
    Particle p;
    for (GLfloat i = 0.0f; i < length && (maxA - i > 0); i += onePxPart)
    {
        bC += dC;
        // maxA -=dA;
        // std::cout << maxA << std::endl;
        // bC.a = maxA;
        // bC.a -=dA;
        bC.a = maxA - i;
        y = y + onePxPart;
        x = sin(i * scaleX) * scaleY;
        // x = sin(i * scaleX) * scaleY;

        p = Particle();
        p.Color = bC;
        p.A = bC.a;
        p.Position = vec2(x, y);
        p.Velocity = vec2(onePxPart);
        p.Life = 1.0f;
        this->particles.push_back(p);
        // this->particles.push_back(Particle(vec2(x, y), vec2(onePxPart), bC, 1.0f));
    }
    this->amount = this->particles.size();
    std::cout << this->amount << std::endl;
    // this->next = true;

    this->drawCircle(0.0f, 0.0f, 0.0f, 0.1f, 12);
}

// Сохраняем индекс последней использованной частицы (для быстрого доступа к следующей использованной частицы)
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle()
{
    // Сначала проводим поиск, начиная с последней использованной частицы (как правило, результат возвращается почти мгновенно)
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i)
    {
        if (this->particles[i].Life <= 0.0f)
        {
            lastUsedParticle = i;
            return i;
        }
    }

    // В противном случае, выполняем линейный поиск
    for (unsigned int i = 0; i < lastUsedParticle; ++i)
    {
        if (this->particles[i].Life <= 0.0f)
        {
            lastUsedParticle = i;
            return i;
        }
    }

    // Все частицы еще живые, поэтому перезаписываем первую (обратите внимание, что если программа
    // неоднократно попадает в данный вариант событий, то следует зарезервировать больше частиц)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLuint numberOfSides)
{
    this->nPointsOfCircle = numberOfSides + 2;

    GLfloat twicePi = 2.0f * M_PI;

    std::vector<GLfloat> allCircleVertices;
    allCircleVertices.push_back(x);
    allCircleVertices.push_back(y);
    allCircleVertices.push_back(1.0f);

    for (int i = 1; i < this->nPointsOfCircle; i++)
    {
        allCircleVertices.push_back(x + (radius * cos(i * twicePi / numberOfSides)));
        allCircleVertices.push_back(y + (radius * sin(i * twicePi / numberOfSides)));
        allCircleVertices.push_back(0.1f);
    }

    /* float vertices[] = { 
        0.0f, 0.0f, 0.0f,  
        -0.4f, 0.4f, 0.0f,  
        0.4f, 0.4f, 0.0f,  
        0.8f, 0.0f, 0.0f,  
        0.4f, -0.4f, 0.0f,  
        -0.4f, -0.4f, 0.0f,  
        -0.8f, 0.0f, 0.0f,  
        -0.4f, 0.4f, 0.0f,  
}; */

    GLuint VBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, allCircleVertices.size() * sizeof(GLfloat), &allCircleVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glBindVertexArray(0);
}

/* void ParticleGenerator::drawPoint(){
    GLfloat point[] = {0.0f, -1.0f};
    GLuint VBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindVertexArray(0);
}; */