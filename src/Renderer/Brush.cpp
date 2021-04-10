#include "Brush.h"

BrushGenerator::BrushGenerator(RenderEngine::ShaderProgram &shader, vec2 position, GLfloat length, GLfloat scaleX, GLfloat scaleY, GLint inversion): shader(shader)
{
    std::cout <<"constructor" << std::endl;
    this->init(position, length, scaleX, scaleY, inversion);
}

void BrushGenerator::Update()
{
    // std::cout << this->counter<< " " <<this->tick << std::endl;
    // std::cout << this->amount << " " << this->tick<<" "<< this->interval << " "<<this->counter << std::endl;
    if (this->counter > this->interval)
    {
        if (this->head >= this->amount)
            this->head = 0;
        // std::cout << this->head << std::endl;
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

void BrushGenerator::respawnParticle(Particle &particle)
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
void BrushGenerator::Draw(mat4 &projection, mat4 &view)
{
    // this->counter += this->tick;
    // std::cout << this->counter<< " " <<this->tick << std::endl;
    // this->counter += this->tick;
    // Используем аддитивный режим смешивания для придания эффекта свечения
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    this->shader.setMat4("projection", projection);
    this->shader.setMat4("view", view);
    glBindVertexArray(this->VAO);
    mat4 model;
    for (GLuint i = 0; i < this->amount; i++)
    // for (Particle particle : this->particles)
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

void BrushGenerator::init(vec2 position = vec2(0.0f, -1.0f), GLfloat length = 1.0f, GLfloat scaleX = 5.0f, GLfloat scaleY = 0.1f, GLint inversion = 1)
{
    this->life = 1.0f;

    GLfloat absoluteHeightPx = 800.0f;//высота окна в пикселях
    GLfloat relativeHeightPart = 2.0f;//Высота окна в частях
    GLfloat x = position.x;//координаты начала графика
    GLfloat y = position.y;
    GLfloat onePxPart = relativeHeightPart / absoluteHeightPx; //шаг смещения (количество частей в одном пикселе)
    GLfloat scale = scaleY * scaleX;
    GLfloat offset;//разряжённость (сколько точек в графике)
    if (scale < 1.0f){
        // offset = 15.0f;
        offset = 5.0f;
        this->interval = 0.002f; //Временная задержка(Скорость пульсации)
    }
    else{
        offset = 1.0f;
        // offset = 10.0f - ((scale - 1) * 10.0f/4.0f);
        this->interval = 0.001f; //Временная задержка(Скорость пульсации)
    }

    this->counter = 0.0f;
    GLfloat period = this->interval * 100000.0f;
    this->tick = this->life / period;

    GLfloat realLength = length / onePxPart;
    vec4 bC = vec4(1.0f, 1.0f, 0.0f, 1.0f);//цвет начала графика
    vec4 eC = vec4(1.0f, 0.0f, 0.0f, 1.0f);// цвет конца графика
    vec4 dC = (eC - bC) / realLength;//шаг смещения цвета
    GLfloat dA = 1.0f / realLength; //Снижение празрачности на 1 px
    onePxPart *= offset;
    dC *= offset;
    dA *= offset;
    /* 
    1.Поправить ГСПЧ
    3.Добавить белый цвет в корне
        3.1. Научиться делать градиент с занными пропорциями
    6.Режимы смешивания

    4.Уменьшение диаметра.
        4.1.EBO
    5.Отрыв искр. искры короткие

    7.ПоЭксперементировать с ускорением, а не временем
    7.1. Если с ускорением не получится, тогда отвязаться от жизни(Life)
    2.У каждого луча должна быть своя пульсация. убрать синхронность

     */

    GLfloat maxA = length;// * 2.0f/3.0f;
    scaleY *= inversion;
    Particle p;
    GLfloat k;
    k = 0.0f;
    {
        k = length / 2.0f;
        bC += (k / onePxPart * dC);
    }
    GLfloat rX = 0.0f;

    // std::cout << "init: " << k <<std::endl;

    for (GLfloat i = k; i < length && (maxA - i > 0); i += onePxPart)
    {
        p = Particle();
        bC += dC;
        // bC.a -=dA;
        p.A = maxA - i;
        bC.a = 0.0f;
        // bC.a = p.A;
        y = y + onePxPart;
        rX = x + sin(i * scaleX) * exp(-i * 0.5f) * scaleY;
        // std::cout << "sin: " << rX << " ";

        // x = sin(i * scaleX) * scaleY;

        p.Color = bC;
        p.Position = vec2(rX, y);
        p.Life = 1.0f;
        this->particles.push_back(p);
    }
    // std::cout << std::endl;
    this->amount = this->particles.size();
    std::cout << this->amount << " " << this->tick << " " << this->interval << " " << this->counter << std::endl;

    this->drawCircle(0.0f, 0.0f, 0.0f, 0.05f, 12);
}

void BrushGenerator::drawCircle(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f, GLfloat radius = 0.1f, GLuint numberOfSides = 6)
{
    this->nPointsOfCircle = numberOfSides + 2;

    GLfloat twicePi = 2.0f * M_PI;

    std::vector<GLfloat> allCircleVertices;
    allCircleVertices.push_back(x);
    allCircleVertices.push_back(y);
    allCircleVertices.push_back(1.0f); //Прозрачность центра круга

    for (int i = 1; i < this->nPointsOfCircle; i++)
    {
        allCircleVertices.push_back(x + (radius * cos(i * twicePi / numberOfSides)));
        allCircleVertices.push_back(y + (radius * sin(i * twicePi / numberOfSides)));
        allCircleVertices.push_back(0.0f); //Прозрачность контура круга
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
