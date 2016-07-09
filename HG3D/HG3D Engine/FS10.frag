//shadow map shader
#version 450

//#extension GL_NV_conservative_raster : require

#define Shadowmap_Res				2048								//shadow map resolution 


layout(early_fragment_tests) in; //later will be added if it was availble

in vec3 VertDisp;

in vec2 Verts[3];


out float Output1;
out float Output2;
out float Output3;

in vec4 Position;

void main()
{
	Output1=-Position.z;//length(Position.xyz);
	if (Output1<0)
		discard;
	/*
	Output2=vec2(VertDisp);//fract(((VertDisp.xy/VertDisp.z+1.0f)/2.0f)*Shadowmap_Res);
	*/
	int vertices[2];
	if (VertDisp.x<VertDisp.y&&VertDisp.x<VertDisp.z)
	{
		vertices[0]=1;
		vertices[1]=2;
	}
	else if (VertDisp.y<VertDisp.x&&VertDisp.y<VertDisp.z)
	{
		vertices[0]=0;
		vertices[1]=2;
	}
	else
	{
		vertices[0]=1;
		vertices[1]=0;
	}
	const vec2 v1=Verts[vertices[0]]/VertDisp[vertices[0]];
	const vec2 v2=Verts[vertices[1]]/VertDisp[vertices[1]];
	const vec2 delta=v1-v2;

	Output2=delta.y/delta.x;
	Output3=v1.y-v1.x*Output2;
}
