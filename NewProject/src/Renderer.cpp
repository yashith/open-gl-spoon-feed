#include "Renderer.h"
#include<iostream>

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