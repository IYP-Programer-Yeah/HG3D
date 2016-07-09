#version 450

in vec2 Vertex;

out vec2 FragCoord;

void main()
{
	FragCoord=(Vertex+1.0f)/2;
	gl_Position=vec4(Vertex,0.0f,1.0f);
}