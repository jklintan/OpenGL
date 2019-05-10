# version 330 core

//Global variables
uniform float time;

in vec3 aNormal;
in vec2 st;
in vec3 fragPos;
out vec4 fragColor;  
in vec3 lightPos;

//Lightning model according to the Phong lighting model
float ambientStrength = 0.5;
float specularStrength = 0.9;

vec3 objectColor = vec3(0.3f, 0.0f, 0.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main()
{
	//Ambient lightning
	vec3 Ia = ambientStrength*lightColor;

	//Diffuse color
	vec3 norm = normalize(aNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 Id = diff*lightColor;

	//Specular lighting
	vec3 viewDir = normalize(lightPos-fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 Is = specularStrength * spec * lightColor;

	vec3 result = (Ia+Id+Is)*objectColor;
	fragColor = vec4(result, 1.0);
}