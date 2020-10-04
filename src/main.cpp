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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(1,1,0,1);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

