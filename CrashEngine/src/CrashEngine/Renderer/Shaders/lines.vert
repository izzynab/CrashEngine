#version 330 core
layout (location = 0) in vec3 a_Position;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform vec3 color;
uniform float lineWidth;

out vec4 v_col;
out float v_line_width;

void main()
{
    v_col = vec4(color,1);
    v_line_width = lineWidth;

    vec4 position = projection * view * vec4(a_Position, 1.0);
    gl_Position = position;
}

   