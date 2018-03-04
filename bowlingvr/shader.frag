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
in vec3 lightDir[10];
in vec3 EyeDirection_cameraspace;


out vec4 color;
     
uniform sampler2D texture_ambient1;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
	
uniform int ambientTexCount;
uniform int diffuseTexCount;
uniform int specularTexCount;
    
void main()
{
	//vec3 lightDir[i];
	float radius = 1.0;
	
	vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);
	
	vec3 N = normalize(Normal); //normal
	
	float constantAtt = 1.0;
	
	for(int i = 0; i < MaxLights; i++)
	{
		if(!Lights[i].isEnabled)
			continue;
		
		//attenuation
		float attenuation, dist;
		dist = length(Lights[i].position - FragPos);
		attenuation = 1.0 / (constantAtt + LINEAR_ATTENUATION * dist + QUADR_ATTENUATION * dist * dist);
		
		vec3 L = normalize(lightDir[i]); //light direction
		
		//diffuse
		float diffuse = max(clamp( dot( N,L ), 0,1 ),0.05);
		
		vec4 diffuseF;
		if(diffuseTexCount == 0)
			diffuseF = Material.diffuse * Lights[i].diffuse;
		else
			diffuseF = texture(texture_diffuse1, TexCoords) * Lights[i].diffuse;
			
		vec4 diff = attenuation * diffuse * diffuseF;
		
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
			if(specularTexCount == 0)
				spec = attenuation *  specular * Material.specular * Lights[i].specular;
			else
				spec = attenuation *  specular * texture(texture_specular1, TexCoords) * Lights[i].specular;
				
		}
			
		
		finalColor +=  diff + spec + Material.emission;
		
		//add ambient
		if(diffuseTexCount == 0)
			finalColor += Lights[i].ambient * (texture(texture_ambient1, TexCoords) + Material.ambient);
	}
	
	color = finalColor;
}
