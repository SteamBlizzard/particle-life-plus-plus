#version 440 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;
layout (location = 2) in vec4 aColor;

uniform mat4 projection;
uniform float radius;

out vec2 fragCenter;
out vec4 fragColor;

void main()
{
    vec2 worldPos = aOffset + aPos * radius;
    gl_Position = (projection * vec4(worldPos, 0.0, 1.0));
    fragCenter = aOffset;
    fragColor = aColor;
}