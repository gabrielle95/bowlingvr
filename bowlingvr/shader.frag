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
		
		finalColor += (amb + (1.0 - shadow) * (diff + spec) * colorFactor) + Material.emission;
		//finalColor += diff * colorFactor + Material.emission;

	}
	
	FragColor = finalColor;
}

