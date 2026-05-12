#version 460 core
layout (depth_greater) out float gl_FragDepth;

out vec4 FragColor;

in vec2 TexCoords;                                                  
uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main()
{
	
	/*
	if(gl_FrontFacing)
		FragColor = texture(frontTexture, TexCoords);
	else
		FragColor = texture(backTexture, TexCoords);
	*/

	FragColor = vec4(1.0);
	gl_FragDepth = gl_FragCoord.z + 0.1;
}