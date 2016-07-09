#version 450

#define PI                  3.14159f			//pi

layout(early_fragment_tests) in; //later will be added if it was availble

in vec3 normal;
in vec2 coord;

out ivec4 Output1;
out vec4 Output2;
out unsigned int Output3;

uniform sampler2D Diffuse_Texture;

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
	const vec3 ABSNormal = abs(normal);
	const float MaxYZ=max(ABSNormal.y, ABSNormal.z);
	const float MaxComponent = max(ABSNormal.x, MaxYZ);
	const float SecondMaxFract =(MaxComponent == ABSNormal.x ? MaxYZ : max(ABSNormal.x,min(ABSNormal.y, ABSNormal.z)))/MaxComponent;
	const float Coef = min(1.0f / min(SecondMaxFract, 1.0f - SecondMaxFract), 127.0f);

	const vec3 BFN = round(normal*(round(floor(127.0f / Coef)*Coef)/MaxComponent));

	Output1=ivec4(BFN,0.0f);
	if (CurrentMesh.have_diff_text==0)
		Output2=vec4(1.0f);
	else
		Output2=vec4(texture(Diffuse_Texture,coord).xyz,0.0f);
	Output3=CurrentMesh.MatID;
}