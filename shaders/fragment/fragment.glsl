#version 410 core
in vec4 vertexColor;

out vec4 FragColor;

uniform vec4 cubeColor;

void main()
{
    FragColor = mix(vertexColor, cubeColor, 0.5 * (1.0 + sin(gl_FragCoord.x * 0.1 + cubeColor.r)));
}