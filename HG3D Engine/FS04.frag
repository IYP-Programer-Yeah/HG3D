#version 450

#define GBufferTextNums			   3

#define PI                  3.14159f			//pi

#define MaxLightNums	   32

#define PCF
#define MaxShadowmapsNums  8
#define MaxCascadessNums   1


in vec2 FragCoord;


out vec3 Output1;

uniform sampler2D GBuffer[GBufferTextNums-2];
uniform isampler2D GBufferNormalMap;
uniform usampler2D GBufferMatID;
uniform sampler2D GBufferDepthMap;
uniform sampler2DArray SMTex_Sampler[MaxCascadessNums];

uniform int CurrentLightIndex;

struct light
{
	vec3 light_position;//light pos

	float max_radius;//the maximum radius of light contribution

	vec3 light_color;//light color

	float cut_off_cos;//cosine of the the the cut of angle 

	vec3 Attenuation;//attenuation values

	float edge_cut_off_cos_delta;

	vec3 direction;//the direction of light

	int nonesense1;
		
};

layout (std140) uniform lights_mat
{
		mat4 LightMat[MaxShadowmapsNums*2];
};

layout (std140) uniform lights
{
		light Light[MaxLightNums];
};
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
float SpecTerm(float a, vec3 L, vec3 V, vec3 N)
{
    vec3 H = normalize(L+V);

    float NoV = dot(N, V);
    float NoL = dot(N, L);
    float a2 = a*a;

    float D = pow(max(0.0f,dot(N, H)), 2.0f / (a2 * a2) - 2.0f) / (a2 * a2 * PI);
    //float D = pow(max(0.0f,dot(V,L)), 2.0f / (a2 * a2) - 2.0f) / (a2 * a2 * PI);
    float G = NoL * NoV;
    float denom = 4.0f * NoL * NoV;

	if (denom<=0.0f)
		return 0;
    float val = D * G / denom;
    return max(val,0.0f);
}

void main()
{
	/****************************************************************************/
	/****************************************************************************/
	/***********************************get the light****************************/
	/****************************************************************************/
	const light CurrentLight=Light[CurrentLightIndex];
	/****************************************************************************/
	/***********************************get the light****************************/
	/****************************************************************************/
	/****************************************************************************/
	const float roughness=0.6;
	const float a=roughness*roughness;

	/****************************************************************************/
	/****************************************************************************/
	/***************read depth and reconstruct the view space pos****************/
	/****************************************************************************/
	const float depth=texture(GBufferDepthMap,FragCoord).x;

	const float z=CurrentCamera.Near/(depth*(CurrentCamera.Far-CurrentCamera.Near)/CurrentCamera.Far-CurrentCamera.Near);

	const vec3 VSPos=vec3((FragCoord*vec2(CurrentCamera.Right-CurrentCamera.Left,CurrentCamera.Top-CurrentCamera.Buttom)+vec2(CurrentCamera.Left,CurrentCamera.Buttom))*abs(z/CurrentCamera.Near),z);
	/****************************************************************************/
	/***************read depth and reconstruct the view space pos****************/
	/****************************************************************************/
	/****************************************************************************/


	const bool SMAvailble=(CurrentLightIndex<MaxShadowmapsNums)&&(CurrentLight.nonesense1>16777215||CurrentLight.nonesense1<0);

	if (SMAvailble)
	{
		const mat4 ViewMat = LightMat[CurrentLightIndex * 2];
		const mat4 ProjMat = LightMat[CurrentLightIndex * 2 + 1];
		const vec4 SMVSPos = ViewMat * vec4(VSPos,1.0f);
		const vec4 SMCoord = ProjMat *SMVSPos;

		if (SMCoord.x<1.0f && SMCoord.y<1.0f && SMCoord.y>0.0f && SMCoord.x>0.0f && SMCoord.z<1.0f && SMCoord.z>0.0f)
		{
			Output1=texture(SMTex_Sampler[0],vec3(SMCoord.xy,0)).xyz/1000;
			return;
		}

		
	}

	/****************************************************************************/
	/****************************************************************************/
	/************************discard the unwanted fragment***********************/
	/****************************************************************************/
	const vec3 TempL=CurrentLight.light_position-VSPos;
	const float L_Length=-dot(TempL,CurrentLight.direction);
	if (L_Length>CurrentLight.max_radius||L_Length < 0)
		discard;
	const vec3 L=-CurrentLight.direction;
	/****************************************************************************/
	/************************discard the unwanted fragment***********************/
	/****************************************************************************/
	/****************************************************************************/
	
	/****************************************************************************/
	/****************************************************************************/
	/**************************read the rest of the data*************************/
	/****************************************************************************/
	const unsigned int MatID=texture(GBufferMatID,FragCoord).x;
	const vec4 Texture=vec4(texture(GBuffer[0],FragCoord));

	const vec4 ReadNormal=vec4(texture(GBufferNormalMap,FragCoord));

	const vec3 normal=normalize(ReadNormal.xyz);
	/****************************************************************************/
	/**************************read the rest of the data*************************/
	/****************************************************************************/
	/****************************************************************************/

	/****************************************************************************/
	/****************************************************************************/
	/***********************calculate the attinuation effect*********************/
	/****************************************************************************/
	const float Attenuation=CurrentLight.Attenuation[0]+L_Length*(CurrentLight.Attenuation[1]+L_Length*CurrentLight.Attenuation[2]);
	/****************************************************************************/
	/***********************calculate the attinuation effect*********************/
	/****************************************************************************/
	/****************************************************************************/


	const vec3 V=-normalize(VSPos);

	Output1=vec3(max(dot(L,normal),0.0));
	Output1+=vec3(SpecTerm(a,L,V,normal));

	Output1=max(Output1*CurrentLight.light_color*Texture.xyz/Attenuation,vec3(0.0f));
	
}