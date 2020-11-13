#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 WorldPos;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

//uniform mat4 projection;
//uniform mat4 view;

void main()
{
    WorldPos = a_Position;

	mat4 rotView = mat4(mat3(view));
	vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);

	gl_Position = clipPos.xyww;
}