#version 460 core
out vec4 FragColor;
layout (depth_greater) out float gl_FragDepth;

uniform vec4 color;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

in VS_OUT
{
	vec2 TexCoords;
} fs_in;

void main()
{
	
	/*
	if(gl_FrontFacing)
		FragColor = texture(frontTexture, TexCoords);
	else
		FragColor = texture(backTexture, TexCoords);
	*/

	//FragColor = vec4(1.0);
	//gl_FragDepth = gl_FragCoord.z + 0.6;
	//FragColor = texture(frontTexture, fs_in.TexCoords);
	FragColor = color;
}