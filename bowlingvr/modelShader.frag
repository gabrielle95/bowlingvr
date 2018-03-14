    #version 330 core
    
	struct LightProperties {
	bool isEnabled;
	//bool isLocal;
	//bool isSpot;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
	vec3 direction;
	//vec3 halfVector;
	//vec3 coneDirection;
	//float spotCosCutoff;
	//float spotExponent;
	float constantAttenuation;
	//float linearAttenuation;
	//float quadraticAttenuation;
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
    out vec4 color;
     
    uniform sampler2D texture_ambient1;
    uniform sampler2D texture_diffuse1;
    uniform sampler2D texture_specular1;
	
	uniform int ambientTexCount;
	uniform int diffuseTexCount;
	uniform int specularTexCount;
    
	uniform vec3 viewPos;
     
    void main()
    {
		vec4 ambient;
		vec4 diffuse;
		vec4 specular = vec4(0.0,0.0,0.0,0.0);
		
		for (int light = 0; light < MaxLights; light++) {
			if (! Lights[light].isEnabled)
				continue;
            // Ambient
            ambient += Lights[light].ambient * (texture(texture_ambient1, TexCoords) + Material.ambient);
     
            // Diffuse
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(Lights[light].position - FragPos);
			//vec3 lightDir = normalize(-Lights[light].position);
			//float lightDistance = length(lightDir);
			
            float diff = max(dot(norm, lightDir), 0.05);
            if (diffuseTexCount == 0) {
                    diffuse +=  Lights[light].diffuse * diff * Material.diffuse;
            }
            else {
                    diffuse +=  Lights[light].diffuse * diff * texture(texture_diffuse1, TexCoords);
            }
           
            // Specular
            vec3 viewDir = -normalize(FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            if(Material.shininess > 1.0) {
					float spec;
					
					if(diff == 0)
						spec = 0;
					else
						spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.shininess);
						
                    if (specularTexCount == 0) {
                            specular += Lights[light].specular * spec * Material.specular;
                    }
                    else {
                            specular += Lights[light].specular * spec * texture(texture_specular1, TexCoords);
                    }
            }
     

		}
		
		//vec3 final = vec3(diffuse.rgb + specular.rgb + Material.emission.rgb);
		//final = min(final, vec3(1.0));
		//color = vec4(final, diffuse.a);
		color = diffuse + specular + Material.emission;
        if (diffuseTexCount == 0) {
            color += ambient;
        }
		//if(color.a > 1.0)
			//color.a = 1.0;
    }
