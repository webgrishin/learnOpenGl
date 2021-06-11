#ifndef MCIRCLE_H
#define MCIRCLE_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/gtc/constants.hpp>

using namespace glm;

class MCircle
{
private:
    GLuint nPointsOfCircle, VAO, VBO;
    std::vector<GLfloat> vertices;
    void deleteBuffers();

public:
    GLuint getVao() const
    {
        return VAO;
    };
    GLuint getNPoints() const
    {
        return nPointsOfCircle;
    };
    // Circle(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f, GLfloat radius = 0.1f, GLuint numberOfSides = 6);
    MCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLuint numberOfSides);
    ~MCircle();
};

#endif