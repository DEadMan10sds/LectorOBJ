#version 330 core

//debe de existir una variable de salida que es el color final
out vec4 FragColor;

in vec4 colorVertice;

void main()
{
    FragColor = vec4(colorVertice.x, colorVertice.y, colorVertice.z, 1.0f);
} 