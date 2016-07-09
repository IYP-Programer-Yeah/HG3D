//shadow map shader
#version 450

//#extension GL_NV_conservative_raster : require

#define Shadowmap_Res				2048								//shadow map resolution 


layout(early_fragment_tests) in; //later will be added if it was availble

out float Output1;
out float Output2;
out float Output3;

in vec4 FAttribs;
in vec2 Verts[3];

void main()
{
	if (FAttribs.w<0)
		discard;
	Output1=FAttribs.w;


	int vertices[2];
	if (FAttribs.x<FAttribs.y&&FAttribs.x<FAttribs.z)
	{
		vertices[0]=1;
		vertices[1]=2;
	}
	else if (FAttribs.y<FAttribs.x&&FAttribs.y<FAttribs.z)
	{
		vertices[0]=0;
		vertices[1]=2;
	}
	else
	{
		vertices[0]=1;
		vertices[1]=0;
	}
	const vec2 v1=Verts[vertices[0]];
	const vec2 v2=Verts[vertices[1]];
	const vec2 delta=v1-v2;

	Output2=delta.y/delta.x;
	Output3=v1.y-v1.x*Output2;
}
