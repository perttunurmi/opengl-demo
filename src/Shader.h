#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
  private:
    std::string m_Filepath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

  public:
    Shader(const std::string &filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1f(const std::string &name, int v1);
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void SetUniform2f(const std::string &name, float v0, float v1);
    void SetUniform4M(const std::string &name, const float matrix[16]);

  private:
    ShaderProgramSource ParseShader(const std::string &filepath);
    unsigned int CompileShader(unsigned int type, const std::string &source);
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

    int GetUniformLocation(const std::string &name);
};
