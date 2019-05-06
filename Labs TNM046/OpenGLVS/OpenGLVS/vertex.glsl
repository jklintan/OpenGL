#version 330 core

//Uniform variables
uniform float time;
uniform mat4 T;
uniform mat4 R;
uniform mat4 stackTransf;

layout (location = 0) in vec3 Position;   // the position variable has attribute position 0
layout (location = 1) in vec3 Normal; 
layout (location = 2) in vec2 TexCoord;
  
out vec3 interpolatedNormal; // output a color to the fragment shader
out vec2 st;
out vec3 lightDirection;

void main(){
    gl_Position = stackTransf*time*vec4(Position, 1.0);
    vec3 transformedNormal = mat3(stackTransf)*Normal;
	interpolatedNormal = normalize(transformedNormal);
    st = TexCoord;
	vec4 result = stackTransf*vec4(1.0, 1.0, 1.0, 1.0);
	lightDirection = vec3(result);
}  