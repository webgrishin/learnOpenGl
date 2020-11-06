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

glm::vec3 lampPos(1.2f, 1.0f, 2.0f);

bool firstMouse = true;

// тайминги
GLfloat deltaTime = 0.0f;	// время между текущим кадром и последним кадром
GLfloat lastFrame = 0.0f;

RenderEngine::Camera camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


int main(void)
{
	/* Initialize the library */
	if (!glfwInit()) {
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

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGL()) {
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

	RenderEngine::ShaderProgram ourShader("C:\\learnOpenGl-VS\\src\\assets\\shaders\\shader.vs", "C:\\learnOpenGl-VS\\src\\assets\\shaders\\shader.fs");
	if (!ourShader.isCompiled())
		return -1;
		
	RenderEngine::ShaderProgram lampShader("C:\\learnOpenGl-VS\\src\\assets\\shaders\\lamp.vs", "C:\\learnOpenGl-VS\\src\\assets\\shaders\\lamp.fs");
	if (!lampShader.isCompiled())
		return -1;

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

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

	//unsigned int indices[] = {
	//	0, 1, 3, // первый треугольник
	//	1, 2, 3  // второй треугольник
	//};

	GLuint VBO, VAO/*, EBO*/;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	//координатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//атрибут вектора нормали
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//2. настраиваем VAO света (VBO остается неизменным; вершины те же и для светового объекта, который также является 3D-кубом)
	GLuint lampVAO;
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);

	//нам нужно только привязаться к VBO (чтобы связать его с glVertexAttribPointer), не нужно заполнять его; данные VBO уже содержат все, что нам нужно(они уже привязаны, но мы делаем это снова в образовательных целях) 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/*
	// атрибуты текстурных координат
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// загрузка и создание текстуры
	// -------------------------
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // все последующие GL_TEXTURE_2D-операции теперь будут влиять на данный текстурный объект
	// установка параметров наложения текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// установка метода наложения текстуры GL_REPEAT (стандартный метод наложения)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// установка параметров фильтрации текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// загрузка изображения, создание текстуры и генерирование mipmap-уровней
	GLint width, height, nrChannels;
	unsigned char* data = stbi_load("C:\\learnOpenGl-VS\\src\\assets\\textures\\wooden_container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	*/

	glEnable(GL_DEPTH_TEST);

	ourShader.use();
	//ourShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	//ourShader.setVec3("lampColor", glm::vec3(1.0f, 1.0f, 1.0f));

	ourShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	ourShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
	ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	ourShader.setFloat("material.shininess", 32.0f);

	//ourShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	//ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // немного затемним рассеянный свет
	//ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	//GLfloat lengthLampPos = glm::length(lampPos);
	//GLfloat lampPosY = lampPos.y;
	while (!glfwWindowShouldClose(window))
	{
		// логическая часть работы со временем для каждого кадра
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// рендеринг
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBindTexture(GL_TEXTURE_2D, texture);

		//Активируем шейдер
		ourShader.use();
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setVec3("light.position", lampPos);
		
		// свойства света
		glm::vec3 lightColor(1.0f);
		//lightColor.x = sin(lastFrame * 2.0f);
		//lightColor.y = sin(lastFrame * 0.7f);
		//lightColor.z = sin(lastFrame * 1.3f);
		//ourShader.setVec3("lampColor", lightColor);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.4f);
		ourShader.setVec3("light.ambient", ambientColor);
		ourShader.setVec3("light.diffuse", diffuseColor);
		ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// свойства материалов
		ourShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		ourShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		ourShader.setFloat("material.shininess", 32.0f);


		// передаем шейдеру матрицу проекции(поскольку проекционная матрица редко меняется, нет необходимости делать это для каждого кадра)
		// -----------------------------------------------------------------------------------------------------------
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// создаем преобразование камеры/вида
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);


		// рендерим ящик
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glm::mat4 model;
		for (unsigned int i = 0; i < 10; i++)
		{
			// вычисляем матрицу модели для каждого объекта и передаём ее в шейдер до отрисовки
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			/*
			float angle = 20.0f * i;
			if (i % 3 == 0) // каждую третью итерацию (включая первую) мы устанавливаем угол, используя функцию времени из библиотеки GLFW
				angle = glfwGetTime() * 5.0f * (i + 1.0);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
			*/
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// также отрисовываем наш объект-"лампочку"
		lampShader.use();
		model = glm::mat4(1.0f);
		//lampPos.x = sin(lastFrame * 0.5) * lengthLampPos;
		//lampPos.z = cos(lastFrame * 0.5) * lengthLampPos;
		//lampPos.y = lampPosY + (sin(lastFrame * 3) * 0.7);

		/*lampPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lampPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		*/

		model = glm::translate(model, lampPos);
		model = glm::scale(model, glm::vec3(0.2f)); // куб, меньшего размера
		lampShader.setMat4("model", model);
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		glBindVertexArray(lampVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}
	// Опционально: освобождаем все ресурсы, как только они выполнили своё предназначение
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	//glDeleteTextures(1, &texture);

	glfwTerminate();
	return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
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
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// убеждаемся, что вьюпорт соответствует новым размерам окна; обратите внимание,
	// что ширина и высота будут значительно больше, чем указано на retina -дисплеях.
	glViewport(0, 0, width, height);
}

// glfw: всякий раз, когда перемещается мышь, вызывается данная callback-функция
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // перевернуто, так как Y-координаты идут снизу вверх

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: всякий раз, когда прокручивается колесико мыши, вызывается данная callback-функция
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
