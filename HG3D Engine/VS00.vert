#version 450

#define MaxCascadessNums   1

in vec3 Vertex;
in ivec3 Normal;
in vec2 Coord;
in int bone_ID;

out vec3 normal;
out vec3 position;
out vec3 position_LV;
out vec2 coord;
out float cascade;


uniform vec3 Camera_Position;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;


void main()
{
	vec4 temp_position=ModelMatrix*vec4(Vertex,1);
	position_LV=Camera_Position-temp_position.xyz;
	position=temp_position.xyz;
	temp_position=ViewMatrix*temp_position;
	cascade=abs(-temp_position.z-1.0)/999.0*float(MaxCascadessNums*MaxCascadessNums);
	gl_Position=ProjectionMatrix*temp_position;
	
	coord=Coord;
	normal=(NormalMatrix*vec4(normalize(vec3(Normal.xyz)),0)).xyz;
}