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
#include "stb_image.h"

/*
 * base example code for setting up-window and opengl context taken from site https://www.glfw.org/documentation.html
 * used glfw docs https://www.glfw.org/docs/latest/window_guide.html
 * */
//#define __APPLE__

const GLuint WINDOW_WIDTH = 800;
const GLuint WINDOW_HEIGHT = 600;

GLfloat lastX = WINDOW_WIDTH / 2.0f;
GLfloat lastY = WINDOW_HEIGHT / 2.0f;

bool firstMouse = true;

// тайминги
GLfloat deltaTime = 0.0f; // время между текущим кадром и последним кадром
GLfloat lastFrame = 0.0f;

RenderEngine::Camera camera;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
GLuint loadTexture(const char *path);

int main(void)
{
	std::string tt = FileSystem::getPath("src/assets/shaders/shader.vs");
	std::cout << "path " << tt << std::endl;
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGL())
	{
		std::cerr << "gladLoadGL failed!" << std::endl;
		return -1;
	}
	// конфигурирование глобального состояния OpenGL
	glEnable(GL_DEPTH_TEST);

	std::cout << "VideoCard: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGl version: " << glGetString(GL_VERSION) << std::endl;
	//    std::cout << "OpenGl: " << GLVersion.major << "." << GLVersion.minor << std::endl;

	/*OPENGL*/
	/*tutorial from site "LearnOpenGl" https://ravesli.com/uroki-po-opengl/
	 * and "SimpleCoding" https://www.youtube.com/playlist?list=PL6x9Hnsyqn2XU7vc8-oFLojbibK91fVd-*/
	// Компилирование нашей шейдерной программы

	//Загрузка файла относительно бинарника
	RenderEngine::ShaderProgram ourShader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
	if (!ourShader.isCompiled())
		return -1;

	//загрузка файла относительно проекта
	RenderEngine::ShaderProgram lampShader(FileSystem::getPath("src/assets/shaders/lamp.vs"), FileSystem::getPath("src/assets/shaders/lamp.fs"));
	if (!lampShader.isCompiled())
		return -1;

	GLfloat vertices[] = {
        // координаты          // нормали           // текстурные координаты
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };


    // координаты всех контейнеров
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // координаты точечных источников света
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };
	
	// 1. настраиваем VAO (и VBO) куба
	GLuint VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	//координатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);
	//атрибут вектора нормали
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//атрибут координат текустуры
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//2. настраиваем VAO света (VBO остается неизменным; вершины те же и для светового объекта, который также является 3D-кубом)
	GLuint lampVAO;
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);

	//загрузка текстур
	// -----------------------------------------------------------------------------
	GLuint diffuseMap = loadTexture("assets/textures/wooden_container_2.png");
	GLuint specularMap = loadTexture("assets/textures/container_2_specular.png");

	ourShader.use();
	ourShader.setInt("material.diffuse", 0);
	ourShader.setInt("material.specular", 1);

    glm::vec3 colorRed = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 colorGreen = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 colorBlue = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 colorYellow = glm::vec3(1.0f, 1.0f, 0.0f);
    glm::vec3 pointLightColors[] = {colorRed, colorGreen, colorBlue, colorYellow};

	#define NR_POINT_LIGHTS 4
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBindTexture(GL_TEXTURE_2D, texture);

		//Активируем шейдер
		ourShader.use();
		ourShader.setVec3("viewPos", camera.Position);
		// свойства материалов
		ourShader.setFloat("material.shininess", 32.0f);

        // направленный свет
        ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // прожектор (фонарик)
        ourShader.setVec3("spotLight.position", camera.Position);
        ourShader.setVec3("spotLight.direction", camera.Front);
        ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("spotLight.constant", 1.0f);
        ourShader.setFloat("spotLight.linear", 0.09);
        ourShader.setFloat("spotLight.quadratic", 0.032);
        ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// точечные источники света
		for (int i = 0; i < NR_POINT_LIGHTS; i++){
			char buffer[64];
			sprintf(buffer, "pointLights[%i].position", i);
			ourShader.setVec3(buffer, pointLightPositions[i]);
			sprintf(buffer, "pointLights[%i].diffuse", i);
			ourShader.setVec3(buffer, pointLightColors[i]);
			sprintf(buffer, "pointLights[%i].ambient", i);
			ourShader.setVec3(buffer, glm::vec3(0.05f));
			sprintf(buffer, "pointLights[%i].specular", i);
			ourShader.setVec3(buffer, glm::vec3(1.0f));
			sprintf(buffer, "pointLights[%i].constant", i);
			ourShader.setFloat(buffer, 1.0f);
			sprintf(buffer, "pointLights[%i].linear", i);
			ourShader.setFloat(buffer, 0.09f);
			sprintf(buffer, "pointLights[%i].qaudratic", i);
			ourShader.setFloat(buffer, 0.032f);
		}

		// преобразования Вида/Проекции
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// мировое преобразование
		glm::mat4 model = glm::mat4(1.0f);
		ourShader.setMat4("model", model);

		// рендерим ящик

		// связывание диффузной карты
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// связывание карты отраженного света
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(cubeVAO);
		for (GLuint i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// также отрисовываем объект лампы
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		// а теперь мы отрисовывает столько ламп, сколько у нас есть точечных источников света
		glBindVertexArray(lampVAO);
		for (GLuint i = 0; i < NR_POINT_LIGHTS; i++)
		{
			lampShader.setVec3("fragColor", pointLightColors[i]);
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // куб, меньшего размера
			lampShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	// Опционально: освобождаем все ресурсы, как только они выполнили своё предназначение
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lampVAO);
	glDeleteBuffers(1, &VBO);

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
// Функция для загрузки 2D-текстуры из файла
GLuint loadTexture(const char *path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	GLint width, height, nrComponents;
	GLubyte *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		std::cout << "Texture failed to load at path: " << path << std::endl;

	stbi_image_free(data);

	return textureID;
}