#version 440 core

in vec2 fragCenter;
in vec4 fragColor;

out vec4 outColor;

uniform float radius;
uniform vec2 u_resolution;

void main()
{
    // Invert y
    vec2 center_inverted = vec2(fragCenter.x, u_resolution.y - fragCenter.y);
    vec2 dist = gl_FragCoord.xy - center_inverted;
    if (length(dist) > radius) discard;
    outColor = vec4(fragColor.rgb, 1);
}