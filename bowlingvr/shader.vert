#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model; //proj * view * model
uniform mat4 view; // view * model
uniform mat4 projection; // proj

void main()
{
		FragPos = vec3(model * vec4(position, 1.0));
		Normal = mat3(transpose(inverse(model))) * normal;  
		gl_Position = projection * view * vec4(FragPos, 1.0);
        //gl_Position = uPMatrix * uVMatrix * vec4(position, 1.0f);
		//gl_Position = mvpMatrix * vec4(position, 1.0f);
        //FragPos = vec3(uMVMatrix * vec4(position, 1.0f));
        //Normal = mat3(transpose(inverse(uMVMatrix))) * normal;
        TexCoords = texCoords;
}