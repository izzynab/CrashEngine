#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D tex;
uniform vec3 color;

void main()
{    
    FragColor = texture(tex,TexCoords);
    //FragColor = vec4(color,1);
}