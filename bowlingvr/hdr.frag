#version 450 core
layout(location = 0)out vec4 FragColor;
layout(location = 1)out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;

void main()
{             
    const float gamma = 0.39;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    if(hdr)
    {
        // reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
		// tone mapping
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // gamma correct    
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
	
	float brightness = dot(FragColor.rgb, vec3(0.2125, 0.7154, 0.0721));
    if(brightness > 0.90)
	{
        BrightColor = vec4(FragColor.rgb, 1.0);
    }
	else
	{
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}