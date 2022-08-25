#include <fstream>
#include <sstream>
#include "Shader.h"
#include "Renderer.h"

#include <GL/glew.h>




Shader::Shader(const std::string& filepath)
    :m_filepath(filepath), m_Renderer_ID(0)
{
    ShaderProgramSource shaderSource = GetShaderStrings(filepath);
    m_Renderer_ID = CreateShader(shaderSource.Vertex, shaderSource.Fragment);
}
Shader::~Shader() {
    GLCall(glDeleteProgram(m_Renderer_ID));
}

void Shader::Bind()const {
    GLCall(glUseProgram(m_Renderer_ID));
}
void Shader::Unbind()const {
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    int location = glGetUniformLocation(m_Renderer_ID, name.c_str());
    glUniform4f(location,v0,v1,v2,v3);
}
void Shader::SetUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
}
unsigned int Shader::GetUniformLocation(const std::string & name) {
    GLCall(unsigned int location = glGetUniformLocation(m_Renderer_ID, name.c_str()));
    //if (location == -1) {
    //    std::cout << "Warning" << name << std::endl;
    //}
    return location;
}
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile Shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}
ShaderProgramSource Shader::GetShaderStrings(std::string filePath) {
    std::ifstream stream(filePath);
    std::stringstream ss[2];
    ShaderType shaderType = ShaderType::NONE;
    std::string line;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                shaderType = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                shaderType = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)shaderType] << line << '\n';
        }
    }

    return { ss[0].str(),ss[1].str() };
}
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}