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

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    shader.Unbind();
    va.Unbind();
    ib.Unbind();
}
