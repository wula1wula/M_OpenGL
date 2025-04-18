#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>// 包含glad来获取所有的必须OpenGL头文件

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader
{
private:
    // 程序ID
    unsigned int ID;
    //着色器源码路径
    std::string FilePath;
public:
    // 构造器读取并构建着色器
    Shader(const std::string filepath);
    ~Shader();
    // 使用/激活程序
    void use();

    unsigned int GetID();

    // uniform工具函数
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
private:
    ShaderProgramSource ParseShader(const std::string filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader,const std::string& fragmentShader);
};

#endif