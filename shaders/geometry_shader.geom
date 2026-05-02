#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

out vec3 fColor;

in VS_OUT {
	vec2 TexCoords;
	vec3 FragPos;
	mat3 TBN;
} gs_in[];

out GS_OUT {
	vec2 TexCoords;
	vec3 FragPos;
	mat3 TBN;
} gs_out;
/*
void build_house(vec4 position)
{
	fColor = gs_in[0].color;
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);
	fColor = vec3(1.0, 1.0, 1.0);
	EmitVertex();

	EndPrimitive();

}
*/
void main()
{
	//build_house(gl_in[0].gl_Position);
	gs_out.TexCoords = gs_in[0].TexCoords;
	gs_out.FragPos = gs_in[0].FragPos;
	gs_out.TBN = gs_in[0].TBN;

}