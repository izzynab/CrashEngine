#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;
uniform bool blur;

void main()
{             
    vec3 Color = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if(blur)Color += bloomColor; // additive blending

    FragColor = vec4(Color, 1);
}