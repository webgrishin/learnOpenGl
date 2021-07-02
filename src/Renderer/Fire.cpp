#include "Fire.h"
void Fire::initBuffers()
{
    GLuint aVerteciesCircle, aPosBuf[2], aStartTime[2], aInitPos, aParticleLifetime, aScaleX, aScaleY, aInversion;
    this->nParticles = Pos.size();
    // Create VAO for each set of buffers
    glGenVertexArrays(2, VAOparticleArray);
    // Generate the buffers
    glGenBuffers(1, &aVerteciesCircle);
    glGenBuffers(2, aPosBuf);    // position buffers
    glGenBuffers(2, aStartTime); // Start time buffers
    glGenBuffers(1, &aInitPos);
    glGenBuffers(1, &aParticleLifetime);
    glGenBuffers(1, &aScaleX);
    glGenBuffers(1, &aScaleY);
    glGenBuffers(1, &aInversion);

    // Allocate space for all buffers
    int size = nParticles * 2 * sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, aPosBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, aPosBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, aInitPos);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    size = nParticles * sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, aStartTime[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, aStartTime[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, aParticleLifetime);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, aScaleX);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, aScaleY);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, aInversion);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, aVerteciesCircle);
    glBufferData(GL_ARRAY_BUFFER, circle.getVerteciesCurcle().size() * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

    /*
    1.Исправить баг при запуске
    2.Разобраться почему плавает волос
    ---
    3.рассчитать количество частитц в зависимости от времени жизни и интервала. или наоборот: интервал в зависимости от количества и времениЖ
    4.Сделать затухание хвоста
    5.ввести ветер и ускорение
    6.сделать изменение цвета
    7.реализовать костёр
    8.Реализовать изображение частицы
    
     */

/*     GLfloat *time = new GLfloat[nParticles];
    GLfloat *iPos = new GLfloat[nParticles];
    GLfloat *pos = new GLfloat[nParticles*2];
    particleLifetime = 2.45; //3.45
    GLfloat t = 1.0f, rate = particleLifetime/nParticles;
    int x;
    for (GLuint i = 0; i < this->nParticles; i++)
    {
        // iPos[i] = getRandomNumber(-5000, 5000)/10000.0f;
        iPos[i] = mix(-0.5f, 0.5f, this->randFloat());
        // iPos[i] = i * 0.1f;
        // iPos[i] =  0.0f;
        // pos[2*i] = 0.0f;
        // pos[2*i+1] = iPos[i];
        pos[2*i] = iPos[i];
        pos[2*i+1] = 0.0f;
        // pos[2*i+1] = mix(0.0f, 0.5f, this->randFloat());
        // pos[2*i+1] = i*0.1f;
        // iPos[i] = i*0.1f;
        time[i] = glm::mix(1.0f, 3.45f, this->randFloat());
        // if (iPos[i]==0.0f)
            // printf("%f\n", iPos[i]);
        //  time[i] = t;
        //  t+=rate;
    } */

    // Set up particle array 0
    glBindVertexArray(VAOparticleArray[0]);

    glBindBuffer(GL_ARRAY_BUFFER, aPosBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size*2, &Pos[0], GL_DYNAMIC_COPY);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, aStartTime[0]);
    glBufferData(GL_ARRAY_BUFFER, size, &StartTime[0], GL_DYNAMIC_COPY);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, aInitPos);
    glBufferData(GL_ARRAY_BUFFER, size*2, &Pos[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, aParticleLifetime);
    glBufferData(GL_ARRAY_BUFFER, size, &LifeTime[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, aScaleX);
    glBufferData(GL_ARRAY_BUFFER, size, &ScaleX[0], GL_STATIC_DRAW);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, aScaleY);
    glBufferData(GL_ARRAY_BUFFER, size, &ScaleY[0], GL_STATIC_DRAW);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(5);

    glBindBuffer(GL_ARRAY_BUFFER, aInversion);
    glBufferData(GL_ARRAY_BUFFER, size, &Inversion[0], GL_STATIC_DRAW);
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(6);

    glBindBuffer(GL_ARRAY_BUFFER, aVerteciesCircle);
    glBufferData(GL_ARRAY_BUFFER, circle.getVerteciesCurcle().size() * sizeof(GLfloat), &circle.getVerteciesCurcle()[0], GL_STATIC_DRAW);
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(7);

    // glBindBuffer(GL_ARRAY_BUFFER, initPos);
    // glBufferData(GL_ARRAY_BUFFER, nParticles * 2* sizeof(GLfloat), pos, GL_STATIC_DRAW);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    // glEnableVertexAttribArray(2);
    // Set up particle array 0
    glBindVertexArray(VAOparticleArray[1]);

    glBindBuffer(GL_ARRAY_BUFFER, aPosBuf[1]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, aStartTime[1]);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, aInitPos);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, aParticleLifetime);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, aScaleX);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, aScaleY);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(5);

    glBindBuffer(GL_ARRAY_BUFFER, aInversion);
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(6);

    glBindBuffer(GL_ARRAY_BUFFER, aVerteciesCircle);
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(7);


    glBindVertexArray(0);

    // Setup the feedback objects
    glGenTransformFeedbacks(2, feedback);

    // Transform feedback 0
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, aPosBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, aStartTime[0]);
    // glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, initPos[0]);

    // Transform feedback 1
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, aPosBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, aStartTime[1]);
    // glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, initPos[1]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    
    GLint value;
    glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &value);
    printf("MAX_TRANSFORM_FEEDBACK_BUFFERS = %d\n", value);


    this->shader.create("assets/shaders/point3.vs", "assets/shaders/point.fs");
    this->shader.use();
