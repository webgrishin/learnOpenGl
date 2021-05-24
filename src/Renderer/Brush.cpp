#include "Brush.h"
#include <GLFW/glfw3.h>
#include "../checkError.h"

BrushGenerator::BrushGenerator(RenderEngine::ShaderProgram &shader, vec2 position, GLfloat length, GLfloat scaleX, GLfloat scaleY, GLint inversion): shader(shader)
{
    this->init(position, length, scaleX, scaleY, inversion);
}

void BrushGenerator::Update()
{
    return;
    // std::cout << this->counter<< " " <<this->tick << std::endl;
    // std::cout << this->amount << " " << this->tick<<" "<< this->interval << " "<<this->counter << std::endl;
    double now = glfwGetTime();
    double age = now - this->lastTime;
    this->counter = age;
    // printf("now:%f; age: %f; lastTime: %f;\n", now, age, lastTime);

            // accumulator += delta;
    if (this->counter > this->interval)
    {
        this->newParticle();
/*      if (this->head >= this->amount)
            this->head = 0;
        
        // std::cout << this->head << std::endl;
        this->respawnParticle(this->particles[this->head]);
        this->head++; */
        this->counter = 0.0f;
        this->lastTime = now;
    }

    GLfloat Transp;
    // Transp = 1.0 - age / 0.5;
    Transp =  1.0 - age / (this->interval*45.0f);
    for (GLuint i = 0; i < this->amount; i++)
    {
        Particle &p = this->particles[i];
        if (p.isAlive())
        {
            // p.Life -= Transp;
            // p.Color.a -= Transp;
            // p.Life *= Transp;
            // printf("a: %f;\n",p.Color.a);
            p.Color.a *= Transp;
            // if (!p.isAlive())
                // this->newParticle();
            // printf("i:%i; Transp:%f; Life: %f; a: %f; A:%f\n", i, Transp, p.Life, p.Color.a, p.A);

            // p.Life -= this->tick;
            //// p.Color.a = p.Life;
            // p.Color.a -= this->tick;
        }
    }

    // this->counter += this->tick;
}

