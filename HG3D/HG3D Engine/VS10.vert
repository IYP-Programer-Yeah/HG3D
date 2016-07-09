#version 450

in vec3 Vertex;
in int bone_ID;

out vec4 Position;
out vec3 VertDisp;
out vec2 Verts[3];

uniform mat4 ProjMatrix;
uniform mat4 ModelMatrix;

void main()
{
	Position=ModelMatrix*vec4(Vertex,1);
	gl_Position=ProjMatrix*Position;

	Verts[0]=vec2(0);
	Verts[1]=vec2(0);
	Verts[2]=vec2(0);


	Verts[gl_VertexID%3]=(gl_Position.xy/gl_Position.w+1.0f)/2.0f;

	VertDisp=vec3(0);
	VertDisp[gl_VertexID%3]=1.0f;
}