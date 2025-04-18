#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>// ����glad����ȡ���еı���OpenGLͷ�ļ�

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
    // ����ID
    unsigned int ID;
    //��ɫ��Դ��·��
    std::string FilePath;
public:
    // ��������ȡ��������ɫ��
    Shader(const std::string filepath);
    ~Shader();
    // ʹ��/�������
    void use();

    unsigned int GetID();

    // uniform���ߺ���
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