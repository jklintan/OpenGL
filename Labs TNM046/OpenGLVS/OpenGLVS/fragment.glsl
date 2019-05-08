# version 330 core

//Global variables
uniform float time;

in vec3 interpolatedNormal;
in vec2 st;
in vec3 lightDirection;
out vec4 finalcolor;  
  
vec3 objectColor = vec3(0.0f, 0.0f, 1.0f);
vec3 lightColor = vec3(0.4f, 0.4f, 0.4f);
vec3 diffuseColor = vec3(1.0f, 0.0f, 0.0f);
//vec3 lightDirection = vec3(0.7, 0.5, 1.0);
float ambientStrength = 5;
float specularStrength = 0.8;
float diffuseStrength = 0.8;
vec3 viewPos = vec3(0.0, 0.0, 1.0);


void main()
{
	//Ambient lightning
	vec3 ambient = ambientStrength*lightColor;
	vec3 Ia = ambient*objectColor;

	//Diffuse color
	float diff = max(dot(interpolatedNormal, lightDirection), 0.0);
	vec3 Id = diffuseStrength*diff*diffuseColor;

	//Specular lighting
	vec3 reflectDir = reflect(-lightDirection, interpolatedNormal);
	float spec = pow(max(dot(viewPos, reflectDir), 0.0), 4);
	vec3 Is = specularStrength * spec * lightColor;
	
	//Shading normals and resuling color
    float shading = dot(interpolatedNormal, lightDirection);
    shading = max(0.0, shading);

	vec3 result = (Ia+Id+Is)*lightColor;
	finalcolor = vec4(result*vec3(shading), 1.0);
}