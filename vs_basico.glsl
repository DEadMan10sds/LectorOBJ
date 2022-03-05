#version 330 core

layout (location = 0) in vec3 verPos;
layout (location = 1) in vec3 verNormal;
layout (location = 2) in vec3 verCol;

uniform mat4x4 model;
uniform mat4x4 view;
uniform mat4x4 projection;

out vec3 fragColor;

void main()
{
    gl_Position = projection * view * model * vec4(verPos, 1.0f);
    fragColor = verCol;
}