#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 newColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform float scaleX = 1.0f;
uniform float scaleY = 1.0f;

uniform float repeatX = 1.0;
uniform float repeatY = 1.0;

void main()
{
	vec4 scaledPos = vec4(aPos.x * scaleX, aPos.y * scaleY, aPos.z, 1.0);
	gl_Position = projection * view * model * scaledPos;	
	newColor = aColor;
    TexCoord = aTexCoord * vec2(repeatX, repeatY);
};