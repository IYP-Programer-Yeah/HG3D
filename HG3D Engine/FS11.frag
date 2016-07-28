//shadow map shader
#version 450

layout(early_fragment_tests) in; //later will be added if it was availble

out vec2 Output1;

in float slope;
in float offset;

void main()
{
	Output1=vec2(slope, offset);
}