void BrushGenerator::newParticle()
{
    if (this->head >= this->amount)
        this->head = 0;
    // std::cout << this->head << std::endl;
    this->respawnParticle(this->particles[this->head]);
    this->head++;
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
    shader.setMat4("model", mat4(1.0f));
    shader.setFloat("Time", glfwGetTime());
    shader.setFloat("ParticleLifetime", this->interval*45.0f);
    glBindVertexArray(this->VAO);
/*     mat4 model;
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
    } */
    // glDrawArrays(GL_POINTS, 0, 3);
    // glCheckError();
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_POINTS, 0, this->amount);
    // glBindVertexArray(0);

    // Не забываем сбросить режим смешивания к изначальным настройкам
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void BrushGenerator::init(vec2 position = vec2(0.0f, -1.0f), GLfloat length = 1.0f, GLfloat scaleX = 5.0f, GLfloat scaleY = 0.1f, GLint inversion = 1)
{
    this->life = 1.0f;

    GLfloat absoluteHeightPx = 600.0f; //высота окна в пикселях
    GLfloat relativeHeightPart = 2.0f; //Высота окна в частях
    GLfloat x = position.x;            //координаты начала графика
    GLfloat y = position.y;
    GLfloat onePxPart = relativeHeightPart / absoluteHeightPx; //шаг смещения (количество частей в одном пикселе)
    GLfloat scale = scaleY * scaleX;
    GLfloat offset; //разряжённость (сколько точек в графике)
    if (scale < 1.0f)
    {
        // offset = 15.0f;
        offset = 5.0f;
        this->interval = 0.012f; //Временная задержка(Скорость пульсации)
        // this->interval = 0.002f; //Временная задержка(Скорость пульсации)
    }
    else
    {
        offset = 5.0f;
        // offset = 10.0f - ((scale - 1) * 10.0f/4.0f);
        this->interval = 0.011f; //Временная задержка(Скорость пульсации)
        // this->interval = 0.001f; //Временная задержка(Скорость пульсации)
    }
    // offset = 1.0f;
    // interval = glm::mix(0.01f, 0.05f, randFloat());

    // srand(static_cast<unsigned int>(time(0)));
    // rand();
    offset = glm::mix(1.0f, 5.0f, randFloat());
    // printf("offset:%f\n", offset);
    this->counter = 0.0f;
    // GLfloat period = this->interval * 100000.0f;
    // this->tick = this->life / period;

    GLfloat realLength = length / onePxPart;
    vec4 bC = vec4(1.0f, 1.0f, 15.0f / 255.0f, 1.0f); //цвет начала графика
    vec4 eC = vec4(1.0f, 0.0f, 0.0f, 1.0f);           // цвет конца графика
    // vec4 bC = vec4(1.0f, 1.0f, 0.0f, 1.0f);//цвет начала графика
    // vec4 eC = vec4(1.0f, 0.0f, 0.0f, 1.0f);// цвет конца графика
    vec4 dC = (eC - bC) / realLength; //шаг смещения цвета
    GLfloat dA = 1.0f / realLength;   //Снижение празрачности на 1 px
    onePxPart *= offset;
    dC *= offset;
    dA *= offset;
    /* 
    3.1. Научиться делать градиент с занными пропорциями

    4.Уменьшение диаметра.
        4.1.EBO
        
        +++++<<<<<<<<
    5.Отрыв искр. искры короткие

    7.ПоЭксперементировать с ускорением, а не временем
    7.1. Если с ускорением не получится, тогда отвязаться от жизни(Life)

    2.У каждого луча должна быть своя пульсация. убрать синхронность

     */

    GLfloat maxA = length; // * 2.0f/3.0f;
    scaleY *= inversion;
    Particle p;
    GLfloat k;
    k = 0.0f;
    /*     {
        k = length / 5.0f;
        bC += (k / onePxPart * dC);
    } */
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
        rX = x + sin(i * scaleX) * exp(-i) * scaleY;
        // rX = x + sin(i * scaleX) * exp(-i * 0.5f) * scaleY;
        // std::cout << "sin: " << rX << " ";

        // rX = x + sin(i * scaleX) * scaleY;
        // rX = x;

        p.Color = bC;
        p.Position = vec2(rX, y);
        p.Life = 1.0f;
        this->particles.push_back(p);
    }
    // std::cout << std::endl;
    this->amount = this->particles.size();
    // this->drawCircle(0.0f, 0.0f, 0.0f, 0.08f, 12);
    // return;
    GLuint size_pos = this->amount * 2;
    GLuint size_color = this->amount * 4;
    GLfloat *pos = new GLfloat[size_pos];
    GLfloat *color = new GLfloat[size_color];
    GLfloat *time = new GLfloat[this->amount];
    GLfloat *initPos = new GLfloat[this->amount];
    GLfloat t = 0.0f;
    for (GLuint i = 0; i < this->amount; i++)
    {
        initPos[i] = this->particles[0].Position.x;
        t+=this->interval;
        time[i] = t;
        pos[2*i] = this->particles[i].Position.x;
        pos[2*i+1] = this->particles[i].Position.y;
        color[4*i] = this->particles[i].Color.r;
        color[4*i+1] = this->particles[i].Color.g;
        color[4*i+2] = this->particles[i].Color.b;
        color[4*i+3] = this->particles[i].Color.a;
    }
    // printf("amount:%i\n", amount);
    // for (GLuint i = 0; i < this->amount; i++)
    // {
        // printf("+i:%i (%f, %f); n:%i (%f, %f);\n", 0, pos[0], pos[1], amount, pos[size_pos-2], pos[size_pos-1]);
        // printf("-i:%i (%f, %f); n:%i (%f, %f);\n", 0, particles[0].Position.x, particles[0].Position.y, amount, particles[amount-1].Position.x, particles[amount-1].Position.y);
    // }

    // std::cout << "constructor" << std::endl;
    // std::cout << this->amount << " " << this->tick << " " << this->interval << " " << this->counter << std::endl;

    // this->drawCircle(0.0f, 0.0f, 0.0f, 0.08f, 12);

    GLuint VBO_POS, VBO_COLOR, VBO_TIME, VBO_INITPOS;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO_POS);
    glGenBuffers(1, &VBO_COLOR);
    glGenBuffers(1, &VBO_TIME);
    glGenBuffers(1, &VBO_INITPOS);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_POS);
    glBufferData(GL_ARRAY_BUFFER, size_pos * sizeof(GLfloat), pos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_COLOR);
    glBufferData(GL_ARRAY_BUFFER, size_color * sizeof(GLfloat), color, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_TIME);
    glBufferData(GL_ARRAY_BUFFER, this->amount * sizeof(GLfloat), time, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_INITPOS);
    glBufferData(GL_ARRAY_BUFFER, this->amount * sizeof(GLfloat), initPos, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(3);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    delete [] pos;
    delete [] color;
    delete [] time;
}

void BrushGenerator::drawCircle(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f, GLfloat radius = 0.1f, GLuint numberOfSides = 6)
{
    this->nPointsOfCircle = numberOfSides + 2;

    GLfloat twicePi = 2.0f * M_PI;

    std::vector<GLfloat> allCircleVertices;
    allCircleVertices.push_back(x);
    allCircleVertices.push_back(y);
    allCircleVertices.push_back(1.0f); //Прозрачность центра круга
    GLfloat rx, ry;

    for (int i = 1; i < this->nPointsOfCircle; i++)
    {
        rx = x + (radius * cos(i * twicePi / numberOfSides));
        ry = y + (radius * sin(i * twicePi / numberOfSides));
        // printf("i:%i; x:%f; y:%f\n", i, rx, ry);
        allCircleVertices.push_back(rx);
        allCircleVertices.push_back(ry);
        allCircleVertices.push_back(0.1f); //Прозрачность контура круга
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
GLfloat BrushGenerator::randFloat()
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    // Равномерно распределяем рандомное число в нашем диапазоне
    return static_cast<float>(rand() * fraction);
}