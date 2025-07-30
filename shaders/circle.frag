#version 330 core

uniform vec4 input_color;
uniform vec2 u_resolution;
uniform float circle_radius;

void main()
{
    vec2 uv = gl_FragCoord.xy / u_resolution;
    float circle = step(distance(uv, vec2(0.5)), circle_radius);
    if (circle == 0.0)
    {
        discard;
    }

    gl_FragColor = input_color;
}