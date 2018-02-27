#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 mvpMatrix; //proj * view * model
uniform mat4 uMVMatrix; // view * model
uniform mat4 uPMatrix; // proj

void main()
{
        gl_Position = uPMatrix * uMVMatrix * vec4(position, 1.0f);
        FragPos = vec3(uMVMatrix * vec4(position, 1.0f));
        Normal = mat3(transpose(inverse(uMVMatrix))) * normal;
        TexCoords = texCoords;
}