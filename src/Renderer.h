#pragma once

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