/*     GLfloat interval = 0.01f;
    this->shader.setFloat("ParticleLifetime", particleLifetime); //3.45

    GLfloat scaleX = getRandomNumber(10, 90) / 10.0f;
    GLfloat scaleY = getRandomNumber(5, 30) / 100.0f;
    this->shader.setFloat("scaleX", 8.0f);
    this->shader.setFloat("scaleY", 0.0009f); */
    vec4 bC = vec4(1.0f, 1.0f, 15.0f / 255.0f, 1.0f);//цвет начала графика
    vec4 eC = vec4(1.0f, 0.0f, 0.0f, 1.0f);// цвет конца графика
    vec4 dC = eC - bC; //шаг смещения цвета
    // vec4 dC = (eC - bC) / particleLifeTime; //шаг смещения цвета
    this->shader.setVec4("bC", bC);
    this->shader.setVec4("dC", dC);

    // for (uint i = 0; i < Inversion.size(); i++)
    // {
        // printf("%i ", Inversion[i]);
    // }
    

    this->renderSub = glGetSubroutineIndex(this->shader.ID, GL_VERTEX_SHADER, "render");
    this->updateSub = glGetSubroutineIndex(this->shader.ID, GL_VERTEX_SHADER, "update");
}
GLfloat Fire::_grapfXQR(float a, float x, float b){
    return a * x*x + b;
}
void Fire::initFire(vec2 position, GLfloat width){

    GLuint const c = 200; //количество язычков пламени

    GLfloat const k = 1.0f; //высота пламени, профиль пламени
    GLfloat const PI = glm::pi<float>();

    // GLfloat const a = -53.0f;
    // GLfloat const b = 1.0f;
    // GLfloat const m = sqrt(-b/a);
    GLfloat const m = PI / (2.0f * k);
    GLfloat const n = -m;
    GLfloat const l = m * 2;

    GLfloat x, y, y0, dy, nx;
    GLfloat const step = l / c;
    GLfloat *xs = new GLfloat[c];
    GLuint i = 0;
    y0 = 0;
    nx = n;

    // GLfloat const _k = l/(2*b);
    GLfloat const _k = l/2.0f;
    for (x = n; x < m && i < c; x += step, i++)
    {
        y = cos(x * k);
        // y = _grapfXQR(a, x, b);
        dy = y - y0;
        y0 = y;
        xs[i] = nx + abs(dy) * _k;
        // xs[i] = nx + (abs(dy) / 2.0f) * l;
        nx = xs[i];
        // printf("i: %i, x:%f, y: %f, dy: %f, xs: %f\n", i, x, y, dy, xs[i]);
    }

    GLfloat scaleY = 1.0f / l;
    GLfloat kl = width * scaleY;
    GLfloat kx = width / l;
    vec2 nextPosition = position;
    GLfloat x0 = xs[0], sumX = 0.0f;
    // GLchar _inversion = 1;
    GLfloat const centr = c/2.0f;
    // i = 0;
    // for (x = n; x < m && i < c; x += step, i++)
    for (i = 0; i < c; i++)
    {
        x = xs[i];
        y = cos(x * k); //*kl;
        // y = _grapfXQR(a, x, b);
        // GLfloat length = y;
        GLfloat length = glm::mix(y * 0.6f, y, this->randFloat());
        if (x==0)
            length = y;
        // printf("i: %i; x: %f; y: %f, l:%f\n", i, x, y, length);
        //0>=length<=0.63

        GLfloat const _lifeTime = 2.0f; //тоже влияет на высоту пламени, как бы масштабирование профиля пламени (k)
        GLuint const MaxParticle = 100;
        particleLifetime = length * _lifeTime; //Срок жизни частицы в язычке
        // particleLifetime = _lifeTime; //Срок жизни частицы в язычке
        // nParticles = particleLifetime * MaxParticle / _lifeTime;
        nParticles = length * MaxParticle; //длина червячка
        if (nParticles > 10)
        {
            // nParticles = MaxParticle; //Количество частиц в одном язычке пламени

            GLfloat period = glm::mix(0.6f, 4.0f, this->randFloat());
            GLfloat rate = particleLifetime / (nParticles * period); //0.7f
            // GLfloat rate = glm::mix(0.01f, 0.09f, this->randFloat()); //Расстояние(отстование,) между точками в чераячке
            // printf("i:%i; l: %f; n: %i; r: %f\n", i, length, nParticles, rate);
            // GLfloat rate = particleLifetime / (nParticles);//0.7f
            /* 
        1.Игры с rate и period:
            1.Сделать период в % соотношении
            2.Сделать rate случайный
            3.Сделать rate случайный для каждой частицы
        5.сделать ScaleY в зависимости от позиции: чем ближе к краю - тем шире, к центру - уже

        7.Применить другой алгоритм(старый)
         */
            // rate = glm::mix(_startTime, _startTime + particleLifetime, this->randFloat());
            // GLfloat const _startTime = 0.0f;
            GLfloat _startTime = glm::mix(1.0f, 2.0f, this->randFloat());
            // printf("n: %i\n", nParticles);

            sumX += x - x0;
            x0 = x;
            nextPosition = position;
            nextPosition.x += sumX * kx;
            GLfloat _time = _startTime;
            GLfloat _scaleX = mix(3.0f, 7.0f, this->randFloat());      //Сколько витков
            GLfloat _scaleY = mix(0.001f, 0.003f, this->randFloat());      //Сколько витков
            // GLfloat distance = abs(i-centr)/centr + 0.01; //(0.9 : 0.0) + 0.1
            // GLfloat _scaleY = 0.001f * distance; //Сжатие витков
            // _scaleY = mix(_scaleY - 0.002f, _scaleY + 0.002f, this->randFloat()); //Сжатие витков
            // GLfloat _scaleX = 7.0f;
            // GLfloat _scaleY = 0.0001f;

            GLint _inversion = getRandomNumber(0, 1);
            _inversion = _inversion == 0 ? -1 : 1;
            // _inversion *= -1;
            for (GLuint j = 0; j < nParticles; j++)
            {
                Pos.push_back(nextPosition);
                LifeTime.push_back(particleLifetime);
                StartTime.push_back(_time);
                _time += rate;
                ScaleX.push_back(_scaleX);
                ScaleY.push_back(_scaleY);
                Inversion.push_back((float)_inversion);
            }
        }
    }
    delete[] xs;
    printf("c: %zi\n", Pos.size());
}
Fire::Fire(vec2 position, GLfloat width, GLuint wWdth, GLuint wHeight)
    : circle(0.0f, 0.0f, 0.06f, 12)
{
    srand(static_cast<unsigned int>(time(0)));
    rand();
    this->initFire(position, width);
    this->initBuffers();
    this->timeNow = 0.0f;
    this->deltaT = 0.0f;
    this->drawBuf = 1;
    // printf("12\n");
}
void Fire::update(float t)
{
    this->deltaT = t - this->timeNow;
    this->timeNow = t;
    //printf("%f %f\n", t, deltaT);
}
void Fire::render(mat4 &projection, mat4 &view)
{
    // Update pass
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &updateSub);

    this->shader.setFloat("Time", timeNow);
    this->shader.setFloat("dT", deltaT);

    glEnable(GL_RASTERIZER_DISCARD);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);

    glBeginTransformFeedback(GL_POINTS);
    // glBeginTransformFeedback(GL_TRIANGLES);
    glBindVertexArray(VAOparticleArray[1 - drawBuf]);
    glDisableVertexAttribArray(7);
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);
    glVertexAttribDivisor(2, 0);
    glVertexAttribDivisor(3, 0);
    glVertexAttribDivisor(4, 0);
    glVertexAttribDivisor(5, 0);
    glVertexAttribDivisor(6, 0);

    glDrawArrays(GL_POINTS, 0, nParticles);
    glBindVertexArray(0);
    glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);

    // Render pass
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderSub);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4 MVP = projection * view;
    this->shader.setMat4("MVP", MVP);

    glBindVertexArray(VAOparticleArray[drawBuf]);
    glEnableVertexAttribArray(7);

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, circle.getNPoints(), nParticles);
    // glDrawTransformFeedback(GL_POINTS, feedback[drawBuf]);
    // glDrawTransformFeedback(GL_TRIANGLE_FAN, feedback[drawBuf]);

    // Swap buffers
    drawBuf = 1 - drawBuf;
}
// Рендеринг всех частиц
void Fire::Draw(mat4 &projection, mat4 &view)
{
    this->update(float(glfwGetTime()));
    this->render(projection, view);
    /*     for (GLuint i = 0; i < this->brushes.size(); i++)
    {
        this->brushes[i].Draw(projection, view);
        // this->brushes[i].Update();
    } */
    /*     for (BrushGenerator brush : this->brushes)
    {
        // brush.Update();
        brush.Draw(projection, view);
     } */
}

GLint Fire::getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    // Равномерно распределяем рандомное число в нашем диапазоне
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

GLfloat Fire::randFloat()
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    // Равномерно распределяем рандомное число в нашем диапазоне
    return static_cast<float>(rand() * fraction);
}
