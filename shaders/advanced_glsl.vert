#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;


layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

layout (std140) uniform ExampleBlock
{
					 //base alignment      //aligned offset
	float value;     //0                    0
	vec3 vector;     //16                   16
	mat4 matrix;     //16                   32
					 //16                   48     
					 //16                   64
					 //16                   80
	float values[3]; //16                   96
					 //16                   112
					 //16                   128
	bool boolean;    //4                    144
	int integer;     //4                    148
};

uniform mat4 model;


out VS_OUT
{
	vec2 TexCoords;
} vs_out;


void main()
{
	gl_Position = projection * view  * model * vec4(aPos, 1.0);
	vs_out.TexCoords = aTexCoords;
}