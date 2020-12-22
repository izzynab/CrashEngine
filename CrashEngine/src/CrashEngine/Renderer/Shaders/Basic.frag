#version 330 core
out vec3 FragColor;
in vec2 TexCoords;

uniform vec3 color;


void main()
{    
    FragColor = color;
}