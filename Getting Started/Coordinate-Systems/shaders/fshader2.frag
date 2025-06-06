#version 460 core
out vec4 FragColor;
uniform vec4 ourColor;
in vec3 vertPos;

void main()
{
 FragColor = vec4(vertPos, 1.0);
};