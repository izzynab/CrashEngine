#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

// material parameters
uniform sampler2D position;
uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D MetalRoughAO;
uniform sampler2D ssao;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// lights
uniform vec3 lightRotation;
uniform vec3 lightColor;

uniform vec3 camPos;

const int Cascades = 4;

uniform sampler2D shadowMap[Cascades];

uniform mat4 lightSpaceMatrix[Cascades];
uniform float cascadeEndClipSpace[Cascades];

uniform int csmColor;


layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

const float PI = 3.14159265359;

float R_shadowVarianceMin = 0.00002f;
float R_shadowLightBleedingReduction = 0.2f;

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   
// ----------------------------------------------------------------------------
float linstep(float low, float high, float v)
{
	return clamp((v-low)/(high-low), 0.0, 1.0);
}

float SampleVarianceShadowMap(vec2 coords, float compare,int cascade)
{
	vec2 moments = texture2D(shadowMap[cascade], coords.xy).xy;

	float p = step(compare, moments.x);
	float variance = max(moments.y - moments.x * moments.x, R_shadowVarianceMin);

	float d = compare - moments.x;
	float pMax = linstep(R_shadowLightBleedingReduction, 1.0, variance / (variance + d*d));

	return min(max(p, pMax), 1.0);
	//return step(compare, texture2D(shadowMap, coords.xy).r);
}

float ShadowCalculation(vec2 coords, float compare,int cascade)
{
    // transform to [0,1] range
    vec2 projCoords = coords * 0.5 + 0.5;
    compare = compare* 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap[cascade], projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = compare;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = texture(normal,TexCoords).rgb;
    vec3 lightDir = normalize(lightRotation);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap[cascade], 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap[cascade], projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(compare > 1.0)
        shadow = 0.0;
        
    return shadow;
}


void main()
{		
    // material properties
    vec3 albedo = pow(texture(albedo, TexCoords).rgb, vec3(2.2));
    float metallic = texture(MetalRoughAO, TexCoords).r;
    float roughness = texture(MetalRoughAO, TexCoords).g;
    float ao = texture(MetalRoughAO, TexCoords).b * texture(ssao, TexCoords).r;
       
    // input lighting data
    vec3 N = texture(normal,TexCoords).rgb;
    vec3 V = normalize(-texture(position,TexCoords).rgb);
    vec3 viewR = reflect(-V, N); 
    vec3 R = inverse(mat3(view)) * viewR;

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    //directional light
    for(int i = 0; i < 1; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lightRotation);
        vec3 H = normalize(V + L);

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;
        
         // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	                
            
        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += specular * lightColor * NdotL*kD; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;
        
    float dst = texture(normal,TexCoords).a;

    if (dst <= cascadeEndClipSpace[0]) 
        {
            vec4 FragPosLightSpace = lightSpaceMatrix[0] * inverse(view) * vec4(texture(position,TexCoords).rgb, 1.0);
            // perform perspective divide
            vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
            float shadow = ShadowCalculation(projCoords.xy,projCoords.z, 0);   
            if(shadow == 1) Lo = vec3(0);
        }
    for (int i = 1 ; i < Cascades ; i++) 
    {      
        if (dst <= cascadeEndClipSpace[i] && dst >= cascadeEndClipSpace[i-1]) 
        {
            vec4 FragPosLightSpace = lightSpaceMatrix[i] * inverse(view) * vec4(texture(position,TexCoords).rgb, 1.0);
             // perform perspective divide
            vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
            float shadow = ShadowCalculation(projCoords.xy, projCoords.z, i);   
            if(shadow == 1) Lo = vec3(0);
        }
    }

    vec3 color = ambient + Lo;
    FragColor = vec4(color , 1.0);

    for (int i = 0 ; i < Cascades ; i++) 
    {
        if (texture(normal,TexCoords).a <= cascadeEndClipSpace[i])
        {
        if(csmColor == 1)
        {
         if (i == 0) 
                 FragColor *= vec4(0.1, 0.0, 0.0, 0.0);
            else if (i == 1)
                FragColor *= vec4(0.0, 0.1, 0.0, 0.0);
            else if (i == 2)
                FragColor *= vec4(0.0, 0.0, 0.1, 0.0);
        }
            break;
        }
   }

}