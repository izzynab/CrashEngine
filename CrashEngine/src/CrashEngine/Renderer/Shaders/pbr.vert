#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Normals;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = a_TexCoords;
    WorldPos = vec3(model * vec4(a_Position, 1.0));
    Normal = mat3(model) * a_Normals;   

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}