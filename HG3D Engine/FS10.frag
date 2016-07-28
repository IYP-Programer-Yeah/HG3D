//shadow map shader
#version 450

layout(early_fragment_tests) in; //later will be added if it was availble

out float Output1;
/*out float Output2;
out float Output3;*/

in float Z;

//in vec4 VAttribs;
/*in vec3 slope;
in vec3 offset;*/

void main()
{
	if (Z<0)
		discard;
	Output1=Z;


	/*int vertices[2];
	if (FAttribs.x<FAttribs.y&&FAttribs.x<FAttribs.z)
	{
		Output2=slope.x/FAttribs.x;
		Output3=offset.x/FAttribs.x;
	}
	else if (FAttribs.y<FAttribs.x&&FAttribs.y<FAttribs.z)
	{
		Output2=slope.y/FAttribs.y;
		Output3=offset.y/FAttribs.y;
	}
	else
	{
		Output2=slope.z/FAttribs.z;
		Output3=offset.z/FAttribs.z;
	}*/
}
