#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

// clang-format off
#define STARTUP_FAILED -1
#define ASSERT(x) if (!x) { raise(SIGTRAP); }
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))
// clang-format on

GLenum lasterror;

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

// Clears errors
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

static bool GLLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        // clang-format off
        std::cerr
        << "[OpenGL Error]:"
        << ", ERR ENUM: "    << error
        << " FUNC: "         << function
        << "\n";
        // clang-format on
        return false;
    }

    return true;
}

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

    return {
        .VertexSource = ss[(int)ShaderType::VERTEX].str(),
        .FragmentSource = ss[(int)ShaderType::FRAGMENT].str()
    };
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

    // Create a windowed mode window and
    // its OpenGL context
    window = glfwCreateWindow(640, 480, "OpenGL DEMO", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(STARTUP_FAILED);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

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

    unsigned int array_buffer;
    glGenBuffers(1, &array_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    unsigned int index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    ShaderProgramSource shaderSource = ParseShaderFile("./res/shaders/basic.glsl");
    unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    glUseProgram(shader);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        GLCall(glfwSwapBuffers(window));
        GLCall(glfwPollEvents());
    }

    GLCall(glDeleteProgram(shader));

    GLCall(glfwTerminate());
    exit(EXIT_SUCCESS);
}
