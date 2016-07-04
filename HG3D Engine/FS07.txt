//high pass shader

#version 450

#define MinLuma 0.9

in vec2 FragCoord;


out vec3 Output1;

uniform sampler2D Texture;


float luma(vec3 RGB)
{
	return 0.2126*RGB.x + 0.7152*RGB.y + 0.0722*RGB.z;
}

void main()
{
	Output1=texture(Texture,FragCoord).xyz;

	if (luma(Output1)<MinLuma)
		Output1=vec3(0.0f);		
		
}