#include "plpp/resource_manager.h"

// External Libraries
#include <glad/glad.h>

// C++ Standard Library
#include <iostream>
#include <sstream>
#include <fstream>

namespace PLPP
{ // Instantiate static variables
  std::map<std::string, Shader> ResourceManager::Shaders;

  Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name)
  {
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
  }

  Shader ResourceManager::LoadShader(const char *cShaderFile, std::string name)
  {
    Shaders[name] = loadShaderFromFile(cShaderFile);
    return Shaders[name];
  }

  Shader ResourceManager::GetShader(std::string name)
  {
    return Shaders[name];
  }

  void ResourceManager::Clear()
  {
    // (properly) delete all shaders
    for (auto iter : Shaders)
      glDeleteProgram(iter.second.ID);
  }

  Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
  {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
      // open files
      std::ifstream vertexShaderFile(vShaderFile);
      std::ifstream fragmentShaderFile(fShaderFile);
      std::stringstream vShaderStream, fShaderStream;
      // read file's buffer contents into streams
      vShaderStream << vertexShaderFile.rdbuf();
      fShaderStream << fragmentShaderFile.rdbuf();
      // close file handlers
      vertexShaderFile.close();
      fragmentShaderFile.close();
      // convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
      // if geometry shader path is present, also load a geometry shader
      if (gShaderFile != nullptr)
      {
        std::ifstream geometryShaderFile(gShaderFile);
        std::stringstream gShaderStream;
        gShaderStream << geometryShaderFile.rdbuf();
        geometryShaderFile.close();
        geometryCode = gShaderStream.str();
      }
    }
    catch (std::exception e)
    {
      std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
  }

  Shader ResourceManager::loadShaderFromFile(const char *cShaderFile)
  {
    std::string computeCode;
    try
    {
      std::ifstream computeShaderFile(cShaderFile);
      std::stringstream cShaderStream;
      cShaderStream << computeShaderFile.rdbuf();
      computeShaderFile.close();
      computeCode = cShaderStream.str();
    }
    catch (std::exception e)
    {
      std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *cShaderCode = computeCode.c_str();
    Shader shader;
    shader.compile(cShaderCode);
    return shader;
  }
}