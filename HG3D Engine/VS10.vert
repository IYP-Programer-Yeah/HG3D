#version 450

in vec3 Vertex;
in int bone_ID;

out vec4 VAttribs;//Z, VertexWeight

uniform mat4 ProjMatrix;
uniform mat4 ModelMatrix;

void main()
{
	VAttribs=vec4(0.0f);
	gl_Position=ModelMatrix*vec4(Vertex,1);
	VAttribs.w=-gl_Position.z;
	gl_Position=ProjMatrix*gl_Position;
	VAttribs[gl_VertexID%3]=1.0f;
}