#version 330 core

//Uniform variables
uniform  float  time;
uniform  mat4 T;
uniform  mat4 R;
uniform  mat4 stackTransf;

layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
  
out vec3 ourColor; // output a color to the fragment shader

void main(){
    gl_Position = stackTransf*vec4(aPos, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}  