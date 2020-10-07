#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Renderer/ShaderProgram.h"

/*
 * base example code for setting up-window and opengl context taken from site https://www.glfw.org/documentation.html
 * used glfw docs https://www.glfw.org/docs/latest/window_guide.html
 * */
//#define __APPLE__
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()){
        std::cerr << "glfwInit failed!" << std::endl;
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
        std::cerr << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(!gladLoadGL()){
        std::cerr << "gladLoadGL failed!" << std::endl;
        return -1;
    }

    std::cout << "VideoCard: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGl version: " << glGetString(GL_VERSION) << std::endl;
//    std::cout << "OpenGl: " << GLVersion.major << "." << GLVersion.minor << std::endl;


/*OPENGL*/
/*tutorial from site "LearnOpenGl" https://ravesli.com/uroki-po-opengl/
 * and "SimpleCoding" https://www.youtube.com/playlist?list=PL6x9Hnsyqn2XU7vc8-oFLojbibK91fVd-*/
// Компилирование нашей шейдерной программы

    RenderEngine::ShaderProgram ourShader("C:\\learnOpenGl-CLion\\src\\assets\\shaders\\shader.vs", "C:\\learnOpenGl-CLion\\src\\assets\\shaders\\shader.fs");
    if (!ourShader.isCompiled())
        return -1;

    float vertices[] = {
            // координаты         // цвета
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // нижняя правая вершина
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // нижняя левая вершина
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // верхняя вершина
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // сначала связываем объект вершинного массива, затем связываем и устанавливаем вершинный буфер(ы), и затем конфигурируем вершинный атрибут(ы).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // координатные артибуты
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // цветовые атрибуты
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        glBindVertexArray(VAO); // поскольку у нас есть только один VАО, то нет необходимости связывать его каждый раз (но мы сделаем это, чтобы всё было немного организованнее)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    // Опционально: освобождаем все ресурсы, как только они выполнили своё предназначение
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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
