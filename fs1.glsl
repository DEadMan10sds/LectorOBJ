#version 330 core

//debe de existir una variable de salida que es el color final
out vec4 FragColor;

in vec4 coloVertice;

void main()
{
    FragColor = vec4(coloVertice.x, coloVertice.y, coloVertice.z, 1.0f);
} 