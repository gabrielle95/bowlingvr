#version 330 core

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
uniform vec3 LightPosition_worldspace[10];

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
		FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0); //light camera space
		//EnabledLightCnt = enabledLights;

}