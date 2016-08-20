#version 450

in vec3 Vertex;
in int bone_ID;

out vec4 VAttribs;//VertexWeight, Z

uniform mat4 ProjMatrix;
uniform mat4 ModelMatrix;

void main()
{
	gl_Position=ModelMatrix*vec4(Vertex,1);
	VAttribs=vec4(0.0f,0.0f,0.0f,-gl_Position.z);
	gl_Position=ProjMatrix*gl_Position;
	VAttribs[gl_VertexID%3]=1.0f;
}