#version 460 core
out vec4 FragColor;

uniform vec4 vertexColor; // we set this variable in the OpenGL code.

void main()
{
    FragColor = vertexColor;
} 
