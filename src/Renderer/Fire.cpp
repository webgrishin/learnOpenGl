#include "Fire.h"
void Fire::initBuffers()
{

    this->nParticles = 5;
    // Create VAO for each set of buffers
    glGenVertexArrays(2, VAOparticleArray);
    // Generate the buffers
    glGenBuffers(2, posBuf);    // position buffers
    glGenBuffers(2, startTime); // Start time buffers

    // Allocate space for all buffers
    int size = nParticles * 2 * sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    size = nParticles * sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    /*
    1.Исправить баг при запуске
    2.Разобраться почему плавает волос
    3.рассчитать количество частитц в зависимости от времени жизни и интервала. или наоборот: интервал в зависимости от количества и времениЖ
    4.Сделать затухание хвоста
    5.ввести ветер и ускорение
    6.сделать изменение цвета
    7.реализовать костёр
    8.Реализовать изображение частицы
    
     */

    GLfloat *time = new GLfloat[nParticles];
    GLfloat *pos = new GLfloat[nParticles*2];
    GLfloat t = 0.0f, rate = 0.25f;
    for (GLuint i = 0; i < this->nParticles; i++)
    {
        pos[2*i] = 0.0f;
        pos[2*i+1] = 0.0f;
        t+=rate;
        time[i] = t;
    }

    // Set up particle array 0
    glBindVertexArray(VAOparticleArray[0]);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * 2 * sizeof(GLfloat), pos, GL_DYNAMIC_COPY);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    delete [] pos;


    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(GLfloat), time, GL_DYNAMIC_COPY);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
    delete [] time;

    // Set up particle array 0
    glBindVertexArray(VAOparticleArray[1]);

    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Setup the feedback objects
    glGenTransformFeedbacks(2, feedback);

    // Transform feedback 0
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, startTime[0]);

    // Transform feedback 1
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, startTime[1]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    
    GLint value;
    glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &value);
    printf("MAX_TRANSFORM_FEEDBACK_BUFFERS = %d\n", value);


    this->shader.create("assets/shaders/point3.vs", "assets/shaders/point.fs");
    this->shader.use();
    GLfloat interval = 0.01f;
    this->shader.setFloat("ParticleLifetime", interval*345.0f);

    GLfloat scaleX = getRandomNumber(10, 90) / 10.0f;
    GLfloat scaleY = getRandomNumber(5, 30) / 100.0f;
    this->shader.setFloat("scaleX", 4.0f);
    this->shader.setFloat("scaleY", 0.0009f);

    this->renderSub = glGetSubroutineIndex(this->shader.ID, GL_VERTEX_SHADER, "render");
    this->updateSub = glGetSubroutineIndex(this->shader.ID, GL_VERTEX_SHADER, "update");
    
    


}
Fire::Fire(vec2 position, GLfloat width, GLuint wWdth, GLuint wHeight)
{
    srand(static_cast<unsigned int>(time(0)));
    rand();
    this->timeNow = 0.0f;
    this->deltaT = 0.0f;
    this->drawBuf = 1;
    this->initBuffers();
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
    m = M_PI / (2 * k);
    n = -m;
    l = m * 2;
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
            BrushGenerator brush = BrushGenerator(this->shader, nextPosition, length, scaleX, scaleY, inversion);
            this->brushes.push_back(brush);
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
void Fire::update( float t )
{
    this->deltaT = t - this->timeNow;
    this->timeNow = t;
    // printf("%f %f\n", t, deltaT);
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

    return;
    for (GLuint i = 0; i < this->brushes.size(); i++)
    {
        this->brushes[i].Draw(projection, view);
        // this->brushes[i].Update();
    }
    /*     for (BrushGenerator brush : this->brushes)
    {
        // brush.Update();
        brush.Draw(projection, view);
     } */
}

int Fire::getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    // Равномерно распределяем рандомное число в нашем диапазоне
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
