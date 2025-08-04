#version 330 core

uniform vec4 color;
uniform vec2 center;
uniform float radius;

uniform vec2 u_resolution;

void main()
{
    // Invert y
    vec2 center_inverted = vec2(center.x, u_resolution.y - center.y);
    float dist = distance(gl_FragCoord.xy, center_inverted);
    if (dist > radius) discard;
    gl_FragColor = vec4(color.rgb, 1);
}