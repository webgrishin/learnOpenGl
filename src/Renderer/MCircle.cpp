
#include "MCircle.h"

// Circle::Circle(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f, GLfloat radius = 0.1f, GLuint numberOfSides = 6)
MCircle::MCircle(GLfloat x, GLfloat y, GLfloat radius, GLuint numberOfSides)
{
    nPointsOfCircle = numberOfSides + 2;

    GLfloat const twicePi = glm::two_pi<float>();

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(1.0f); //Прозрачность центра круга
    GLfloat rx, ry;

    for (GLuint i = 1; i < nPointsOfCircle; i++)
    {
        rx = x + (radius * cos(i * twicePi / numberOfSides));
        ry = y + (radius * sin(i * twicePi / numberOfSides));
        // printf("i:%i; x:%f; y:%f\n", i, rx, ry);
        vertices.push_back(rx);
        vertices.push_back(ry);
        vertices.push_back(0.1f); //Прозрачность контура круга
    }

// float vertices[] = { 
//         0.0f, 0.0f, 0.0f,  
//         -0.4f, 0.4f, 0.0f,  
//         0.4f, 0.4f, 0.0f,  
//         0.8f, 0.0f, 0.0f,  
//         0.4f, -0.4f, 0.0f,  
//         -0.4f, -0.4f, 0.0f,  
//         -0.8f, 0.0f, 0.0f,  
//         -0.4f, 0.4f, 0.0f,  
// };

    /* glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glBindVertexArray(0); */
};

void MCircle::deleteBuffers() {
    if( VAO != 0 ) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    if( VBO != 0 ) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    // if (vertices.size()>0){
        // for (int i = 0; i < vertices.size(); i++)
        // {
            // delete &vertices[i];
            // delete vertices[i];
        // }
        // vertices.clear();
    // }
}
MCircle::~MCircle()
{
    // deleteBuffers();
}