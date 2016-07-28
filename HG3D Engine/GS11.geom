#version 450

#define Shadowmap_Res			4096
#define conserved_pixels 1.0f

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out float slope;
out float offset;

void main()
{
	vec2 movement;
	
	vec2 Verts[3];

	Verts[0]=(gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w+1.0f)/2.0f;
	Verts[1]=(gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w+1.0f)/2.0f;
	Verts[2]=(gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w+1.0f)/2.0f;

	vec2 delta[3];

	delta[0]=Verts[2]-Verts[1];
	delta[1]=Verts[0]-Verts[2];
	delta[2]=Verts[1]-Verts[0];
	
	delta[0]=normalize(delta[0]);
	delta[1]=normalize(delta[1]);
	delta[2]=normalize(delta[2]);

	vec2 P;
	vec2 diameter[2];


	P=vec2(-delta[0].y,delta[0].x);

	slope=delta[0].y/delta[0].x;
	offset=Verts[2].y-Verts[2].x*slope;
    gl_Position = gl_in[0].gl_Position;
	gl_Position.xy+=(delta[1]*abs(dot(delta[1],P[2]))-delta[2]*abs(dot(delta[2],P[1])))*gl_Position.w/Shadowmap_Res;
    EmitVertex();



	slope=delta[1].y/delta[1].x,0.0f;
	offset=Verts[0].y-Verts[0].x*slope;
    gl_Position = gl_in[1].gl_Position;
	gl_Position.xy+=(delta[2]*abs(dot(delta[2],P[0]))-delta[0]*abs(dot(delta[0],P[2])))*gl_Position.w/Shadowmap_Res;
    EmitVertex();


	slope=delta[2].y/delta[2].x;
	offset=Verts[1].y-Verts[1].x*slope;
    gl_Position = gl_in[2].gl_Position;
	gl_Position.xy+=(delta[0]*abs(dot(delta[0],P[1]))-delta[1]*abs(dot(delta[1],P[0])))*gl_Position.w/Shadowmap_Res;
    EmitVertex();

    EndPrimitive();
}