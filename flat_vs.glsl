#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 textCoord;

out vec3 FragPos;
out vec3 FragColor;
out vec3 FragNormal;
out vec2 FragTextCoord;

flat out vec3 faceNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * model * vec4(pos, 1.0);
    vec3 normalTransformada = mat3(transpose(inverse(model))) * normal;

    FragPos = vec3(model * vec4(pos, 1.0));
    FragNormal = normalTransformada;
    faceNormal = FragNormal;
    FragColor = color;
    FragTextCoord = textCoord;
}