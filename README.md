# ParticleLife++

## What is ParticleLife++?
ParticleLife++ is a particle simulation written in C++20 using OpenGL and related libraries/tools. It is inspired by [Tom Mohr's Particle Life](https://github.com/tom-mohr/particle-life-app) implementation, which itself was inspired by [Jeffrey Ventrella](https://www.ventrella.com/). I'd strongly recommend you check out their works!

## Why remake Particle Life?
I was fascinated by Particle Life and wanted to challenge myself by taking on a project that would allow me to learn different tools and techniques. Ideally, this implementation of Particle Life will be close in quality to Mohr's.

This project is my first attempt at using & learning graphics APIs outside of game engines and my first major project in C++.

## Installation & Usage
### Prerequisites
* gcc/g++
* [vcpkg](https://github.com/microsoft/vcpkg)
* [CMake](https://cmake.org/)

### Steps
1. Ensure you've properly configured `vcpkg`
    - Run `bootstrap-vcpkg.bat`/`bootstrap-vcpkg.sh` if not already done
    - Set `VCPKG_ROOT` environment variable to the path of vcpkg
2. From the ParticleLife++ root directory, run: `cmake --preset release; make`
3. Run the program from the project root:
    - Windows: `./build/bin/release/pl++`
    - Mac/Linux: `.\build\bin\release\pl++.exe`

## Credits & Resources
* [Particle Life](https://github.com/tom-mohr/particle-life-app)
* [Jeffrey Ventrella](https://www.ventrella.com/)
* [LearnOpenGL](https://learnopengl.com/)
* [OpenGL](https://www.opengl.org/)
* [Dear ImGUI](https://github.com/ocornut/imgui)
* [GLFW](https://www.glfw.org/)
* [GLAD](https://github.com/Dav1dde/glad)
* [OpenGL Mathematics (GLM)](https://glm.g-truc.net/0.9.9/)
* [CMake](https://cmake.org/)
* [vcpkg](https://github.com/microsoft/vcpkg)