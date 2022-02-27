#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Color;

out vec4 colorVertice;

void main()
{
    //gl_position en una variable out predefinida de tipo vec4
    gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0);

    colorVertice = vec4(Color.x, Color.y, Color.z, 1.0);
}