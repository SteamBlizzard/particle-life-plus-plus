#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
  public:
    unsigned int ID; 
    Shader() { }

    Shader  &Use();

    void   compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

    void    SetBool     (const char *name, bool value, bool useShader = false);
    void    SetFloat    (const char *name, float value, bool useShader = false);
    void    SetInteger  (const char *name, int value, bool useShader = false);
    void    SetVec2f    (const char *name, float x, float y, bool useShader = false);
    void    SetVec2f    (const char *name, const glm::vec2 &value, bool useShader = false);
    void    SetVec3f    (const char *name, float x, float y, float z, bool useShader = false);
    void    SetVec3f    (const char *name, const glm::vec3 &value, bool useShader = false);
    void    SetVec4f    (const char *name, float x, float y, float z, float w, bool useShader = false);
    void    SetVec4f    (const char *name, const glm::vec4 &value, bool useShader = false);
    void    SetMat4     (const char *name, const glm::mat4 &matrix, bool useShader = false);

  private:
    void    checkCompileErrors(unsigned int object, std::string type); 
};

#endif