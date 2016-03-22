#version 450

in vec2 Vertex;

void main()
{
	gl_Position=vec4(Vertex,0.0f,1.0f);
}