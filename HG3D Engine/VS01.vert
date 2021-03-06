#version 450

in vec3 Vertex;
in ivec3 Normal;
in vec2 Coord;
in int bone_ID;

uniform mat4 ProjViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
	gl_Position=ProjViewMatrix*(ModelMatrix*vec4(Vertex,1));
}
