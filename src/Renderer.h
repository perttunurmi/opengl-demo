#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include <GL/glew.h>
#include <csignal>

// clang-format off
#define ASSERT(x) if (!x) { raise(SIGTRAP); }
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))
// clang-format on

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

class Renderer
{
  public:
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
    void Clear() const;
};
