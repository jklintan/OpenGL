# version 330 core

//Global variables
uniform float time;

in vec3 ourColor;
out vec4 FragColor;  
  
void main()
{
    FragColor = vec4(ourColor, 1.0);
}