#version 330 core

layout (location = 0) out vec4 FragColor;

uniform float near_plane;
uniform float far_plane;


void main()
{             
    FragColor = vec4(gl_FragCoord.z);
}