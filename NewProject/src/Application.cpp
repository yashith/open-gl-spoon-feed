#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "vendor/stb_image.h"

int main(void)
{
    GLFWwindow* window;

    float positions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f, 0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,   0.0f, 1.0f
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

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);



    IndexBuffer ib(indecies, 6);

    Shader shader("ref/Shaders/Source.shader");
    shader.Bind();
    
    //texture test
    // 
    Texture texture("ref/Textures/texture.png");
    texture.Bind();

    shader.SetUniform1i("u_Texture", 0);
    

    //shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    Renderer renderer;

    float r = 0.0f;
    float increment = 0.5f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

        renderer.Draw(va, ib, shader);


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