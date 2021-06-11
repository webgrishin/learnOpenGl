#include "Fire.h"
void Fire::initBuffers()
{
    GLuint aPosBuf[2], aStartTime[2], aInitPos, aParticleLifetime, aScaleX, aScaleY, aInversion;
    this->nParticles = Pos.size();
    // Create VAO for each set of buffers
    glGenVertexArrays(2, VAOparticleArray);
    // Generate the buffers
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
    // glBindBuffer(GL_ARRAY_BUFFER, initPos[0]);
    // glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    // glBindBuffer(GL_ARRAY_BUFFER, initPos[1]);
    // glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
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
    // glBufferData(GL_ARRAY_BUFFER, size*2, &Pos[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, aParticleLifetime);
    // glBufferData(GL_ARRAY_BUFFER, size, &LifeTime[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, aScaleX);
    // glBufferData(GL_ARRAY_BUFFER, size, &ScaleX[0], GL_STATIC_DRAW);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, aScaleY);
    // glBufferData(GL_ARRAY_BUFFER, size, &ScaleY[0], GL_STATIC_DRAW);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(5);

    glBindBuffer(GL_ARRAY_BUFFER, aInversion);
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(6);
    // glBindBuffer(GL_ARRAY_BUFFER, initPos[1]);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    // glEnableVertexAttribArray(2);

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
void Fire::initFire(vec2 position, GLfloat width){

    GLuint const c = 500; //количество язычков пламени

    GLfloat k = 1.0f;
    GLfloat const PI = glm::pi<float>();

    GLfloat m = PI / (2.0f * k);
    GLfloat n = -m;
    GLfloat l = m * 2;

    GLfloat x, y, y0, dy, nx;
    GLfloat step = l / c;
    GLfloat *xs = new GLfloat[c];
    GLuint i = 0;
    y0 = 0;
    nx = n;
    for (x = n; x < m && i < c; x += step, i++)
    {
        y = cos(x * k);
        dy = y - y0;
        y0 = y;
        xs[i] = nx + (abs(dy) / 2.0f) * l;
        nx = xs[i];
        // printf("i: %i, y: %f\n", i, xs[i]);
    }

    GLfloat scaleY = 1.0f / l;
    GLfloat kl = width * scaleY;
    GLfloat kx = width / l;
    vec2 nextPosition = position;
    GLfloat x0 = xs[0], sumX = 0.0f;
    // GLchar _inversion = 1;
    for (i = 0; i < c; i++)
    {
        x = xs[i];
        y = cos(x * k) * kl;
        GLfloat length = glm::mix(0.6f * y, y, this->randFloat());
        // printf("i: %i; x: %f; y: %f, l:%f\n", i, x, y, length);

        GLfloat const _lifeTime = 3.0f; //тоже влияет на высоту пламени
        GLuint const MaxParticle = 200;
        particleLifetime = length * _lifeTime;
        // nParticles = particleLifetime * MaxParticle / _lifeTime;
        nParticles = length * MaxParticle;
        
        GLfloat period = glm::mix(0.6f, 4.0f, this->randFloat());
        GLfloat rate = particleLifetime / (nParticles * period);//0.7f
        /* 
        1.Игры с rate и period:
            1.Сделать период в % соотношении
            2.Сделать rate случайный
            3.Сделать rate случайный для каждой частицы
        2.Сделать удаление векторов
        3.Сделать изменение диаметра частиц
        4.Ввести ветер
        5.сделать ScaleY в зависимости от позиции: чем ближе к краю - тем шире, к центру - уже
        6.Применить форму для частицы

        7.Применить другой алгоритм(старый)
         */
        // rate = glm::mix(_startTime, _startTime + particleLifetime, this->randFloat());
        GLfloat const _startTime = 1.0f;
        // printf("n: %i\n", nParticles);

        sumX += x - x0;
        x0 = x;
        nextPosition = position;
        nextPosition.x += sumX * kx;
        GLfloat _time = _startTime;
        GLfloat _scaleX = mix(5.0f, 10.0f, this->randFloat());
        GLfloat _scaleY = mix(0.0001f, 0.0008f, this->randFloat());
        // GLfloat _scaleX = 10.0f;
        // GLfloat _scaleY = 0.0009f;
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
    delete[] xs;
    printf("c: %zi, %zi\n", Pos.size(), Inversion.size());
}
Fire::Fire(vec2 position, GLfloat width, GLuint wWdth, GLuint wHeight)
:circle(0.0f, 0.0f, 0.0f, 0.08f, 12)
{
    // MCircle mcircle(0.0f, 0.0f, 0.0f, 0.08f, (uint)12);
    // MCircle circle = MCircle();
    srand(static_cast<unsigned int>(time(0)));
    rand();
    this->initFire(position, width);
    this->initBuffers();
    this->timeNow = 0.0f;
    this->deltaT = 0.0f;
    this->drawBuf = 1;
    // this->update(float(glfwGetTime()));
    // printf("12\n");
    return;
    this->shader.create("assets/shaders/point3.vs", "assets/shaders/point.fs");

    // this->shader.create("assets/shaders/point2.vs", "assets/shaders/point.fs", "assets/shaders/circleGeometry.gs");

    // GLfloat onePxPart = 2.0f / (GLfloat)wWdth; //шаг смещения (количество частей в одном пикселе)
    vec2 nextPosition = position;
    GLint inversion, i;
    // GLfloat interval;
    GLfloat scaleX, scaleY, length;
    // GLuint k = 0;
    srand(static_cast<unsigned int>(time(0)));
    rand();
    GLfloat m, n, k, l, step, x, nx;
    GLfloat const c = 200.0f; //100 язычков пламени

    k = 2.0f;
    // m = PI / (2.0f * k);
    n = -m;
    l = m * 2.0f;
    step = l / c;
    scaleY = 1.0f / l;

    GLfloat xs[(uint)(c - 1)];

    GLfloat offset = width / c;
    // onePxPart *= offset;
    // GLfloat step = width;
    // width += position.x;

    GLfloat y, y0, dy, yMax, kl;
    // kl = 2.0f;
    kl = width * scaleY;
    i = 0;
    y0 = 0;
    // printf("n: %f, l: %f, step: %f; offset:%f\n", n, l, step, offset);

    nx = n;
    for (x = n + step; x < m - step; x += step, i++)
    {
        y = cos(x * k);
        dy = y - y0;
        y0 = y;
        xs[i] = nx + abs(dy) / 2.0f * l;
        nx = xs[i];
        // printf("i: %i, y: %f\n", i, xs[i]);
        // i++;
    }

    GLfloat kx = width / l;

    for (i = 0; i < sizeof(xs) / sizeof(xs[0]); i++)
    {
        x = xs[i];
        y = cos(x * k);
        yMax = y * kl;
        length = getRandomNumber((int)(0.6 * yMax * 1000.0f), (int)(yMax * 1000.0f)) / 1000.0f;
        // length = yMax;

        if (length > 0.4f)
        {
            nextPosition.x = kx * x;

            inversion = getRandomNumber(0, 1);
            inversion = inversion == 0 ? -1 : 1;
            // interval = getRandomNumber(10, 20) / 1000.0f; //0.02f;
            scaleX = getRandomNumber(10, 90) / 10.0f;
            // scaleY = 1.0f;
            scaleY = getRandomNumber(5, 30) / 100.0f;
            // BrushGenerator brush = BrushGenerator(this->shader, nextPosition, length, scaleX, scaleY, inversion);
            // this->brushes.push_back(brush);
            // return;
        }
    }

    /*    return;

    i=position.x;
    for (GLfloat x = n+step; x < m; x += step){
        y = cos(x*k);
        // y = abs(x*k)*-1.0f+1.0f;
        yMax = y*kl;
        length = getRandomNumber((int)(0.6 * yMax * 1000.0f), (int)(yMax * 1000.0f)) / 1000.0f;
        // length = yMax;

        i += offset;
        nextPosition.x = i;
        if (length > 0.9f)
        {
     //        {
                dy = y - y0;
                y0 = y;
                position.x += abs(dy)/2.0f * width;
                nextPosition.x = position.x;
       //     }
            // printf("i:%i; y: %f; yMax: %f; x:%f; ody:%f; dx:%f; xn:%f; l: %f\n", i++, y, yMax, nextPosition.x, dy, dy*width, position.x, length);
            inversion = getRandomNumber(0, 1);
            inversion = inversion == 0 ? -1 : 1;
            // interval = getRandomNumber(10, 20) / 1000.0f; //0.02f;
            scaleX = getRandomNumber(10, 50) / 10.0f;
            // scaleY = 1.0f;
            scaleY = getRandomNumber(5, 30) / 100.0f;
            BrushGenerator brush = BrushGenerator(this->shader, nextPosition, length, scaleX, scaleY, inversion);
            this->brushes.push_back(brush);
        }
    }*/
    // std::cout <<"ee " << this->brushes.size() << std::endl;
    // std::cout <<"ee " <<k << " "<< offset<<" " << this->brushes.size() << std::endl;
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
    glBindVertexArray(VAOparticleArray[1 - drawBuf]);
    glDrawArrays(GL_POINTS, 0, nParticles);
    glEndTransformFeedback();

    glDisable(GL_RASTERIZER_DISCARD);

    // Render pass
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderSub);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4 MVP = projection * view;
    this->shader.setMat4("MVP", MVP);

    glBindVertexArray(VAOparticleArray[drawBuf]);
    glDrawTransformFeedback(GL_POINTS, feedback[drawBuf]);

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
