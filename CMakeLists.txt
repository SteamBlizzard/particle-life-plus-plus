cmake_minimum_required(VERSION 3.10)
project(ParticleLife++ LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

set(SOURCES
  src/clock.cpp
  src/configurations.cpp
  src/main.cpp
  src/overlay.cpp
  src/physics_engine.cpp
  src/renderer.cpp
  src/resource_manager.cpp
  src/settings.cpp
  src/shader.cpp
  src/simulator.cpp
)

add_executable(pl++ ${SOURCES})
target_include_directories(pl++ PRIVATE include)
target_link_libraries(pl++ PRIVATE
  glad::glad
  glfw
  glm::glm
  imgui::imgui
)