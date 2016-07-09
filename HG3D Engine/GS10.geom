#version 450

#define sqrt2 2//1.41421356237f

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 VAttribs[];

out vec4 FAttribs;
out vec2 Verts[3];

void main()
{
	vec2 movement;

	FAttribs=VAttribs[0];
    gl_Position = gl_in[0].gl_Position;
	Verts[0]=(gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w+1.0f)/2.0f;
	Verts[1]=(gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w+1.0f)/2.0f;
	Verts[2]=(gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w+1.0f)/2.0f;
	movement=Verts[0]*2.0f-Verts[1]-Verts[2];
	movement=movement/abs(movement)*sqrt2*gl_Position.w;
	//movement=movement*gl_Position.w/abs(dot(normalize(movement),vec2(sqrt2)));
	gl_Position+=vec4(movement/2048.0f,0.0f,0.0f);
    EmitVertex();

	FAttribs=VAttribs[1];
    gl_Position = gl_in[1].gl_Position;
	Verts[0]=(gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w+1.0f)/2.0f;
	Verts[1]=(gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w+1.0f)/2.0f;
	Verts[2]=(gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w+1.0f)/2.0f;
	movement=Verts[1]*2.0f-Verts[0]-Verts[2];
	movement=movement/abs(movement)*sqrt2*gl_Position.w;
	//movement=movement*gl_Position.w/abs(dot(normalize(movement),vec2(sqrt2)));
	gl_Position+=vec4(movement/2048.0f,0.0f,0.0f);
    EmitVertex();

	FAttribs=VAttribs[2];
    gl_Position = gl_in[2].gl_Position;
	Verts[0]=(gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w+1.0f)/2.0f;
	Verts[1]=(gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w+1.0f)/2.0f;
	Verts[2]=(gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w+1.0f)/2.0f;
	movement=Verts[2]*2.0f-Verts[0]-Verts[1];
	movement=movement/abs(movement)*sqrt2*gl_Position.w;
	//movement=movement*gl_Position.w/abs(dot(normalize(movement),vec2(sqrt2)));
	gl_Position+=vec4(movement/2048.0f,0.0f,0.0f);
    EmitVertex();

    EndPrimitive();
}