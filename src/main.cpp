#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using std::cout;
using std::endl;

/*
 * base example code for setting up-window and opengl context taken from site https://www.glfw.org/documentation.html
 * used glfw docs https://www.glfw.org/docs/latest/window_guide.html
 * */
//#define __APPLE__
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

const char* vertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";
const char* vertexShaderWithColor = R"(
#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
out vec3 color;
void main()
{
    color = vColor;
    gl_Position = vec4(vPos, 1.0);
}
)";

const char* fragmentShader = R"(
#version 330 core
in vec3 color;
out vec4 fragColor;
void main()
{
    fragColor = vec4(color, 1.0f);
}
)";
const char* fragmentShaderOrange = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()){
        cout << "glfwInit failed!" << endl;
        return -1;
    }

    /*Create configuration glfw*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Learn OpenGL", nullptr, nullptr);
    if (!window)
    {
        cout << "glfwCreateWindow failed!" << endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(!gladLoadGL()){
        cout << "gladLoadGL failed!" << endl;
        return -1;
    }

    cout << "VideoCard: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGl version: " << glGetString(GL_VERSION) << endl;
//    cout << "OpenGl: " << GLVersion.major << "." << GLVersion.minor << endl;


/*OPENGL*/
/*tutorial from site "LearnOpenGl" https://ravesli.com/uroki-po-opengl/
 * and "SimpleCoding" https://www.youtube.com/playlist?list=PL6x9Hnsyqn2XU7vc8-oFLojbibK91fVd-*/
// Компилирование нашей шейдерной программы

    // Вершинный шейдер
    int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertexShader, NULL);
    glCompileShader(vShader);

    // Вершинный шейдер с цветом
    int vShaderWithColor = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShaderWithColor, 1, &vertexShaderWithColor, NULL);
    glCompileShader(vShaderWithColor);

    // Фрагментный шейдер Orange
    int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragmentShader, NULL);
    glCompileShader(fShader);

    // Фрагментный шейдер Orange
    int fSOrange = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fSOrange, 1, &fragmentShaderOrange, NULL);
    glCompileShader(fSOrange);

    // Связывание шейдеров
    int shaderProgramFillOrange = glCreateProgram();
    glAttachShader(shaderProgramFillOrange, vShader);
    glAttachShader(shaderProgramFillOrange, fSOrange);
    glLinkProgram(shaderProgramFillOrange);

    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShaderWithColor);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vShader);
    glDeleteShader(vShaderWithColor);
    glDeleteShader(fSOrange);

    // Указывание вершин (и буферов) и настройка вершинных атрибутов
    float vTriangle0[] = {
            -1.0f, -1.0f, 0.0f, // левая вершина
            0.0f, -1.0f, 0.0f, // правая вершина
            -0.5f,  0.0f, 0.0f,  // верхняя вершина
    };
    float vTriangle1[] = {
            0.0f, -1.0f, 0.0f, // левая вершина
            1.0f, -1.0f, 0.0f, // правая вершина
            0.5f,  0.0f, 0.0f  // верхняя вершина
    };

    float vRectangle[] = {
            -0.5f, 0.5f, 0.0f, // left top
            -0.5f, 0.0f, 0.0f, // left down
            0.5f,  0.0f, 0.0f,  // right down
            0.5f, 0.5f, 0.0f, // right up
    };
    unsigned int indices[] = {
            0, 1, 2,   // первый треугольник
            2, 3, 0    // второй треугольник
    };
    GLfloat colors[]={
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
    };

    unsigned int VBOs[4], VAOs[3], EBO;
    glGenVertexArrays(3, VAOs);
    glGenBuffers(4, VBOs);
    glGenBuffers(1, &EBO);

    //set first triangle
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vTriangle0), vTriangle0, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //set second triangle
    glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vTriangle1), vTriangle1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    //set Rectangle
    glBindVertexArray(VAOs[2]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vRectangle), vRectangle, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Обратите внимание, что данное действие разрешено, вызов glVertexAttribPointer() зарегистрировал VBO как привязанный вершинный буферный объект для вершинного атрибута, так что после этого мы можем спокойно выполнить отвязку
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // помните: не отвязывайте EBO, пока VАО активен, поскольку связанного объект буфера элемента хранится в VАО; сохраняйте привязку EBO.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Вы можете отменить привязку VАО после этого, чтобы другие вызовы VАО случайно не изменили этот VAO (но подобное довольно редко случается)
    // Модификация других VAO требует вызов glBindVertexArray(), поэтому мы обычно не снимаем привязку VAO (или VBO), когда это не требуется напрямую
    glBindVertexArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw rectangle
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUseProgram(shaderProgramFillOrange);
        glBindVertexArray(VAOs[2]); // поскольку у нас есть только один VАО, то нет необходимости связывать его каждый раз (но мы сделаем это, чтобы всё было немного организованнее)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //draw first triangle
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(VAOs[0]); // поскольку у нас есть только один VАО, то нет необходимости связывать его каждый раз (но мы сделаем это, чтобы всё было немного организованнее)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //draw second triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[1]); // поскольку у нас есть только один VАО, то нет необходимости связывать его каждый раз (но мы сделаем это, чтобы всё было немного организованнее)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // не нужно каждый раз его отвязывать

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    // Опционально: освобождаем все ресурсы, как только они выполнили своё предназначение
    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(4, VBOs);
    glDeleteBuffers(1, &EBO);

    glDeleteProgram(shaderProgramFillOrange);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

/*change size viewport(drawing, content) area of the window */
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
/*callback handler keyboard event - close window on key press escape*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
