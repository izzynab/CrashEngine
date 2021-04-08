#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D frame_texture;


void main()
{    
    vec3 color = vec3(texture(frame_texture,TexCoords));
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        FragColor = vec4(color.rgb,1.0);
    else
        FragColor = vec4(0.0, 0.0, 0.0,1.0);
  
}