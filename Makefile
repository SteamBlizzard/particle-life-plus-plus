CXX := g++
CXXFLAGS := -std=c++20 -Iimgui -Iimgui/backends -Iinclude

LDFLAGS := -Llib -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lshell32

# Source files
SRC := src/main.cpp \
       src/shader.cpp \
       src/resource_manager.cpp \
       src/renderer.cpp \
       src/physics_engine.cpp \
       src/simulator.cpp \
       include/imgui/imgui.cpp \
       include/imgui/imgui_draw.cpp \
       include/imgui/imgui_tables.cpp \
       include/imgui/imgui_widgets.cpp \
       include/imgui/imgui_demo.cpp \
       include/imgui/backends/imgui_impl_glfw.cpp \
       include/imgui/backends/imgui_impl_opengl3.cpp \
       include/glad/glad.c

# Output binary
TARGET := build/pl++.exe

# Default rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)
