#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D frame_texture;

uniform float Threshold;
uniform float SoftThreshold;

void main()
{    
    vec3 color = vec3(texture(frame_texture,TexCoords));
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    //float brightness = max(color.r, max(color.g, color.b));
    //if(brightness > 1.0)
    //    FragColor = vec4(color.rgb,1.0);
    //else
     //   FragColor = vec4(0.0, 0.0, 0.0,1.0);

     float knee = Threshold * SoftThreshold;
	 float soft = brightness - Threshold + knee;
	 soft = clamp(soft, 0, 2 * knee);
	 soft = soft * soft / (4 * knee + 0.00001);
	 float contribution = max(soft, brightness - Threshold);
	 contribution /= max(brightness, 0.00001);
     if(brightness > 0)FragColor = vec4(color.rgb * contribution,1.0);
     else FragColor = vec4(0.0, 0.0, 0.0,1.0);
}