#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gAlbedo;
layout (location = 2) out vec4 gNormal;
layout (location = 3) out vec4 gMetalRoughAO;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;        

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{    
    gPosition = vec4(WorldPos,1.f);

    gAlbedo = vec4(texture(albedoMap, TexCoords).rgb,1.f);

    gNormal = vec4(getNormalFromMap(),1.f);

    gMetalRoughAO.r = texture(metallicMap, TexCoords).r;
    gMetalRoughAO.g = texture(roughnessMap, TexCoords).r;
    gMetalRoughAO.b = texture(aoMap, TexCoords).r;
}