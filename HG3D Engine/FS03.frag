#version 450

#define GBufferTextNums			3

#define PI						3.14159f			//pi

#define MaxLightNums			32


#define MaxShadowmapsNums		8
#define MaxCascadessNums		1

#define SampleCount				4
#define Shadowmap_Res			2048

#define Cos30					0.8660254f
#define Sin30					0.5f

#define TargetCount				4

#define GN(x)				(pow(e,-x*x/(2.0f*Sigma*Sigma))/pow(2.0f*PI*Sigma*Sigma,0.5f))


#define PCSSMaxKernelSize 12
#define PCSSMinKernelSize 3


in vec2 FragCoord;


out vec3 Output1;

uniform sampler2D GBuffer[GBufferTextNums-2];
uniform isampler2D GBufferNormalMap;
uniform usampler2D GBufferMatID;
uniform sampler2D GBufferDepthMap;
uniform sampler2DArray SMTex_Sampler[MaxCascadessNums];
uniform sampler2DArray SSMTex_Sampler[MaxCascadessNums];

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
		mat4 LightMat[MaxShadowmapsNums];
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

layout (std140) uniform TSOS
{
		vec4 text_offsets[16];
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


float BilinearShadowTest(const vec2 ShadowCoord, const float Depth, const sampler2DArray ShadowMap, const int i, const float MaxRad, const ivec2 TextureSize)
{
	const vec2 samples=floor(float(TextureSize)*ShadowCoord);

	const vec4 samplesDepth=textureGather(ShadowMap,vec3(samples/vec2(TextureSize),i),0);
	
	vec4 SamplesShadowVals;

	if (Depth>samplesDepth.w)	
		SamplesShadowVals[0]=0.0f;
	else
		SamplesShadowVals[0]=1.0f;
	
	if (Depth>samplesDepth.z)	
		SamplesShadowVals[1]=0.0f;
	else
		SamplesShadowVals[1]=1.0f;

	if (Depth>samplesDepth.x)		
		SamplesShadowVals[2]=0.0f;
	else
		SamplesShadowVals[2]=1.0f;

	if (Depth>samplesDepth.y)	
		SamplesShadowVals[3]=0.0f;
	else
		SamplesShadowVals[3]=1.0f;

	const vec2 lerpVals=vec2(TextureSize)*ShadowCoord.xy-samples;

	return mix(mix(SamplesShadowVals[0],SamplesShadowVals[1],lerpVals.x),mix(SamplesShadowVals[2],SamplesShadowVals[3],lerpVals.x),lerpVals.y);
}


float SilhouetteShadowTest(const vec2 ShadowCoord, const float Depth, const sampler2DArray ShadowMap, const int i, const float MaxRad, const ivec2 TextureSize)
{
	const vec2 samples=floor(float(TextureSize)*ShadowCoord);

	const vec4 samplesDepth=textureGather(ShadowMap,vec3(samples/vec2(TextureSize),i),0);
	
	vec4 SamplesShadowVals;

	if (Depth>samplesDepth.w)	
		SamplesShadowVals[0]=0.0f;
	else
		SamplesShadowVals[0]=1.0f;
	
	if (Depth>samplesDepth.z)	
		SamplesShadowVals[1]=0.0f;
	else
		SamplesShadowVals[1]=1.0f;

	if (Depth>samplesDepth.x)		
		SamplesShadowVals[2]=0.0f;
	else
		SamplesShadowVals[2]=1.0f;

	if (Depth>samplesDepth.y)	
		SamplesShadowVals[3]=0.0f;
	else
		SamplesShadowVals[3]=1.0f;

	const vec2 lerpVals=vec2(TextureSize)*ShadowCoord.xy-samples;

	return mix(mix(SamplesShadowVals[0],SamplesShadowVals[1],lerpVals.x),mix(SamplesShadowVals[2],SamplesShadowVals[3],lerpVals.x),lerpVals.y);
}

float AvarageShadowCasterDepth(vec2 ShadowCoord, float Depth, sampler2DArray ShadowMap, int TextID, int R, ivec2 TextureSize)//reciever's coordinate on shadow map, reciever's depth, shadow map sampler, current layer of the shadow map array sampler, radiouse of the blocker search, size of the shadow map texture
{
	float RetValue=0.0f;
	float Count=0.0f;

	const int start=-R/2;;
	const int end=start+R;

	for (int IS=0;IS<4;IS++)//cause a descrete search
		for (int JS=0;JS<4;JS++)
		{
			for (int i=start+IS;i<end;i+=4)
				for (int j=start+JS;j<R;j+=4)
				{
					const vec4 SamplesDepth=textureGather(ShadowMap,vec3(ShadowCoord+vec2(i,j)*2.0f/vec2(TextureSize),TextID),0);
					for (int k=0;k<4;k++)
						if (SamplesDepth[k]<Depth)
						{
							RetValue+=SamplesDepth[k];
							Count++;
						}
					if (Count>16)//early bail
						return RetValue/Count;
				}
		}

	return (Count!=0.0f)?RetValue/Count:0.0f;
}

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233f))) * 43758.5453f);
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

	/****************************************************************************/
	/****************************************************************************/
	/************************discard the unwanted fragment***********************/
	/****************************************************************************/
	const vec3 TempL=CurrentLight.light_position-VSPos;
	const float L_Length=length(TempL);
	if (L_Length>CurrentLight.max_radius)
		discard;
	const vec3 L=TempL/L_Length;

	float Cut_Off_Dot;
	if (CurrentLight.cut_off_cos==-1.0)
		Cut_Off_Dot=-1;
	else
		Cut_Off_Dot=CurrentLight.cut_off_cos+dot(L,CurrentLight.direction);

	if (Cut_Off_Dot>CurrentLight.edge_cut_off_cos_delta)
		discard;
	/****************************************************************************/
	/************************discard the unwanted fragment***********************/
	/****************************************************************************/
	/****************************************************************************/
	
		

	/****************************************************************************/
	/****************************************************************************/
	/*********************************Shadow mapping*****************************/
	/****************************************************************************/

	float ShadowEffect = 1.0f;

	const bool SMAvailble=(CurrentLightIndex<MaxShadowmapsNums)&&(CurrentLight.nonesense1>16777215||CurrentLight.nonesense1<0);

	if (SMAvailble)
	{
		ShadowEffect=0.0f;
#ifdef PCF
		/////////////////////////////////////PCF//////////////////////////////
		const float ActualDepth=length(VSPos-CurrentLight.light_position);
		const mat4 ProjMat = LightMat[CurrentLightIndex];
		vec4 SMCoord = ProjMat * vec4(VSPos,1.0f);
		SMCoord/=SMCoord.w;
		SMCoord=(SMCoord+1.0f)/2.0f;
		ShadowEffec=1.0f;
		if (SMCoord.x<1.0f && SMCoord.y<1.0f && SMCoord.y>=0.0f && SMCoord.x>=0.0f && SMCoord.z<1.0f && SMCoord.z>=0.0f)
		{
			const ivec2 TextureSize=ivec2(Shadowmap_Res);
			vec2 tempSMcoords;
			ShadowEffect=BilinearShadowTest(SMCoord.xy,ActualDepth,SMTex_Sampler[0],CurrentLightIndex,CurrentLight.max_radius,TextureSize)*text_offsets[0].w;
			for (int j=0;j<8;j++)
			{
				tempSMcoords=SMCoord.xy+text_offsets[j].xy;
				if (!(tempSMcoords.x<0.0001||tempSMcoords.x>0.9999||tempSMcoords.y<0.0001||tempSMcoords.y>0.9999))
					ShadowEffect+=BilinearShadowTest(tempSMcoords,ActualDepth,SMTex_Sampler[0],CurrentLightIndex,CurrentLight.max_radius,TextureSize)*text_offsets[j].z;
				else
					ShadowEffect+=text_offsets[j].z;
			}
		}
		else
			discard;
#endif
		//////////////////////////////////PCSS////////////////////////////////
		const float ActualDepth=dot(VSPos-CurrentLight.light_position, CurrentLight.direction);//z in SM VS
		const mat4 ProjMat = LightMat[CurrentLightIndex];
		vec4 SMCoord = ProjMat * vec4(VSPos,1.0f);
		SMCoord/=SMCoord.w;
		SMCoord=(SMCoord+1.0f)/2.0f;
		




		if (SMCoord.x<1.0f && SMCoord.y<1.0f && SMCoord.y>=0.0f && SMCoord.x>=0.0f && SMCoord.z<1.0f && SMCoord.z>=0.0f)
		{
			ShadowEffect=1.0f;
			ivec2 TexelCoords=ivec2(floor(SMCoord.xy*float(Shadowmap_Res)));

			if (ActualDepth>texelFetch(SMTex_Sampler[0],ivec3(TexelCoords.xy,CurrentLightIndex),0).x)
			{
				vec2 TexelCenter=(vec2(TexelCoords)+0.5)/float(Shadowmap_Res);

				float slope=texelFetch(SSMTex_Sampler[0],ivec3(TexelCoords,CurrentLightIndex),0).x;
				float offset=texelFetch(SSMTex_Sampler[0],ivec3(TexelCoords,CurrentLightIndex+MaxShadowmapsNums),0).x;

				float CurrentState=SMCoord.x*slope+offset-SMCoord.y;

				ivec2 step;
				if (slope>1.0f||slope<-1.0f)
				{
					step=ivec2(1,0);
				}
				else
				{
					step=ivec2(0,1);
				}
				float NextState=(SMCoord.x+float(step.x))*slope+offset-SMCoord.y-float(step.y);
				if (NextState*CurrentState<0.0f)
					step=-step;


				if (ActualDepth>texelFetch(SMTex_Sampler[0],ivec3(TexelCoords.xy+step,CurrentLightIndex),0).x)
					ShadowEffect=0.0f;
				else
				{
					ShadowEffect=1.0f;
					Output1=vec3(1.0f,0.0f,0.0f);
					return;
				}
			}
		}
		else
			discard;


		/*if (SMCoord.x<1.0f && SMCoord.y<1.0f && SMCoord.y>=0.0f && SMCoord.x>=0.0f && SMCoord.z<1.0f && SMCoord.z>=0.0f)
		{
			
			const ivec2 TextureSize=ivec2(Shadowmap_Res);
			float AvrgDepth=AvarageShadowCasterDepth(SMCoord.xy,ActualDepth,SMTex_Sampler[0],CurrentLightIndex,PCSSMaxKernelSize,TextureSize);

			const float FilterSize=(ActualDepth-AvrgDepth)/(AvrgDepth)*TextureSize.x*2.0f/(abs(dot(L,CurrentLight.direction))*L_Length)*0.01f*100;

			const int PCSSSampleCount=min(int(FilterSize),PCSSMaxKernelSize-PCSSMinKernelSize)+PCSSMinKernelSize;

			const float StepSize=min(FilterSize,PCSSMaxKernelSize)/float(PCSSSampleCount);
			
			const float NewSampleCount=float(PCSSSampleCount*PCSSSampleCount);

			const int start=-PCSSSampleCount/2;
			const int end=start+PCSSSampleCount;

			for (int i=start;i<end;i++)
				for (int j=start;j<end;j++)
					ShadowEffect+=BilinearShadowTest(SMCoord.xy+vec2(i,j)*2.0f/float(Shadowmap_Res)*StepSize,ActualDepth,SMTex_Sampler[0],CurrentLightIndex,CurrentLight.max_radius,TextureSize)/NewSampleCount;
			

			if (ShadowEffect!=0.0f&&ShadowEffect!=1.0f)
			{
				const float a=rand(VSPos.xy),b=rand(VSPos.xz),c=rand(VSPos.yz);

				const float NormalRand=(abs(a+b+c)/6.0f+(int(VSPos.x*b+VSPos.y*c+VSPos.z*a)%2==1?0.5f:0.0f));

				ShadowEffect=clamp(ShadowEffect+(NormalRand-0.5f)*2.0f/NewSampleCount,0.0f,1.0f);
			}

			
		}
		else
			discard;*/
	}
	/****************************************************************************/
	/*********************************Shadow mapping*****************************/
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
	/*****************calculate the cut of and attinuation effect****************/
	/****************************************************************************/
	const float Cut_Off_Effect=min(1.0,(CurrentLight.edge_cut_off_cos_delta-Cut_Off_Dot)/CurrentLight.edge_cut_off_cos_delta);
	const float Attenuation=CurrentLight.Attenuation[0]+L_Length*(CurrentLight.Attenuation[1]+L_Length*CurrentLight.Attenuation[2]);
	/****************************************************************************/
	/*****************calculate the cut of and attinuation effect****************/
	/****************************************************************************/
	/****************************************************************************/


	const vec3 V=-normalize(VSPos);

	Output1=vec3(max(dot(L,normal),0.0));
	Output1+=vec3(SpecTerm(a,L,V,normal));

	Output1=max(Output1*CurrentLight.light_color*Texture.xyz*Cut_Off_Effect/Attenuation,vec3(0.0f))*ShadowEffect;
}