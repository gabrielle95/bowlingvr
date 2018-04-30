#pragma once

#include <iostream>
#include <string>

const char c_modelShaderVert[] = R"glsl(

#version 450 core

//black magic happens here

layout(location = 0) in vec3 position; //position modelspace
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// for shadows
out vec4 FragPosLightSpace;
uniform mat4 lightSpaceMatrix;

//other stuff
out vec3 Normal;
out vec3 FragPos; //worldspace
out vec2 TexCoords;
out vec3 lightDir[10]; //cameraspace
out vec3 EyeDirection_cameraspace;
//out vec3 viewPos; //worldspace

uniform int enabledLights;
out int EnabledLightCnt;
uniform vec3 LightPosition_worldspace[10]; //to convert to lightDirection cameraspace

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
		mat4 MV = view * model;
		vec4 pos = vec4(position, 1.0f);
		TexCoords = texCoords;
		
		Normal = normalize(mat3(transpose(inverse(MV))) * normal); //cameraspace
		
		FragPos = vec3(model * pos); //worldspace
		
		vec3 position_cameraspace = (MV * pos).xyz;
		EyeDirection_cameraspace = vec3(0,0,0) - position_cameraspace;
		
		vec3 LightPosition_cameraspace;
		
		for(int i = 0; i < enabledLights; i++)
		{
			LightPosition_cameraspace = ( view * vec4(LightPosition_worldspace[i],1)).xyz;
			lightDir[i] = LightPosition_cameraspace + EyeDirection_cameraspace;
		}
		
		gl_Position = projection * MV * pos;
		//viewPos = (view * pos).xyz;
		//FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0); //light camera space
		//EnabledLightCnt = enabledLights;

}

)glsl";

const char c_modelShaderFrag[] = R"glsl(

#version 450 core

//black magic continues here
 
#define LINEAR_ATTENUATION 0.011
#define QUADR_ATTENUATION 0.0008 

struct LightProperties {
	bool isEnabled;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position; //world space
};

struct MaterialProperties {
	vec4 emission;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

const int MaxLights = 10;
uniform LightProperties Lights[MaxLights];

uniform MaterialProperties Material;
	
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

in vec3 lightDir[10]; //cameraspace
in vec3 EyeDirection_cameraspace;

uniform vec3 viewPos;

uniform bool isTVQuad;

// for shadows
in vec4 FragPosLightSpace;
uniform mat4 lightSpaceMatrix;
uniform samplerCube depthMap;
uniform float far_plane;

//out
layout(location = 0) out vec4 FragColor;
     
uniform sampler2D texture_ambient1;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
	
uniform int ambientTexCount;
uniform int diffuseTexCount;
uniform int specularTexCount;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos, vec3 lightPos)
{
	// get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;

    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows

    float shadow = 0.0;
    float bias = 0.43;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 100.f;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
        
    return shadow;
}

//main
void main()
{

	if(isTVQuad)
	{
		FragColor = texture(texture_diffuse1, TexCoords);
		return;
	}
	//vec3 lightDir[i];
	float radius = 1.0;
	
	vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
	
	vec3 N = normalize(Normal); //normal
	
	float constantAtt = 1.0;
	float shadow;
	
	vec4 colorFactor;
	vec4 emission = Material.emission;
	
	for(int i = 0; i < MaxLights; i++)
	{
		if(!Lights[i].isEnabled)
			continue;
		
		
		//attenuation
		float attenuation, dist;
		dist = length(Lights[i].position - FragPos); //worldspace
		attenuation = 1.0 / (constantAtt + LINEAR_ATTENUATION * dist + QUADR_ATTENUATION * dist * dist);
		//attenuation = 1.0 / (dist * dist);
		//attenuation = pow(attenuation, 0.45);
		
		vec3 L = normalize(lightDir[i]); //light direction cameraspace
		
		//shadow	
		shadow = ShadowCalculation(FragPos, Lights[i].position);
		
		//diffuse
		float diffuse = max(dot( N,L ),0.00);
		
		if(diffuseTexCount == 0){
			colorFactor = Material.diffuse;
		}
		else
		{
			colorFactor = texture(texture_diffuse1, TexCoords);
		}
		
		vec4 diff = attenuation * diffuse * Lights[i].diffuse;
		
		//reflect vec
		vec3 R = reflect(-L, N);

		// Eye vector (towards the camera)
		vec3 E = normalize(EyeDirection_cameraspace);

		
		//specular
		float specular;
		
		if(diff == 0)
			specular = 0;
		else
			specular = pow(dot( E,R ), Material.shininess);
		
		vec4 spec = vec4(0.0,0.0,0.0,1.0);
		
		if(specular >= 0.0)
		{
			spec = attenuation * specular * Lights[i].specular * Material.specular;		
		}
		 //ambient
		 vec4 amb = Lights[i].ambient * (texture(texture_diffuse1, TexCoords) + Material.ambient);
		
		finalColor += (amb + (1.0 - shadow) * (diff ) * colorFactor + spec) + Material.emission;

	}
	
	FragColor = finalColor;
}

)glsl";

