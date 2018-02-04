    #version 330 core
     
    in vec3 FragPos;
    in vec3 Normal;
    in vec2 TexCoords;
     
    out vec4 color;
     
    uniform sampler2D texture_ambient1;
    uniform sampler2D texture_diffuse1;
    uniform sampler2D texture_specular1;
     
    uniform vec4 ambientColor;
    uniform vec4 diffuseColor;
    uniform vec4 specularColor;
    uniform vec4 emissiveColor;
    uniform float shininess;
    uniform int ambientTexCount;
    uniform int diffuseTexCount;
    uniform int specularTexCount;
     
    uniform vec4 light_ambient;
    uniform vec4 light_diffuse;
    uniform vec4 light_specular;
    uniform vec3 light_position;
     
    void main()
    {
            // Ambient
            vec4 ambient = light_ambient * (texture(texture_ambient1, TexCoords) + ambientColor);
     
            // Diffuse
            vec4 diffuse;
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(light_position - FragPos);
            float diff = max(dot(norm, lightDir), 0.05);
            if (diffuseTexCount == 0) {
                    diffuse =  light_diffuse * diff * diffuseColor;
            }
            else {
                    diffuse =  light_diffuse * diff * texture(texture_diffuse1, TexCoords);
            }
           
            // Specular
            vec4 specular = vec4(0.0,0.0,0.0,0.0);
            vec3 viewDir = -normalize(FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            if(shininess > 1.0) {
                    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
                    if (specularTexCount == 0) {
                            specular = light_specular * spec * specularColor;
                    }
                    else {
                            specular = light_specular * spec * texture(texture_specular1, TexCoords);
                    }
            }
     
            color = diffuse + specular + emissiveColor;
            if (diffuseTexCount == 0) {
                    color += ambient;
            }
    }
