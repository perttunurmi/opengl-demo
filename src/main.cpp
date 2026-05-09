#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define STARTUP_FAILED -1

int main(void)
{
    GLFWwindow *window;

    // Initialize the library
    if (!glfwInit())
        exit(STARTUP_FAILED);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and
    // its OpenGL context
    GLCall(window = glfwCreateWindow(640, 480, "OpenGL DEMO", NULL, NULL));

    if (!window)
    {
        GLCall(glfwTerminate());
        exit(STARTUP_FAILED);
    }

    // Make the window's context current
    GLCall(glfwMakeContextCurrent(window));

    GLCall(glfwSwapInterval(1));

    // Try to initialize glew
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "ERROR: GLEW NOT OK" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        // clang-format off
        float positions[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f,
        };
        // clang-format on

        unsigned int indices[] = {0, 1, 2, 2, 3, 0};

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push(GL_FLOAT, 2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("./res/shaders/Basic.glsl");
        shader.Bind();

        shader.SetUniform4("u_Color", 0.2f, 0.3f, 0.4f, 1.0f);
        shader.SetUniform2("u_Offset", 0.0f, 0.0f);

        vb.Unbind();
        va.UnBind();
        ib.Unbind();
        shader.Unbind();

        float r = 0.0f;
        float increment = 0.02f;
        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            shader.Bind();

            va.Bind();
            ib.Bind();

            shader.SetUniform4("u_Color", r, 0.3f, 0.4f, 1.0f);
            shader.SetUniform2("u_Offset", r - 0.5f, r - 0.5f);

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f || r < 0.0f)
                increment = -1 * increment;

            r += increment;

            GLCall(glfwSwapBuffers(window));
            GLCall(glfwPollEvents());
        }

        shader.Unbind();
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
