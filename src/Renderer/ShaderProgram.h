//
// Created by Kostya on 06.10.2020.
//

#ifndef LEARNOPENGL_SHADERPROGRAM_H
#define LEARNOPENGL_SHADERPROGRAM_H

#include <glad/glad.h>

//#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace RenderEngine
{
    class ShaderProgram
    {
    public:
        unsigned int ID;

        ShaderProgram(){}
        // конструктор генерирует шейдер на лету
        ShaderProgram(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr){
            this->create(vertexPath, fragmentPath, geometryPath);
        }
        void create(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
        {
            // 1. получение исходного кода вершинного/фрагментного шейдера из переменной filePath
            std::string vertexCode;
            std::string fragmentCode;
            std::string geometryCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            std::ifstream gShaderFile;
            // убеждаемся, что объекты ifstream могут выбросить исключение:
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
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

                // Если дан путь к геометрическому шейдеру, то загружаем и его
                if (geometryPath != nullptr)
                {
                    gShaderFile.open(geometryPath);
                    std::stringstream gShaderStream;
                    gShaderStream << gShaderFile.rdbuf();
                    gShaderFile.close();
                    geometryCode = gShaderStream.str();
                }
            }
            catch (std::ifstream::failure &e)
            {
                std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << "-" << e.code() << std::endl;
                m_isCompiled = false;
                return;
            }
            // 2. компилируем шейдеры
            GLuint idVertex, idFragment, idGeometry;
            // vertex shader
            idVertex = createShader(GL_VERTEX_SHADER, vertexCode.c_str());
            if (!m_isCompiled)
            {
                glDeleteShader(idVertex);
                return;
            }

            // Фрагментный шейдер
            idFragment = createShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
            if (!m_isCompiled)
            {
                glDeleteShader(idFragment);
                return;
            }

            // Если был дан геометрический шейдер, то компилируем его
            if (geometryPath != nullptr)
            {
                idGeometry = createShader(GL_GEOMETRY_SHADER, geometryCode.c_str());
                if (!m_isCompiled)
                {
                    glDeleteShader(idGeometry);
                    return;
                }
            }

            // Шейдерная программа
            ID = glCreateProgram();
            glAttachShader(ID, idVertex);
            glAttachShader(ID, idFragment);
            if (geometryPath != nullptr)
                glAttachShader(ID, idGeometry);
            
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");
            // После того, как мы связали шейдеры с нашей программой, удаляем их, т.к. они больше не нужны
            glDeleteShader(idVertex);
            glDeleteShader(idFragment);
            if (geometryPath != nullptr)
                glDeleteShader(idGeometry);
        }

        ~ShaderProgram()
        {
            glDeleteProgram(ID);
        }

        // активация шейдера
        void use() const
        {
            glUseProgram(ID);
        }

        bool isCompiled() const
        {
            return m_isCompiled;
        }

        // полезные униформ-функции
        void setMat4(const std::string &name, glm::mat4 &value) const
        {
            //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
        }

        void setVec3(const std::string &name, glm::vec3 &value) const
        {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string &name, float x, float y, float z) const
        {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
        }

        void setVec4(const std::string &name, glm::vec4 &value) const
        {
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string &name, float x, float y, float z, float a) const
        {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, a);
        }

        void setBool(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }

        void setInt(const std::string &name, int value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        // ------------------------------------------------------------------------
        void setFloat(const std::string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }

    private:
        bool m_isCompiled = true;
        // полезные функции для проверки ошибок компиляции/связывания шейдеров.
        GLuint createShader(GLenum shaderType, const char *sourceCode)
        {
            GLuint idShader = glCreateShader(shaderType);
            glShaderSource(idShader, 1, &sourceCode, nullptr);
            glCompileShader(idShader);
            checkCompileErrors(idShader, shaderType == GL_VERTEX_SHADER ? "VERTEX" : shaderType == GL_FRAGMENT_SHADER ? "FRAGMENT" : "GEOMETRY");

            return idShader;
        }

        // полезные функции для проверки ошибок компиляции/связывания шейдеров.
        // ------------------------------------------------------------------------
        void checkCompileErrors(unsigned int shader, std::string type)
        {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    m_isCompiled = false;
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                              << infoLog
                              << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    m_isCompiled = false;
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                              << infoLog
                              << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
    };
} // namespace RenderEngine

#endif //LEARNOPENGL_SHADERPROGRAM_H
