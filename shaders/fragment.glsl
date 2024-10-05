#version 460 core
out vec4 FragColor;

in vec4 ourColor; // we set this variable in the OpenGL code.

void main()
{
    FragColor = ourColor;
} 
