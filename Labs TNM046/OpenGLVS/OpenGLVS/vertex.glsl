#version 330 core


uniform float time;
uniform mat4 model; //Model matrix (transformations on object)
uniform mat4 view; //View matrix (transformations on scene)
uniform mat4 projection; //Perspective Projection
uniform mat4 lightP;
uniform vec3 light;

layout (location = 0) in vec3 Position;   // the position variable has attribute position 0
layout (location = 1) in vec3 Normal; 
layout (location = 2) in vec2 Texture;
  
//Output to the fragment shader
out vec3 aNormal; 
out vec2 TexCoord;
out vec3 lightDirection;
out vec3 fragPos;
out vec3 lightPos;

//Initial light position
vec3 LightPos = vec3(0, 0, 1);

void main(){

    gl_Position = projection*view*model*vec4(Position, 1.0); 
	aNormal = mat3(transpose(inverse(view*model)))*Normal;
	fragPos = vec3(view*model*vec4(Position, 1.0));
	lightDirection = vec3(lightP*vec4(LightPos, 1.0));

    TexCoord = Texture;
}  