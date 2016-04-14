#version 450

in vec3 Vertex;
in ivec3 Normal;
in vec2 Coord;
in unsigned int bone_ID;

out vec3 normal;
out vec2 coord;

struct CameraData
{
	mat4 ViewMatrix;//modelview matrix
	mat4 ProjectionMatrix;//projection
	vec4 veiwport_dimensions;

	float Left, Right, Buttom, Top, Near, Far;//projection property
	vec3 Position;
	float data[19];
};
layout (std140) uniform camera
{
		CameraData CurrentCamera;
};

struct MeshData
{
	mat4 ModelMatrix;          //model matrix will be sent to shader
	mat4 NormalMatrix;
	unsigned int MatID;
	unsigned int have_diff_text;              //have diffuse texture? 
	unsigned int have_roughness_text;         //have roughness texture? 
	unsigned int have_NM_text;                //have normals map texture? 
	unsigned int have_HM_text;                //have height map texture? 
	unsigned int have_mask_text;              //have mask texture? 
	unsigned int have_alphamap_text;		  //have alpha map texture?
	unsigned int have_dispmap_text;			  //have displacement map texture?
	unsigned int have_TFmap_text;			  //have transmision filter map texture?
	unsigned int Padding[23];
};

layout (std140) uniform mesh
{
		MeshData CurrentMesh;
};


void main()
{
	gl_Position=CurrentCamera.ProjectionMatrix*(CurrentCamera.ViewMatrix*(CurrentMesh.ModelMatrix*vec4(Vertex,1.0f)));
	normal=(CurrentCamera.ViewMatrix*vec4((CurrentMesh.NormalMatrix*vec4(normalize(vec3(Normal)),0.0f)).xyz,0.0f)).xyz;
	coord=Coord;
}
