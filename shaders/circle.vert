#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    // z is set to 0.0 for 2D rendering
    // gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    // gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
    gl_Position = model * vec4(aPos, 0.0, 1.0);
}