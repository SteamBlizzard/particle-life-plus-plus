#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// C++ Standard Library
#include <map>
#include <string>

// Project Includes
#include "plpp/shader.h"

namespace PLPP
{
  class ResourceManager
  {
  public:
    static std::map<std::string, Shader> Shaders;
    static Shader LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    static Shader LoadShader(const char *cShaderFile, std::string name);
    static Shader GetShader(std::string name);
    static void Clear();

  private:
    ResourceManager() {}
    static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    static Shader loadShaderFromFile(const char *cShaderFile);
  };
}

#endif
