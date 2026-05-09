#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>

#include "Renderer.h"
#include "Texture.h"
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

    // Set GL_VERSION and turn Compatibility Profile off
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

    // Vsync
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
            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f
        };
        // clang-format on

        unsigned int indices[] = {0, 1, 2, 2, 3, 0};

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push(GL_FLOAT, 2);
        layout.Push(GL_FLOAT, 2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("./res/shaders/Basic.glsl");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.4f, 1.0f);
        shader.SetUniform2f("u_Offset", 0.0f, 0.0f);

        vb.Unbind();
        va.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.02f;

        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            shader.Bind();

            shader.SetUniform4f("u_Color", r, 0.3f, 0.4f, 1.0f);
            shader.SetUniform2f("u_Offset", r - 0.5f, r - 0.5f);

            Texture texture("res/textures/flipped_hearth.ppm");
            texture.Bind();
            shader.SetUniform1f("u_Texture", 0);

            renderer.Draw(va, ib, shader);

            if (r > 1.0f || r < 0.0f)
                increment = -1 * increment;

            r += increment;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        shader.Unbind();
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
