#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/geometric.hpp>
//#include <glm/vec3.hpp> // glm::vec3
//#include <glm/vec4.hpp> // glm::vec4
//#include <glm/mat4x4.hpp> // glm::mat4
//#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
//#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
//#include <glm/ext/scalar_constants.hpp> // glm::pi
#include "filesystem.h"

#include <iostream>
#include "Renderer/ShaderProgram.h"
#include "Renderer/Camera.h"
#include "Renderer/Model.h"
#include "Renderer/StaticModel.h"
#include "stb_image.h"
// #include "Renderer/particle_generator.h"
#include "Renderer/Fire.h"

/*
 * base example code for setting up-window and opengl context taken from site https://www.glfw.org/documentation.html
 * used glfw docs https://www.glfw.org/docs/latest/window_guide.html
 * */
//#define __APPLE__

// настройки
const GLuint WINDOW_WIDTH = 800;
const GLuint WINDOW_HEIGHT = 600;

// камера
RenderEngine::Camera camera;
GLfloat lastX = WINDOW_WIDTH / 2.0f;
GLfloat lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

struct StateLights
{
	bool onDirLight = true;
	bool onSpotLight = false;
	bool onPointsLight = false;
};
StateLights stateLight;

// тайминги
GLfloat deltaTime = 0.0f; // время между текущим кадром и последним кадром
GLfloat lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
	{
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

	// glfw создание окна
	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Learn OpenGL", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "glfwCreateWindow failed!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGL())
	{
		std::cerr << "gladLoadGL failed!" << std::endl;
		return -1;
	}

	std::cout << "VideoCard: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGl version: " << glGetString(GL_VERSION) << std::endl;
	//    std::cout << "OpenGl: " << GLVersion.major << "." << GLVersion.minor << std::endl;


	// конфигурирование глобального состояния OpenGL
    // glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);//default. Если тест глубины всегда проходит успешно (GL_ALWAYS glDisable(GL_DEPTH_TEST))
    // glEnable(GL_STENCIL_TEST);
	// glStencilMask(0xFF); // default
	glStencilFunc(GL_ALWAYS, 0, 0xFF); //default
    // glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //default
    // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// компилирование нашей шейдерной программы
	//Загрузка файла относительно бинарника
	RenderEngine::ShaderProgram ourShader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
	if (!ourShader.isCompiled())
		return -1;

	RenderEngine::ShaderProgram floorShader("assets/shaders/shader.vs", "assets/shaders/floor.fs");
	if (!ourShader.isCompiled())
		return -1;
		
	RenderEngine::ShaderProgram lampShader("assets/shaders/lamp.vs", "assets/shaders/lamp.fs");
	if (!lampShader.isCompiled())
		return -1;

	RenderEngine::ShaderProgram stancilShader("assets/shaders/lamp.vs", "assets/shaders/stencil_single_color.fs");
	if (!stancilShader.isCompiled())
		return -1;

	// загрузка моделей
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	// говорим stb_image.h чтобы он перевернул загруженные текстуры относительно y-оси (до загрузки модели).
	stbi_set_flip_vertically_on_load(true);
	// Model ourModel(window, "assets/objects/backpack/backpack.obj");
	// Model ourModel(window, "assets/objects/fireplace_bricks/13111_fireplacebricks_v2_l2.obj");
	// Model ourModel(window, "assets/objects/fireplace/fireplace.obj");
	//Активируем шейдер
	RenderEngine::StaticModel staticModel;

	//режим отсечения граней
	// glEnable(GL_CULL_FACE);
	// ParticleGenerator particleGen;
	Fire fire(vec2(-0.5f, -1.0f), 0.3f, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	while (!glfwWindowShouldClose(window))
	{
		// логическая часть работы со временем для каждого кадра
		// --------------------
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// обработка ввода
		processInput(window);

		// рендеринг
		// ------
		// glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		// glStencilMask(0x00); 

/* 		ourShader.use();
		staticModel.OnDirLight(ourShader);
		staticModel.OnPointsLight(ourShader);
		staticModel.OnSpotLight(ourShader, camera);
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setBool("stateLight.onDirLight", stateLight.onDirLight);
		ourShader.setBool("stateLight.onSpotLight", stateLight.onSpotLight);
		ourShader.setBool("stateLight.onPointsLight", stateLight.onPointsLight); */

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

/* 		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view); */

/* 
		// преобразования Вида/Проекции
		// Рендерим гитару
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f)); // Ставим на пол
		model = glm::scale(model, glm::vec3(0.65f));	 // объект слишком большой для нашей сцены, поэтому немного уменьшим его

		// model = glm::translate(model, glm::vec3(0.0f, -0.9f, 0.0f)); // Ставим на пол
		// model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		// model = glm::scale(model, glm::vec3(0.01f));	 // объект слишком большой для нашей сцены, поэтому немного уменьшим его
		ourShader.setMat4("model", model);
		// ourModel.Draw(ourShader);
 */
		// staticModel.Draw(projection, view);

		fire.Draw(projection, view);
		// particleGen.Draw(projection, view);
		// particleGen.Update4();

		// Рендерим пол
/* 		floorShader.use();
		staticModel.OnDirLight(floorShader);
		staticModel.OnPointsLight(floorShader);
		staticModel.OnSpotLight(floorShader, camera);
		floorShader.setVec3("viewPos", camera.Position);
		floorShader.setBool("stateLight.onDirLight", stateLight.onDirLight);
		floorShader.setBool("stateLight.onSpotLight", stateLight.onSpotLight);
		floorShader.setBool("stateLight.onPointsLight", stateLight.onPointsLight);
		floorShader.setMat4("projection", projection);
		floorShader.setMat4("view", view);
		staticModel.FloorDraw(floorShader);

		// Рендерим лампочки
		if (stateLight.onPointsLight){
			staticModel.LampsDraw(lampShader, projection, view, stancilShader);
		}
 */
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		stateLight.onDirLight = !stateLight.onDirLight;
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		stateLight.onPointsLight = !stateLight.onPointsLight;
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		stateLight.onSpotLight = !stateLight.onSpotLight;
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или опер. системой), вызывается данная функция
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// убеждаемся, что вьюпорт соответствует новым размерам окна; обратите внимание,
	// что ширина и высота будут значительно больше, чем указано на retina -дисплеях.
	glViewport(0, 0, width, height);
}

// glfw: всякий раз, когда перемещается мышь, вызывается данная callback-функция
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // перевернуто, так как Y-координаты идут снизу вверх

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: всякий раз, когда прокручивается колесико мыши, вызывается данная callback-функция
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}