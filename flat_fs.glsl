#version 330 core

//debe de existir una variable de salida que es el color final
out vec4 FragColorOut;

in vec3 FragColor; //|Debe tner el mismo nombre que en el vertexshader|
in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragTextCoord;

flat in vec3 faceNormal;

uniform vec3 colorUniform;
uniform float factorAmbiental;

vec4 Light(vec3 objectColor);

void main(){
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightPos = vec3(1.2f, 1.0f, 5.0f);
    vec3 viewPos = vec3(1.0f, 0.0f, -4.0f);

    //ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(faceNormal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * FragColor;

	FragColorOut = vec4(((ambient + diffuse + specular) * FragColor), 1.0f);
}