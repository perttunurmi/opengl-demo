#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#define STARTUP_FAILED -1

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShaderFile(const std::string &filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
            else
            {
                std::cout << "Please provide a `type` after #shader.\n";
                assert(false);
            }
        }
        else
        {
            assert(type != ShaderType::NONE);
            ss[(int)type] << line << '\n';
        }
    }

    return {.VertexSource = ss[(int)ShaderType::VERTEX].str(), .FragmentSource = ss[(int)ShaderType::FRAGMENT].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = &source[0];
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char *message = (char *)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cerr << "Shader compilation failed " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader"
                  << std::endl;

        std::cerr << message << std::endl;

        assert(false);
    }

    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

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

    VertexBuffer vb(positions, 4 * 2 * sizeof(float));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    IndexBuffer ib(indices, 6);

    ShaderProgramSource shaderSource = ParseShaderFile("./res/shaders/Basic.glsl");
    unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    GLCall(glUseProgram(shader));

    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT((location != -1))
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.4f, 1.0f));

    GLCall(int offset_location = glGetUniformLocation(shader, "u_Offset"));
    ASSERT((offset_location != -1))
    GLCall(glUniform2f(offset_location, 0.0f, 0.0f));

    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glUseProgram(0));

    float r = 0.0f;
    float increment = 0.02f;
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUseProgram(shader));

        GLCall(glBindVertexArray(vao));
        ib.Bind();

        GLCall(glUniform4f(location, r, 0.3f, 0.4f, 1.0f));
        GLCall(glUniform2f(offset_location, r - 0.5f, r - 0.5f));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f || r < 0.0f)
            increment = -1 * increment;

        r += increment;

        GLCall(glfwSwapBuffers(window));
        GLCall(glfwPollEvents());
    }

    GLCall(glDeleteProgram(shader));

    GLCall(glfwTerminate());
    exit(EXIT_SUCCESS);
}
