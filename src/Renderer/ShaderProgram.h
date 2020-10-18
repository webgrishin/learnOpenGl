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
namespace RenderEngine {
    class ShaderProgram {
    public:
        unsigned int ID;

        // конструктор генерирует шейдер на лету
        // ------------------------------------------------------------------------
        ShaderProgram(const char *vertexPath, const char *fragmentPath);

        ~ShaderProgram();

        bool isCompiled() const { return m_isCompiled; }

        void use() const;

        void setBool(const std::string &name, bool value) const;

        void setInt(const std::string &name, int value) const;

        void setFloat(const std::string &name, float value) const;

    private:
        // полезные функции для проверки ошибок компиляции/связывания шейдеров.
        // ------------------------------------------------------------------------
        void checkCompileErrors(unsigned int shader, std::string type);

        GLuint createShader(GLenum shaderType, const char *sourceCode);

        bool m_isCompiled = true;
    };
}

#endif //LEARNOPENGL_SHADERPROGRAM_H
