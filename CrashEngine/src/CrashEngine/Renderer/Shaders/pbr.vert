#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;       
out float ClipSpacePosZ;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;

    vec4 viewPos = view * model * vec4(aPos, 1.0);
    WorldPos = viewPos.rgb; 

    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    Normal = normalMatrix *  aNormal;

    gl_Position = projection * viewPos;

    ClipSpacePosZ = gl_Position.z;
}