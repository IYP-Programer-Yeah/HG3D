#version 450

in vec3 Vertex;
in int bone_ID;

uniform mat4 ProjMatrix;
uniform mat4 ModelMatrix;

void main()
{
	gl_Position=ProjMatrix*(ModelMatrix*vec4(Vertex,1));
}