#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// Project Includes
#include "plpp/shader.h"

// C++ Standard Library
#include <map>
#include <string>

namespace PLPP
{
  class ResourceManager
  {
  public:
    static std::map<std::string, Shader> Shaders;

    static Shader LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    static Shader LoadShader(const char *cShaderFile, std::string name);
    static void Clear();

    static Shader GetShader(std::string name) { return Shaders.at(name); };
    

  private:
    ResourceManager() {}
    static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    static Shader loadShaderFromFile(const char *cShaderFile);
  };
}

#endif
