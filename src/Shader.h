//
// Created by Kostya on 06.10.2020.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <glad/glad.h>

//#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader {
public:
    unsigned int ID;

    // конструктор генерирует шейдер на лету
    // ------------------------------------------------------------------------
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
private:
    // полезные функции для проверки ошибок компиляции/связывания шейдеров.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif //LEARNOPENGL_SHADER_H
