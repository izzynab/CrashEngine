#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform int samples = 35;
float sigma = float(samples) * .25;

float gaussian(vec2 i) 
{
    return exp( -.5* dot(i/=sigma,i) ) / ( 6.28 * sigma*sigma );
}

vec4 blur(sampler2D image, vec2 uv, vec2 scale) 
{
    vec4 color = vec4(0);  
    int s = samples;
    
    for ( int i = 0; i < s*s; i++ ) 
    {
        vec2 d = vec2(i%s, i/s) - float(samples)/2.;
        color += gaussian(d) * texture(image, uv + scale * d);
    }
    
    return color / color.a;
}

void main()
{             
    FragColor = blur(image, TexCoords, 1/vec2(1000,1000));
}