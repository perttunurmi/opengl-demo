#include "Renderer.h"
#include <iostream>

// Clears errors
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        // clang-format off
        std::cerr
        << "[OpenGL Error]:"
        << ", ERR ENUM: "    << error
        << " FUNC: "         << function
        << " FILE: "         << file
        << " LINE: "         << line
        << "\n";
        // clang-format on
        return false;
    }

    return true;
}
