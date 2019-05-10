# version 330 core

//Global variables
uniform float time;

in vec3 interpolatedNormal;
in vec2 st;
in vec3 lightDirection;
in vec3 fragPos;
out vec4 finalcolor;  
  
vec3 objectColor = vec3(1.0f, 0.5f, 0.5f);
vec3 lightColor = vec3(0.6f, 0.6f, 0.6f);
float ambientStrength = 0.1;
float specularStrength = 0.9;
vec3 viewPos = vec3(0.0, 0.0, 1.0);


void main()
{
	//Ambient lightning
	vec3 Ia = ambientStrength*lightColor;

	//Diffuse color
	vec3 lightDir = normalize(lightDirection - fragPos);
	float diff = max(dot(interpolatedNormal, lightDir), 0.0);
	vec3 Id = diff*lightColor;

	//Specular lighting
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, interpolatedNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 Is = specularStrength * spec * lightColor;
	
	//Shading normals and resulting color
    float shading = dot(interpolatedNormal, lightDirection);
    shading = max(0.0, shading);

	vec3 result = (Ia+Id+Is)*objectColor;
	finalcolor = vec4(result, 1.0);

	//Set final color to white for testing perspective projection
	//finalcolor = vec4(1.0, 1.0, 1.0, 1.0);
}