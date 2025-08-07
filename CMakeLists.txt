cmake_minimum_required(VERSION 3.10)
project(ParticleLife++ LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_library(imgui STATIC
  lib/imgui/imgui.cpp
  lib/imgui/imgui_draw.cpp
  lib/imgui/imgui_tables.cpp
  lib/imgui/imgui_widgets.cpp
  lib/imgui/imgui_demo.cpp
  lib/imgui/backends/imgui_impl_glfw.cpp
  lib/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(glad STATIC
  lib/glad/glad.c
)

add_library(glfw STATIC IMPORTED)

set_target_properties(glad PROPERTIES
  LINKER_LANGUAGE C
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
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/build/lib/libglfw3.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY build/lib
  LIBRARY_OUTPUT_DIRECTORY build/lib
  RUNTIME_OUTPUT_DIRECTORY build/bin
)

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
target_include_directories(pl++ PRIVATE lib include)
target_include_directories(imgui PRIVATE lib lib/imgui lib/imgui/backends)
target_include_directories(glad PRIVATE lib lib/glad)
target_link_libraries(pl++ PRIVATE glfw glad imgui)

set_target_properties(pl++ PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY build/lib
  LIBRARY_OUTPUT_DIRECTORY build/lib
  RUNTIME_OUTPUT_DIRECTORY build/bin
)