#version 330 core

out vec4 FragColorOut;

in vec3 FragPos;
in vec3 FragNormal;
in vec3 FragColor;

flat in vec3 faceNormal;

uniform vec3 colorUniform;
uniform float factorAmbiental;

vec4 Light(vec3 objectColor);

void main()
{
   FragColorOut = Light(FragColor);
}

vec4 Light(vec3 objectColor)
{
 vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightPos = vec3(0.0, 0.0, 10.0);
	vec3 viewPos = vec3(0.0, 0.0, 10.0);

    //Ambiental
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //Difusa
    vec3 norm = normalize(faceNormal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    //Especular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * FragColor;

	vec3 result  = (ambient + diffuse + specular) * FragColor;

	return vec4(result, 1.0);


}