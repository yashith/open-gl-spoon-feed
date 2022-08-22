#pragma once
#include <iostream>

struct ShaderProgramSource
{
	std::string Vertex;
	std::string Fragment;
};
enum ShaderType
{
	NONE = 2, VERTEX = 0, FRAGMENT = 1
};

class Shader {
private:
	unsigned int m_Renderer_ID;
	std::string m_filepath;
public:
	Shader(const std::string& filepath);
	~Shader();
	
	void Bind() const;
	void Unbind() const;
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
private:
	unsigned int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource GetShaderStrings(std::string filePath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};