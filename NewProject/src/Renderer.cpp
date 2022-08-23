#include "Renderer.h"
#include<iostream>
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
bool GLLogCall(std::string function, std::string file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << error
            << function <<
            file <<
            line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
    }