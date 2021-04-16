#version 330 core
layout (location = 0) in vec3 a_Position;

out vec2 lineCenter;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

uniform vec2 viewPort;

void main()
{
    vec4 position = projection * view * model * vec4(a_Position, 1.0);
    gl_Position = position;
    lineCenter = 0.5 * (position.xy + vec2(1,1))*viewPort;
}