#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(std::string function, std::string file,int line) {
    while (GLenum error = glGetError()) {
        std::cout << error 
            << function <<
             file <<
            line<< std::endl;
        return false;
    }
    return true;
}
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

    /*float positions[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,

         0.5f, 0.5f,
         -0.5f, 0.5f,
         -0.5f, -0.5f,
    };*/
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

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 *2 * sizeof(float),positions,GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 *  sizeof(unsigned int), &indecies[0], GL_STATIC_DRAW);

    ShaderProgramSource shaderSource = GetShaderStrings("src/Source.shader");
    unsigned int shader = createShader(shaderSource.Vertex,shaderSource.Fragment);
    glUseProgram(shader);

   int location = glGetUniformLocation(shader,"u_Color");
   glUniform4f(location,0.8f,0.3f,0.8f,1.0f);
   float r = 0.0f;
   float increment = 0.5f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        GLClearError();

        if (r > 1.0f) {
            increment = -0.05f;
        }
        else if (r < 1.0f) {
            increment = 0.05f;
        }
        r += increment;
     
        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}