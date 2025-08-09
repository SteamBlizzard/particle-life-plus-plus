#version 440 core

layout (std430, binding = 0) buffer Positions {
    vec2 positions[];
};
layout (location = 0) in vec2 aPos;

layout (location = 1) in vec4 aColor;

uniform mat4 projection;
uniform float radius;

out vec2 fragCenter;
out vec4 fragColor;

void main()
{
    vec2 worldPos = positions[gl_InstanceID] + aPos * radius;
    gl_Position = (projection * vec4(worldPos, 0.0, 1.0));
    fragCenter = positions[gl_InstanceID];
    fragColor = aColor;
}