#version 450

#define Shadowmap_Res			4096
#define conserved_pixels 1.0f

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

out float slope;
out float offset;

void main()
{
	vec2 movement;
	
	vec2 Verts[3];

	Verts[0]=gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w;
	Verts[1]=gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w;
	Verts[2]=gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w;

	vec2 delta[3];

	delta[0]=Verts[2]-Verts[1];
	delta[1]=Verts[0]-Verts[2];
	delta[2]=Verts[1]-Verts[0];
	
	delta[0]=normalize(delta[0]);
	delta[1]=normalize(delta[1]);
	delta[2]=normalize(delta[2]);

	vec2 P[2];
	vec4 diameter[2];


	P[0]=vec2(-delta[0].y,delta[0].x)*(abs(delta[0].y)+abs(delta[0].x))/float(Shadowmap_Res)*gl_in[1].gl_Position.w*3.0f;
	P[1]=P[0]/float(Shadowmap_Res)*gl_in[2].gl_Position.w/gl_in[1].gl_Position.w;

	slope=delta[0].y/delta[0].x;
	offset=Verts[2].y-Verts[2].x*slope;

	diameter[0]=gl_in[1].gl_Position+vec4(P[0],0.0f,0.0f);
	diameter[1]=gl_in[2].gl_Position-vec4(P[1],0.0f,0.0f);

    gl_Position = gl_in[1].gl_Position-vec4(P[0],0.0f,0.0f);
    EmitVertex();

    gl_Position = diameter[1];
    EmitVertex();
	
    gl_Position = diameter[0];
    EmitVertex();
	EndPrimitive();

    EmitVertex();

	gl_Position = gl_in[2].gl_Position+vec4(P[1],0.0f,0.0f);
    EmitVertex();

    gl_Position = diameter[1];
    EmitVertex();
	EndPrimitive();



	P[0]=vec2(-delta[1].y,delta[1].x)*(abs(delta[1].y)+abs(delta[1].x))/float(Shadowmap_Res)*gl_in[2].gl_Position.w*3.0f;
	P[1]=P[0]/float(Shadowmap_Res)*gl_in[0].gl_Position.w/gl_in[2].gl_Position.w;

	slope=delta[1].y/delta[1].x;
	offset=Verts[0].y-Verts[0].x*slope;

	diameter[0]=gl_in[2].gl_Position+vec4(P[0],0.0f,0.0f);
	diameter[1]=gl_in[0].gl_Position-vec4(P[1],0.0f,0.0f);

    gl_Position = gl_in[2].gl_Position-vec4(P[0],0.0f,0.0f);
    EmitVertex();

    gl_Position = diameter[1];
    EmitVertex();
	
    gl_Position = diameter[0];
    EmitVertex();
	EndPrimitive();

    EmitVertex();

	gl_Position = gl_in[0].gl_Position+vec4(P[1],0.0f,0.0f);
    EmitVertex();

    gl_Position = diameter[1];
    EmitVertex();
	EndPrimitive();


	
	P[0]=vec2(-delta[2].y,delta[2].x)*(abs(delta[2].y)+abs(delta[2].x))/float(Shadowmap_Res)*gl_in[0].gl_Position.w*3.0f;
	P[1]=P[0]/float(Shadowmap_Res)*gl_in[1].gl_Position.w/gl_in[0].gl_Position.w;

	slope=delta[2].y/delta[2].x;
	offset=Verts[1].y-Verts[1].x*slope;

	diameter[0]=gl_in[0].gl_Position+vec4(P[0],0.0f,0.0f);
	diameter[1]=gl_in[1].gl_Position-vec4(P[1],0.0f,0.0f);

    gl_Position = gl_in[0].gl_Position-vec4(P[0],0.0f,0.0f);
    EmitVertex();

    gl_Position = diameter[1];
    EmitVertex();
	
    gl_Position = diameter[0];
    EmitVertex();
	EndPrimitive();

    EmitVertex();

	gl_Position = gl_in[1].gl_Position+vec4(P[1],0.0f,0.0f);
    EmitVertex();

    gl_Position = diameter[1];
    EmitVertex();
	EndPrimitive();
}