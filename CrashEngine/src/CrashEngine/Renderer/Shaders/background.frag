#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 WorldPos;

uniform samplerCube environmentMap;

void main()
{		
    vec3 envColor = textureLod(environmentMap, WorldPos, 0).rgb;
    
    // HDR tonemap and gamma correct
    //envColor = envColor / (envColor + vec3(1.0));
    //envColor = pow(envColor, vec3(1.0/2.2)); 
    
    FragColor = vec4(envColor, 1.0);
    BrightColor = vec4(0,0,0,1);
}