const char c_depthShaderVert[] = R"glsl(

// Vertex shader for shadow map generation
#version 450 core
layout (location = 0) in vec3 position;

uniform mat4 model;

void main(void)
{
	gl_Position = model * vec4(position, 1.0);
}

)glsl";


const char c_depthShaderFrag[] = R"glsl(

// Fragment shader for shadow map generation
#version 450 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}

)glsl";

const char c_depthShaderGeom[] = R"glsl(

#version 450 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
} 

)glsl";

const char c_hdrShaderVert[] = R"glsl(

#version 450 core
layout (location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = texCoords;
    gl_Position = vec4(position, 1.0);
}

)glsl";

const char c_hdrShaderFrag[] = R"glsl(

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

)glsl";

const char c_blurShaderVert[] = R"glsl(

#version 450 core
layout (location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = texCoords;
    gl_Position = vec4(position, 1.0);
}

)glsl";

const char c_blurShaderFrag[] = R"glsl(

#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{             
     vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
     vec3 result = texture(image, TexCoords).rgb * weight[0];
     if(horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
         }
     }
     FragColor = vec4(result, 1.0);
}

)glsl";

const char c_bloomShaderVert[] = R"glsl(

#version 450 core
layout (location = 0) in vec2 position;
layout(location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = texCoords;
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}

)glsl";

const char c_bloomShaderFrag[] = R"glsl(

#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{         
	//const float gamma = 0.9;
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if(bloom)
        hdrColor += bloomColor; // additive blending
    
	// tone mapping
    //vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correct    
    //result = pow(result, vec3(1.0 / gamma));
	
	vec3 result = hdrColor;
    FragColor = vec4(result, 1.0);
}

)glsl";

const char c_VRcontrollerShaderVert[] =
// vertex shader
"#version 450 core\n"
"uniform mat4 matrix;\n"
"layout(location = 0) in vec4 position;\n"
"layout(location = 1) in vec3 v3ColorIn;\n"
"out vec4 v4Color;\n"
"void main()\n"
"{\n"
"	v4Color.xyz = v3ColorIn; v4Color.a = 1.0;\n"
"	gl_Position = matrix * position;\n"
"}\n";

const char c_VRcontrollerShaderFrag[] =
// fragment shader
"#version 450 core\n"
"in vec4 v4Color;\n"
"out vec4 outputColor;\n"
"void main()\n"
"{\n"
"   outputColor = v4Color;\n"
"}\n";

const char c_VRrendermodelShaderVert[] =
// vertex shader
"#version 450\n"
"uniform mat4 matrix;\n"
"layout(location = 0) in vec4 position;\n"
"layout(location = 1) in vec3 v3NormalIn;\n"
"layout(location = 2) in vec2 v2TexCoordsIn;\n"
"out vec2 v2TexCoord;\n"
"void main()\n"
"{\n"
"	v2TexCoord = v2TexCoordsIn;\n"
"	gl_Position = matrix * vec4(position.xyz, 1);\n"
"}\n";

const char c_VRrendermodelShaderFrag[] =
//fragment shader
"#version 450 core\n"
"uniform sampler2D diffuse;\n"
"in vec2 v2TexCoord;\n"
"out vec4 outputColor;\n"
"void main()\n"
"{\n"
"   outputColor = texture( diffuse, v2TexCoord);\n"
"}\n";

const char c_VRcompanionwindowShaderVert[] =
// vertex shader
"#version 450 core\n"
"layout(location = 0) in vec4 position;\n"
"layout(location = 1) in vec2 v2UVIn;\n"
"noperspective out vec2 v2UV;\n"
"void main()\n"
"{\n"
"	v2UV = v2UVIn;\n"
"	gl_Position = position;\n"
"}\n"
;

const char c_VRcompanionwindowShaderFrag[] =
// fragment shader
"#version 450 core\n"
"uniform sampler2D mytexture;\n"
"noperspective in vec2 v2UV;\n"
"out vec4 outputColor;\n"
"void main()\n"
"{\n"
"		outputColor = texture(mytexture, v2UV);\n"
"}\n"
;

const char c_TVShaderVert[] = R"glsl(

#version 450 core
layout (location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = texCoords;
    gl_Position =  projection * view * model * vec4(position, 1.0);
}

)glsl";

const char c_TVShaderFrag[] = R"glsl(

#version 450 core

in vec2 TexCoords;
layout(location = 0)out vec4 FragColor;

uniform sampler2D tex;

void main()
{
	//FragColor = texture(tex, TexCoords);
	FragColor = vec4(0.5,0.5,0.5,1);
}

)glsl";