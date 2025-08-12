#include "plpp/resource_manager.h"

// External Libraries
#include <glad/glad.h>

// C++ Standard Library
#include <iostream>
#include <sstream>
#include <fstream>
#include <format>
#include <filesystem>

namespace PLPP
{
  std::map<std::string, Shader> ResourceManager::Shaders;

  Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, std::string name)
  {
    Shader shader = loadShaderFromFile(vShaderFile, fShaderFile);
    Shaders.emplace(name, shader);
    return shader;
  }

  Shader ResourceManager::LoadShader(const char *cShaderFile, std::string name)
  {
    Shader shader = loadShaderFromFile(cShaderFile);
    Shaders.emplace(name, shader);
    return shader;
  }

  void ResourceManager::Clear()
  {
    for (auto iter : Shaders)
      glDeleteProgram(iter.second.ID);
  }

  Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile)
  {
    std::ifstream vertexShaderFile(vShaderFile);
    std::ifstream fragmentShaderFile(fShaderFile);
    if (!vertexShaderFile)
        std::cout << std::format("Vertex shader not found at location {}", std::filesystem::current_path().append(vShaderFile).string()) << std::endl;
    if (!fragmentShaderFile)
        std::cout << std::format("Fragment shader not found at location {}", std::filesystem::current_path().append(fShaderFile).string()) << std::endl;

    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vertexShaderFile.rdbuf();
    fShaderStream << fragmentShaderFile.rdbuf();
    vertexShaderFile.close();
    fragmentShaderFile.close();

    std::string vertexCode(vShaderStream.str());
    std::string fragmentCode(fShaderStream.str());

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    return Shader(vShaderCode, fShaderCode);
  }

  Shader ResourceManager::loadShaderFromFile(const char *cShaderFile)
  {
    std::ifstream computeShaderFile(cShaderFile);

    if (!computeShaderFile)
        std::cout << std::format("Compute shader not found at location {}", std::filesystem::current_path().append(cShaderFile).string()) << std::endl;

    std::stringstream cShaderStream;
    cShaderStream << computeShaderFile.rdbuf();
    computeShaderFile.close();
    std::string computeCode(cShaderStream.str());

    const char *cShaderCode = computeCode.c_str();
    return Shader(cShaderCode);
  }
}