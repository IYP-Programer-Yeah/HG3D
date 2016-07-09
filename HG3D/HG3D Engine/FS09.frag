//bloom shader mix
#version 450


in vec2 FragCoord;


out vec3 Output1;

uniform sampler2D Texture;
uniform sampler2D AliasedImage;

void main()
{
	Output1=texture(Texture,FragCoord).xyz;
	Output1+=texture(AliasedImage,FragCoord).xyz;
}