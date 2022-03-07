#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 FragPos;
out vec3 FragNormal;
out vec3 FragColor;



void main()
{
    vec4 PosVertice = vec4(Pos.x, Pos.y, Pos.z, 1.0);

    gl_Position = projection * view * model * PosVertice;

    FragPos = vec3(model*vec4(Pos, 1.0));
    FragNormal = mat3(transpose(inverse(model))) * Normal;
    FragColor = Color;
}