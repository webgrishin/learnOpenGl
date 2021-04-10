#include "Fire.h"

Fire::Fire(vec2 position, GLfloat width, GLuint wWdth, GLuint wHeight)
{
    this->shader.create("assets/shaders/point.vs", "assets/shaders/point.fs");
    // GLfloat onePxPart = 2.0f / (GLfloat)wWdth; //шаг смещения (количество частей в одном пикселе)
    GLfloat offset = width/100.0f; //100 язычков пламени
    // onePxPart *= offset;
    // GLfloat step = width;
    width += position.x;
    vec2 nextPosition = position;
    GLint inversion;
    // GLfloat interval; 
    GLfloat scaleX, scaleY, length;
    // GLuint k = 0;
    for (GLfloat i = position.x; i < width; i += offset)
    {
        // std::cout << i <<std::endl;
        nextPosition.x = i;
        inversion = getRandomNumber(0, 1);
        inversion = inversion == 0 ? -1 : 1;
        // interval = getRandomNumber(10, 20) / 1000.0f; //0.02f;
        scaleX = getRandomNumber(10, 50) / 10.0f;
        scaleY = getRandomNumber(1, 30) / 100.0f;
        // scaleY = getRandomNumber(10, 100) / 100.0f;
        length = getRandomNumber(5, 15) / 10.0f;
        BrushGenerator brush = BrushGenerator(this->shader, nextPosition, length, scaleX, scaleY, inversion);
        this->brushes.push_back(brush);
        // k++;
    }
    std::cout <<"ee " << this->brushes.size() << std::endl;
    // std::cout <<"ee " <<k << " "<< offset<<" " << this->brushes.size() << std::endl;
}

// Рендеринг всех частиц
void Fire::Draw(mat4 &projection, mat4 &view)
{
    for (GLuint i = 0; i < this->brushes.size(); i++){
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
