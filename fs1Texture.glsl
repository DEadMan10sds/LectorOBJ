#version 330 core

//debe de existir una variable de salida que es el color final
out vec4 FragColorOut;

in vec3 FragPos;
in vec3 FragNormal;
in vec3 FragColor;
in vec2 FragTextCoord;

uniform sampler2D Texture;
uniform vec3 colorUniform;
uniform float factorAmbiental;


vec4 Light(vec3 objectColor);

void main()
{
	//FragColorOut = Light(FragColor);
	vec4 color = texture(Texture, FragTextCoord);
	
	//Textura con color y sombras
	//FragColorOut = Light(FragColor * vec3(color.x, color.y, color.z));
	
	//Solo Textura
	//FragColorOut = texture(Texture, FragTextCoord);
	
	
	//Textura con sombras
	FragColorOut = Light(vec3(color.x, color.y, color.z));
}

vec4 Light(vec3 objectColor)
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightPos = vec3(0.0, 35.0, 0.0);
	vec3 viewPos = vec3(0.0, 35.0, 0.0);

	//Ambiental
	float ambientStrenght = 0.5;
	vec3 ambient = ambientStrenght * lightColor;

	//Difusa
	vec3 norm = normalize(FragNormal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//Specular
	float specularStrenght = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 refleDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, refleDir), 0.0), 32);
	vec3 specular = specularStrenght * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;


	return vec4(result, 1.0);
}