#version 330 core

uniform vec4 color;
uniform vec2 u_resolution;
uniform mat4 model;
uniform mat4 projection;

// TODO: Works for circles, but not for ellipses.
void main()
{
    // Follow transformation from vertex shader
    // Calculate UV coordinates
    vec2 uv = gl_FragCoord.xy / u_resolution;
    uv = uv * 2.0 - 1.0; // Convert to range [-1, 1]
    
    float comparison = (model * vec4(0.5)).x;

    float circle = step(distance(uv, vec2(comparison)), comparison);
    if (circle == 0.0)
    {
        discard;
    }

    gl_FragColor = color;
}