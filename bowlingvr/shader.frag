#version 330 core

//black magic continues here
 
#define LINEAR_ATTENUATION 0.022
#define QUADR_ATTENUATION 0.0019 

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
in int EnabledLightCnt;

in vec4 FragPosLightSpace;
uniform mat4 lightSpaceMatrix;
uniform sampler2D shadowMap;


uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;
uniform float far_plane;

//out
out vec4 color;
     
uniform sampler2D texture_ambient1;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
	
uniform int ambientTexCount;
uniform int diffuseTexCount;
uniform int specularTexCount;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightPos)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
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
		//shadow = ShadowCalculation(FragPosLightSpace, Lights[i].position);
		
		//diffuse
		float diffuse = max(clamp( dot( N,L ), 0,1 ),0.05);
		
		vec4 diffuseF;
		if(diffuseTexCount == 0){
			diffuseF = Material.diffuse;
		}
		else
		{
			diffuseF = texture(texture_diffuse1, TexCoords);
		}

		colorFactor = diffuseF;
		//diffuseF *= Lights[i].diffuse;
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
			specular = pow(clamp( dot( E,R ), 0,1 ), Material.shininess);
		
		vec4 spec = vec4(0.0,0.0,0.0,1.0);
		
		if(specular >= 0.0)
		{
			spec = attenuation * specular * Lights[i].specular * Material.specular;		
		}
		 //ambient
		 vec4 amb = Lights[i].ambient * (texture(texture_ambient1, TexCoords) + Material.ambient);
		
		finalColor += (amb +  (diff + spec)) * colorFactor;

	}
	
	color = finalColor + Material.emission;
}

