#version 330 core
out vec4 FragColor;

in vec2 lineCenter;

uniform vec3 color;
uniform float lineWidth;

uniform vec2 ViewPort;

float blendFactor = 1.4f;

void main()
{       
      vec4 col = vec4(color,1);
      float d = length(lineCenter-(gl_FragCoord.xy));
      if (d>lineWidth)
        col.w = 0;
      else
        col.w *= pow(float((lineWidth-d)/lineWidth), blendFactor);
      //FragColor = col;

      FragColor = vec4(color,1);
      
}