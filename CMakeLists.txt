cmake_minimum_required(VERSION 3.10)
project(ParticleLife++ LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(imgui STATIC
  include/imgui/imgui.cpp
  include/imgui/imgui_draw.cpp
  include/imgui/imgui_tables.cpp
  include/imgui/imgui_widgets.cpp
  include/imgui/imgui_demo.cpp
  include/imgui/backends/imgui_impl_glfw.cpp
  include/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(glad STATIC
  include/glad/glad.c
)

add_library(glfw STATIC
  build/lib/glfw3.dll
  build/lib/libglfw3.a
  build/lib/libglfw3dll.a
)

set_target_properties(glad PROPERTIES
  LINKER_LANGUAGE CXX
  ARCHIVE_OUTPUT_DIRECTORY build/lib
  LIBRARY_OUTPUT_DIRECTORY build/lib
  RUNTIME_OUTPUT_DIRECTORY build/bin
)

set_target_properties(imgui PROPERTIES
  LINKER_LANGUAGE CXX
  ARCHIVE_OUTPUT_DIRECTORY build/lib
  LIBRARY_OUTPUT_DIRECTORY build/lib
  RUNTIME_OUTPUT_DIRECTORY build/bin
)

set_target_properties(glfw PROPERTIES
  LINKER_LANGUAGE CXX
  ARCHIVE_OUTPUT_DIRECTORY build/lib
  LIBRARY_OUTPUT_DIRECTORY build/lib
  RUNTIME_OUTPUT_DIRECTORY build/bin
)

set(SOURCES
  src/main.cpp
  src/physics_engine.cpp
  src/renderer.cpp
  src/resource_manager.cpp
  src/shader.cpp
  src/simulator.cpp
)

add_executable(pl++ ${SOURCES}) 
target_link_libraries(pl++ PRIVATE glfw glad imgui)

target_include_directories(pl++ PRIVATE include)
target_include_directories(imgui PRIVATE include)

install(TARGETS pl++ DESTINATION build/bin)