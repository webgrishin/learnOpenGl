//
// Created by Kostya on 06.10.2020.
//
#include "ShaderProgram.h"

// конструктор генерирует шейдер на лету
// ------------------------------------------------------------------------
namespace RenderEngine {
	ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) {
		// 1. получение исходного кода вершинного/фрагментного шейдера из переменной filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// убеждаемся, что объекты ifstream могут выбросить исключение:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// открываем файлы
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// читаем содержимое файловых буферов
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// закрываем файлы
			vShaderFile.close();
			fShaderFile.close();
			// конвертируем в строковую переменную данные из потока
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e) {
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << "-" << e.code() << std::endl;
			m_isCompiled = false;
			return;
		}
		// 2. компилируем шейдеры
		GLuint idVertex, idFragment;
		// vertex shader
		idVertex = createShader(GL_VERTEX_SHADER, vertexCode.c_str());
		if (!m_isCompiled) {
			glDeleteShader(idVertex);
			return;
		}

		// Фрагментный шейдер
		idFragment = createShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
		if (!m_isCompiled) {
			glDeleteShader(idFragment);
			return;
		}

		// Шейдерная программа
		ID = glCreateProgram();
		glAttachShader(ID, idVertex);
		glAttachShader(ID, idFragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// После того, как мы связали шейдеры с нашей программой, удаляем их, т.к. они больше не нужны
		glDeleteShader(idVertex);
		glDeleteShader(idFragment);
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(ID);
	}

	// активация шейдера
	// ------------------------------------------------------------------------
	void ShaderProgram::use() const {
		glUseProgram(ID);
	}

	// полезные униформ-функции

			// ------------------------------------------------------------------------
	void ShaderProgram::setMat4(const std::string& name, glm::mat4 value) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	// ------------------------------------------------------------------------
	void ShaderProgram::setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	// ------------------------------------------------------------------------
	void ShaderProgram::setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	// ------------------------------------------------------------------------
	void ShaderProgram::setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	GLuint ShaderProgram::createShader(GLenum shaderType, const char* sourceCode) {
		GLuint idShader = glCreateShader(shaderType);
		glShaderSource(idShader, 1, &sourceCode, nullptr);
		glCompileShader(idShader);
		checkCompileErrors(idShader, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");

		return idShader;
	}

	// полезные функции для проверки ошибок компиляции/связывания шейдеров.
	// ------------------------------------------------------------------------
	void ShaderProgram::checkCompileErrors(unsigned int shader, std::string type) {
		int success;
		char infoLog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				m_isCompiled = false;
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
					<< "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				m_isCompiled = false;
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
					<< "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
}