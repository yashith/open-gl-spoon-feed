#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderProgramSource
{
    std::string Vertex;
    std::string Fragment;
};
enum ShaderType
{
    NONE=2,VERTEX=0,FRAGMENT=1
};
static ShaderProgramSource GetShaderStrings(std::string filePath) {
    std::ifstream stream(filePath);
    std::stringstream ss[2];
    ShaderType shaderType=ShaderType::NONE;
    std::string line;
    while (getline(stream, line)) {
        if (line.find("#shader")!=std::string::npos) {
            if (line.find("vertex")!=std::string::npos) {
                shaderType = ShaderType::VERTEX;
            }
            else if (line.find("fragment")!= std::string::npos) {
                shaderType = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)shaderType] << line << '\n';
        }
    }
    
    return { ss[0].str(),ss[1].str() };
}
static unsigned int compileShader( unsigned int type, const std::string& source) {
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
static unsigned int createShader(const std::string& vertexShader,const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}
int main(void)
{
    GLFWwindow* window;

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f, 0.5f,
        -0.5f, 0.5f
    };
    unsigned int indecies[] = {
        0,1,2,
        2,3,0
    };

    /* Initialize the library */
    if (!glfwInit())
        return -1;
   
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    IndexBuffer ib(indecies, 6);

    ShaderProgramSource shaderSource = GetShaderStrings("ref/Shaders/Source.shader");
    unsigned int shader = createShader(shaderSource.Vertex,shaderSource.Fragment);
    glUseProgram(shader);

   int location = glGetUniformLocation(shader,"u_Color");
   glUniform4f(location,0.8f,0.3f,0.8f,1.0f);

   GLCall(glUseProgram(0));
   GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
   GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));
   GLCall(glBindVertexArray(0));
   float r = 0.0f;
   float increment = 0.5f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        GLCall(glUseProgram(shader));
        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

        //GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
        GLCall(glBindVertexArray(vao));
        //GLCall(glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,0));
        //GLCall(glEnableVertexAttribArray(0));

        GLCall(ib.Bind());


        if (r > 1.0f) {
            increment = -0.05f;
        }
        else if (r < 0.0f) {
            increment = 0.05f;
        }
        r += increment;
     
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}