#version 330 core
layout (location = 0) in vec2 aPos;
void main()
{
    // z is set to 0.0 for 2D rendering
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}