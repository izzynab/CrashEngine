#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

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
    vec2 uv = SampleSphericalMap(normalize(WorldPos));
    if(samples == 0) FragColor = texture(equirectangularMap,uv);
    else FragColor = blur(equirectangularMap, uv, 1/vec2(3000,3000));
}