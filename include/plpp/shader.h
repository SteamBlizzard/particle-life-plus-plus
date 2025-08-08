#ifndef SHADER_H
#define SHADER_H

// External Libraries
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// C++ Standard Library
#include <string>

namespace PLPP
{
  class Shader
  {
  public:
    unsigned int ID;
    Shader() = default;

    Shader &Use();

    void Dispatch(int groups);

    void compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr);
    void compile(const char *computeSource);
    void SetBool(const char *name, bool value, bool useShader = false);
    void SetFloat(const char *name, float value, bool useShader = false);
    void SetInteger(const char *name, int value, bool useShader = false);
    void SetVec2f(const char *name, float x, float y, bool useShader = false);
    void SetVec2f(const char *name, const glm::vec2 &value, bool useShader = false);
    void SetVec3f(const char *name, float x, float y, float z, bool useShader = false);
    void SetVec3f(const char *name, const glm::vec3 &value, bool useShader = false);
    void SetVec4f(const char *name, float x, float y, float z, float w, bool useShader = false);
    void SetVec4f(const char *name, const glm::vec4 &value, bool useShader = false);
    void SetVec2i(const char *name, int x, int y, bool useShader = false);
    void SetVec2i(const char *name, const glm::ivec2 &value, bool useShader = false);
    void SetVec3i(const char *name, int x, int y, int z, bool useShader = false);
    void SetVec3i(const char *name, const glm::ivec3 &value, bool useShader = false);
    void SetVec4i(const char *name, int x, int y, int z, int w, bool useShader = false);
    void SetVec4i(const char *name, const glm::ivec4 &value, bool useShader = false);
    void SetMat4(const char *name, const glm::mat4 &matrix, bool useShader = false);

  private:
    enum class ShaderType
    {
      Render,
      Compute
    };
    void checkCompileErrors(unsigned int object, std::string type);
    ShaderType type_;
  };
}

#endif