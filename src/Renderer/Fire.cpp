#include "Fire.h"

Fire::Fire(vec2 position, GLfloat width, GLuint wWdth, GLuint wHeight)
{
    this->shader.create("assets/shaders/point.vs", "assets/shaders/point.fs");
    // GLfloat onePxPart = 2.0f / (GLfloat)wWdth; //шаг смещения (количество частей в одном пикселе)
    vec2 nextPosition = position;
    GLint inversion, i;
    // GLfloat interval; 
    GLfloat scaleX, scaleY, length;
    // GLuint k = 0;
    srand(static_cast<unsigned int>(time(0)));
    rand();
    GLfloat m, n, k, l, step, x, nx;
    GLfloat const c = 100.0f; //100 язычков пламени

    k = 3.0f;
    m = M_PI/(2*k);
    n = -m;
    l = m*2;
    step = l/c;
    scaleY = 1.0f/l;

    GLfloat xs[(uint)(c-1)];

    GLfloat offset = width/c;
    // onePxPart *= offset;
    // GLfloat step = width;
    // width += position.x;

    GLfloat y, y0, dy, yMax, kl;
    // kl = 2.0f;
    kl = width * scaleY;
    i = 0;
    y0 = 0;
    // GLfloat i=position.x;
    printf("n: %f, l: %f, step: %f; offset:%f\n", n, l, step, offset);

    nx= n;
    for (x = n+step; x < m-step; x += step, i++){
        y = cos(x*k);
        dy = y - y0;
        y0 = y;
        xs[i] = nx + abs(dy) / 2.0f * l;
        nx = xs[i];
        printf("i: %i, y: %f\n", i, xs[i]);
        // i++;
    }

    GLfloat kx = width / l;

    for (i=0; i < sizeof(xs)/sizeof(xs[0]); i++){
        x = xs[i];
        y = cos(x*k);
        yMax = y*kl;
        length = getRandomNumber((int)(0.6 * yMax * 1000.0f), (int)(yMax * 1000.0f)) / 1000.0f;
        // length = yMax;

        if (length > 0.4f)
        {
            nextPosition.x = kx * x;

            inversion = getRandomNumber(0, 1);
            inversion = inversion == 0 ? -1 : 1;
            // interval = getRandomNumber(10, 20) / 1000.0f; //0.02f;
            scaleX = getRandomNumber(10, 50) / 10.0f;
            // scaleY = 1.0f;
            scaleY = getRandomNumber(5, 30) / 100.0f;
            BrushGenerator brush = BrushGenerator(this->shader, nextPosition, length, scaleX, scaleY, inversion);
            this->brushes.push_back(brush);
        }
    }

    return;

    for (GLfloat x = n+step; x < m; x += step){
        y = cos(x*k);
        // y = abs(x*k)*-1.0f+1.0f;
        yMax = y*kl;
        length = getRandomNumber((int)(0.6 * yMax * 1000.0f), (int)(yMax * 1000.0f)) / 1000.0f;
        // length = yMax;

        // i += offset;
        // nextPosition.x = i;
        if (length > 0.4f)
        {
            {
                dy = y - y0;
                y0 = y;
                position.x += abs(dy)/2.0f * width;
                nextPosition.x = position.x;
            }
            printf("i:%i; y: %f; yMax: %f; x:%f; ody:%f; dx:%f; xn:%f; l: %f\n", i++, y, yMax, nextPosition.x, dy, dy*width, position.x, length);
            inversion = getRandomNumber(0, 1);
            inversion = inversion == 0 ? -1 : 1;
            // interval = getRandomNumber(10, 20) / 1000.0f; //0.02f;
            scaleX = getRandomNumber(10, 50) / 10.0f;
            // scaleY = 1.0f;
            scaleY = getRandomNumber(5, 30) / 100.0f;
            BrushGenerator brush = BrushGenerator(this->shader, nextPosition, length, scaleX, scaleY, inversion);
            this->brushes.push_back(brush);
        }
    }
    // std::cout <<"ee " << this->brushes.size() << std::endl;
    // std::cout <<"ee " <<k << " "<< offset<<" " << this->brushes.size() << std::endl;
}

// Рендеринг всех частиц
void Fire::Draw(mat4 &projection, mat4 &view)
{
    for (GLuint i = 0; i < this->brushes.size(); i++)
    {
        this->brushes[i].Draw(projection, view);
        this->brushes[i].Update();
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
