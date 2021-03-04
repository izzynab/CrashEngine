#version 330 core
layout (location = 0) out vec4 FragColor;

in vec3 WorldPos;

uniform samplerCube environmentMap;

void main()
{		
    vec3 envColor = textureLod(environmentMap, WorldPos, 0).rgb;    
    FragColor = vec4(envColor, 1.0);
}