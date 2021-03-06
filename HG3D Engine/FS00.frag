#version 450

#define PCF
#define MaxShadowmapsNums  8
#define MaxCascadessNums   1

#define Shadowmap_Res			   4096

#define VL1

#define PI                  3.14159f			//pi
#define log2				0.3010299956639811f	//log2
#define sample_count		64					//number of samples

#define TAU					0.0001f
#define PHI					10000000.0f
#define PI_RCP				0.31830988618379067153776752674503f
#define Fog_Const			TAU * (PHI * 0.25f * PI_RCP)

#define MaxCascadessNums   1

in vec3 normal;
in vec3 position;
in vec3 position_LV;
in vec2 coord;
in float cascade;

out vec4 Output1;

layout(early_fragment_tests) in; //will try this later

uniform int Lights_Nums;
uniform sampler2DArray SMTex_Sampler[MaxCascadessNums];
uniform mat4 light_proj_view_matrix[MaxCascadessNums*MaxShadowmapsNums];
uniform mat4 inv_light_proj_view_matrix[MaxCascadessNums*MaxShadowmapsNums];
uniform vec3 Camera_Position;
uniform mat4 ViewMatrix;


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

layout (std140) uniform lights
{
		light Light[32];
};

layout (std140) uniform TSOS
{
		vec4 text_offsets[16];
};


bool _2_ranges_intersect(double a1, double a2, double b1, double b2)
{
	return !(b1 > a2 || a1 > b2);
}



float BilinearShadowTest(const vec2 ShadowCoord, const float Depth, const sampler2DArray ShadowMap, const int i, const float MaxRad, const ivec2 TextureSize)
{
	const vec2 samples=floor(vec2(TextureSize)*ShadowCoord);

	const vec4 samplesDepth=textureGather(ShadowMap,vec3(samples/vec2(TextureSize),i),0);
	
	vec4 SamplesShadowVals;

	float Offseted_Depth=Depth-0.001f/(MaxRad-1.0f);

	if (Offseted_Depth>samplesDepth.w)	
		SamplesShadowVals[0]=0.0f;
	else
		SamplesShadowVals[0]=1.0f;
	
	if (Offseted_Depth>samplesDepth.z)	
		SamplesShadowVals[1]=0.0f;
	else
		SamplesShadowVals[1]=1.0f;

	if (Offseted_Depth>samplesDepth.x)		
		SamplesShadowVals[2]=0.0f;
	else
		SamplesShadowVals[2]=1.0f;

	if (Offseted_Depth>samplesDepth.y)	
		SamplesShadowVals[3]=0.0f;
	else
		SamplesShadowVals[3]=1.0f;
	

	if (samples.x+1.0f>TextureSize.x)
	{
		SamplesShadowVals[1]=1.0f;
		SamplesShadowVals[3]=1.0f;
	}
	if (samples.y+1.0f>TextureSize.y)
	{
		SamplesShadowVals[2]=1.0f;
		SamplesShadowVals[3]=1.0f;
	}

	const vec2 lerpVals=vec2(TextureSize)*ShadowCoord.xy-samples;

	return mix(mix(SamplesShadowVals[0],SamplesShadowVals[1],lerpVals.x),mix(SamplesShadowVals[2],SamplesShadowVals[3],lerpVals.x),lerpVals.y);
}

float ShadowTest(vec2 ShadowCoord, float Depth, sampler2DArray ShadowMap, int i, float MaxRad, ivec2 TextureSize)
{
	const float Offseted_Depth=Depth-0.001f/(MaxRad-1.0f);

	const vec4 samplesDepth=textureGather(ShadowMap,vec3(ShadowCoord,i),0);
	
	vec4 SamplesShadowVals;

	if (Offseted_Depth>samplesDepth.w)	
		SamplesShadowVals[0]=0.0f;
	else
		SamplesShadowVals[0]=1.0f;
	
	if (Offseted_Depth>samplesDepth.z)	
		SamplesShadowVals[1]=0.0f;
	else
		SamplesShadowVals[1]=1.0f;

	if (Offseted_Depth>samplesDepth.x)		
		SamplesShadowVals[2]=0.0f;
	else
		SamplesShadowVals[2]=1.0f;

	if (Offseted_Depth>samplesDepth.y)	
		SamplesShadowVals[3]=0.0f;
	else
		SamplesShadowVals[3]=1.0f;
	vec2 SMResRCP=ShadowCoord+vec2(1.0f)/vec2(TextureSize);
	if (SMResRCP.x>0.999f)
	{
		SamplesShadowVals[1]=1.0f;
		SamplesShadowVals[3]=1.0f;
	}
	if (SMResRCP.y>0.999f)
	{
		SamplesShadowVals[2]=1.0f;
		SamplesShadowVals[3]=1.0f;
	}
	return (SamplesShadowVals[0]+SamplesShadowVals[1]+SamplesShadowVals[2]+SamplesShadowVals[3])/4.0f;
}

vec3 refract (vec3 I,vec3 N,float eta)
{
	/*const vec3 biN = (I - dot(I,N)*N)/n;
	cos
	return I;*/
	float k = 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I));
	if (k < 0.0f)
		return vec3(0.0f); 
	return (eta * I - (eta * dot(N, I) + sqrt(k)) * N);
}

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

	if (denom==0.0f)
		return 0;
    float val = D * G / denom;
    return val;
}

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233f))) * 43758.5453f);
}

void main()
{
	const float roughness=0.6f;
	const float a=roughness*roughness;

	Output1=vec4(0.0f);

	vec3 N=normalize(normal);

	vec3 Fog;

	for (int i=0;i<Lights_Nums;i++)
	{

		const vec3 LightColor=Light[i].light_color;

		Fog=vec3(0.0f);

		const bool SMAvailble=(i<MaxShadowmapsNums)&&(Light[i].nonesense1>16777215||Light[i].nonesense1<0);

		const int Cascade=int(cascade);
		vec4 Actual_Pos;

		if (SMAvailble)
		{

			Actual_Pos=light_proj_view_matrix[i*MaxCascadessNums+int(cascade)]*vec4(position,1.0f);
			float FogIntensity=0.0f;



			
			


#ifdef VL1
			const int CascadeIndex=MaxCascadessNums==1?0:int(abs(-(ViewMatrix*vec4(Camera_Position,1)).z-1.0)/999.0*float(MaxCascadessNums*MaxCascadessNums));
			vec4 temp_vec4s=light_proj_view_matrix[i*MaxCascadessNums+CascadeIndex]*vec4(Camera_Position,1);
			
			vec4 temp_vec4e=Actual_Pos;

			const vec4 RayInNDC=temp_vec4e-temp_vec4s;

			if (temp_vec4s.w>1.0f||temp_vec4e.w>1.0f)
			{
				if (temp_vec4s.w>1.0f&&temp_vec4e.w<1.0f)
					temp_vec4e=temp_vec4s+RayInNDC*((1.0-temp_vec4s.w)/(RayInNDC.w));
				else if (temp_vec4e.w>1.0f&&temp_vec4s.w<1.0f)
					temp_vec4s=temp_vec4s+RayInNDC*((1.0-temp_vec4s.w)/(RayInNDC.w));

				vec3 start=temp_vec4s.xyz/(temp_vec4s.w*2.0f)+0.5f;
				vec3 end=temp_vec4e.xyz/(temp_vec4e.w*2.0f)+0.5f;


				if (_2_ranges_intersect(min(start.x,end.x),max(start.x,end.x),0.0f,1.0f)&&_2_ranges_intersect(min(start.y,end.y),max(start.y,end.y),0.0f,1.0f))
				{



					vec3 ray=end-start;

					int intersection_count=0;

					float interections[4];
					interections[intersection_count]=-start.x/ray.x;

					if (interections[intersection_count]>0.0f&&interections[intersection_count]<1.0f)
						intersection_count++;

					interections[intersection_count]=(1.0f-start.x)/ray.x;

					if (interections[intersection_count]>0.0f&&interections[intersection_count]<1.0f)
						intersection_count++;

					interections[intersection_count]=-start.y/ray.y;

					if (interections[intersection_count]>0.0f&&interections[intersection_count]<1.0f)
						intersection_count++;

					interections[intersection_count]=(1.0f-start.y)/ray.y;

					if (interections[intersection_count]>0.0f&&interections[intersection_count]<1.0f)
						intersection_count++;

					for (int i=intersection_count-1;i>-1;i--)
						for (int j=0;j<i;j++)
							if (interections[j]>interections[j+1])
							{
								float temp=interections[j];
								interections[j]=interections[j+1];
								interections[j+1]=temp;
							}

					const bool start_in=start.x<1.0f&&start.x>0.0f&&start.y>0.0f&&start.y<1.0f;
					const bool end_in=end.x<1.0f&&end.x>0.0f&&end.y>0.0f&&end.y<1.0f;


					if (!(start_in&&end_in))
					{
						if (start_in)
							end=start+interections[0]*ray;
						else if (end_in)
							start=start+interections[intersection_count-1]*ray;
						else
						{
							vec3 center;
							for (int i=0;i<intersection_count-1;i++)
							{
								center=start+(interections[i]+interections[i+1])*0.5*ray;
								if (center.x>0.0f&&center.x<1.0f&&center.y>0.0f&&center.y<1.0f)
								{
									end=start+interections[i+1]*ray;
									start=start+interections[i]*ray;
									break;
								}
							}
						}
					}

					const vec3 Start2End=(end-start);
					const int sample_NO=int(min(length(Start2End.xy*Shadowmap_Res),sample_count-1));
					const vec3 step=Start2End/float(sample_NO);

					const float a=rand(position.xy),b=rand(position.xz),c=rand(position.yz);

					vec3 current_pos=start+step*(abs(a+b+c)/6.0f+(int(position.x*b+position.y*c+position.z*a)%2==1?0.5f:0.0f));

					vec4 TempActualPos=inv_light_proj_view_matrix[i*MaxCascadessNums+Cascade]*vec4(end*2.0f-1.0f,1.0f);
					const vec3 EndActualPos=TempActualPos.xyz/TempActualPos.w;

					TempActualPos=inv_light_proj_view_matrix[i*MaxCascadessNums+Cascade]*vec4(start*2.0f-1.0f,1.0f);
					vec3 current_position=TempActualPos.xyz/TempActualPos.w;
					vec3 ActualRay=current_position-EndActualPos;

					vec3 last_sample;

					const float RayLength=length(ActualRay);

					const float DepthConstant=(Light[i].max_radius-1.0f)/Light[i].max_radius;

					const float EndZ=1.0f/(end.z*DepthConstant-1.0f);
					const float StartZ=1.0f/(start.z*DepthConstant-1.0f);

					ActualRay/=(EndZ-StartZ);

					float MarchedDistance=0.0f;

					for (int j=0;j<=sample_NO;j++)
					{
						last_sample=current_position.xyz;

						if (j!=sample_NO)
						{
							const float currentz=1.0f/(current_pos.z*DepthConstant-1.0f);
							current_position=EndActualPos+ActualRay*(EndZ-currentz);
						}
						else
						{
							current_position.xyz=EndActualPos;
							current_pos=end-step*0.2f;
						}
						const vec3 middle=(current_position.xyz+last_sample)*0.5f;

						const float step_length=length(current_position.xyz-last_sample);


						vec3 LTS=middle-Light[i].light_position;
						float LTS_Length=length(LTS);

						if (dot(LTS,Light[i].direction)/LTS_Length>Light[i].cut_off_cos-Light[i].edge_cut_off_cos_delta)
						{
							const int Cascade=MaxCascadessNums==1?0:int(abs(-(ViewMatrix*vec4(current_pos,1)).z-1.0f)/999.0f*float(MaxCascadessNums*MaxCascadessNums));

							const float texture_data=texture(SMTex_Sampler[Cascade],vec3(current_pos.xy, i) , 0).x;


							if (current_pos.z<=texture_data)
							{
								const float AttenuationEffect=(Light[i].Attenuation[0]+LTS_Length*(Light[i].Attenuation[1]+LTS_Length*Light[i].Attenuation[2]))*LTS_Length*LTS_Length;//attenuation with double length devission
								FogIntensity+= Fog_Const * exp( (LTS_Length+RayLength-MarchedDistance) * -TAU )*step_length/AttenuationEffect;
							}
						}
						
						MarchedDistance+=step_length;

						current_pos+=step;
					}

				}
			
			}
			
#endif			
			



			
			




#ifdef VL0
			const vec3 step=(-position_LV)/float(sample_count);
			const float step_length=length(step);

			vec3 current_pos=Camera_Position+step*(float(int(gl_FragCoord.x)%8+(int(gl_FragCoord.y)%8)*8)/64.0);

			vec4 ShadowMapNDC;
			vec3 ShadowMapCoord;


			for (int j=0;j<sample_count;j++)
			{
				const vec3 LTS=current_pos-Light[i].light_position;
				const float LTS_Length=length(LTS);

				if (dot(LTS,Light[i].direction)/LTS_Length>Light[i].cut_off_cos-Light[i].edge_cut_off_cos_delta)
				{
					const int Cascade=MaxCascadessNums==1?0:int(abs(-(ViewMatrix*vec4(current_pos,1)).z-1.0)/999.0*float(MaxCascadessNums*MaxCascadessNums));

					bool shadowed=true;

					ShadowMapNDC=light_proj_view_matrix[i*MaxCascadessNums+Cascade]*vec4(current_pos,1);
					ShadowMapCoord=(ShadowMapNDC.xyz/ShadowMapNDC.w+1.0)*0.5;

					if (!(ShadowMapCoord.x<0.001||ShadowMapCoord.x>0.999||ShadowMapCoord.y<0.001||ShadowMapCoord.y>0.999||ShadowMapCoord.z<0.001||ShadowMapCoord.z>1.0))
					{
						float texture_data=texture(SMTex_Sampler[Cascade],vec3(ShadowMapCoord.xy, i) , 0).x;
						if (ShadowMapCoord.z<=texture_data)
							shadowed=false;
					}
					else
						shadowed=false;

					if (!shadowed)
					{
								const float AttenuationEffect=(Light[i].Attenuation[0]+LTS_Length*(Light[i].Attenuation[1]+LTS_Length*Light[i].Attenuation[2]))*LTS_Length*LTS_Length;//attenuation with double length devission
								FogIntensity += Fog_Const * exp( (LTS_Length+(sample_count-j)*step_length) * -TAU )*step_length/AttenuationEffect;
					}
				}
				current_pos+=step;
			}
#endif			



			












			Fog=FogIntensity * LightColor;
			Output1+=vec4(Fog,0.0);
			













		}


		
		vec3 L=Light[i].light_position-position;
		const float L_Length=length(L);

		if (Light[i].max_radius<L_Length)
			continue;

		L=L/L_Length;
		const float NoL=dot(L,N);

		if (NoL<0.0)
			continue;
		
		float Cut_Off_Dot;
		if (Light[i].cut_off_cos==-1.0)
			Cut_Off_Dot=-1;
		else
			Cut_Off_Dot=Light[i].cut_off_cos+dot(L,Light[i].direction);

		if (Cut_Off_Dot>Light[i].edge_cut_off_cos_delta)
			continue;

		const vec3 V=normalize(position_LV);

		const float Cut_Off_Effect=min(1.0,(Light[i].edge_cut_off_cos_delta-Cut_Off_Dot)/Light[i].edge_cut_off_cos_delta);

		float ShadowValue=1.0, BRDF=1.0, Alpha=1.0;

		const float Attenuation=Light[i].Attenuation[0]+L_Length*(Light[i].Attenuation[1]+L_Length*Light[i].Attenuation[2]);

		BRDF=NoL+SpecTerm(a,L, V, N);



		vec3 Albedo=vec3(1.0);
		
		if (SMAvailble)
		{
			const vec3 SMCoords=(Actual_Pos.xyz/Actual_Pos.w+1)/2;
			if (!(SMCoords.x<0.001||SMCoords.x>0.999||SMCoords.y<0.001||SMCoords.y>0.999||SMCoords.z<0.001||SMCoords.z>1.0))
			{

#ifdef VSM
				////////////////////////////VSM///////////////////////////////////////
				const float off_set=pow(10.0,floor(log(SMCoords.z)-4.0));
				const float filter_offset=off_set*20;
				const float depth=SMCoords.z-off_set;
				vec2 texture_data=texture(SMTex_Sampler[Cascade],vec3(SMCoords.xy, i) , 0).xy;
				if (depth<=texture_data.x)
				{
					ShadowValue=1.0;
					vec2 tempSMcoords;
					for (int j=0;j<8;j++)
					{
						tempSMcoords=SMCoords.xy+text_offsets[j].xy;
						if (!(tempSMcoords.x<0.001||tempSMcoords.x>0.999||tempSMcoords.y<0.001||tempSMcoords.y>0.999))
						{
							texture_data=texture(SMTex_Sampler[Cascade],vec3(tempSMcoords,i),0).xy;
							if (SMCoords.z>texture_data.x+filter_offset)	
							{				
								const float variance=texture_data.y-texture_data.x*texture_data.x;
								const float mD = texture_data.x - SMCoords.z + filter_offset; 
								const float mD_2 = mD * mD;
								ShadowValue*=min(1,max(0,variance/(variance+mD_2)));
							}
						}						
					}
				}
				else
					ShadowValue=0.0;
#endif
#ifdef PCF
					/////////////////////////////////////PCF//////////////////////////////
				const ivec2 TextureSize=ivec2(Shadowmap_Res);
				vec2 tempSMcoords;
				ShadowValue=BilinearShadowTest(SMCoords.xy,SMCoords.z,SMTex_Sampler[Cascade],i,Light[i].max_radius,TextureSize)*text_offsets[0].w;
				for (int j=0;j<8;j++)
				{
					tempSMcoords=SMCoords.xy+text_offsets[j].xy/**(length(position_LV)/100.0f+1.0)*/;
					if (!(tempSMcoords.x<0.0001||tempSMcoords.x>0.9999||tempSMcoords.y<0.0001||tempSMcoords.y>0.9999))
						ShadowValue+=BilinearShadowTest(tempSMcoords,SMCoords.z,SMTex_Sampler[Cascade],i,Light[i].max_radius,TextureSize)*text_offsets[j].z;
					else
						ShadowValue+=text_offsets[j].z;
				}
#endif
#ifdef PCSS
				const ivec2 TextureSize=ivec2(Shadowmap_Res);
				const vec2 LightSize=vec2(20,20);

				const vec2 TextureSizeRCP=vec2(1.0f)/vec2(TextureSize);

				float TempBlockerDepth;

				float blocker_depth=0.0;
				{
					const vec2 EPS=vec2(4,4)/vec2(TextureSize);
					int SampleCount=0,SecondaryCounter=0;
					for (float j=-EPS.x;j<EPS.x;j+=EPS.x*0.25)
					{
						for (float k=-EPS.y;k<EPS.y;k+=EPS.y*0.25)
						{
							TempBlockerDepth=texture(SMTex_Sampler[Cascade],vec3(SMCoords.xy+vec2(j,k), i) , 0).x;
							if (TempBlockerDepth>(SMCoords.z+0.00001))
							{
								blocker_depth+=TempBlockerDepth;
								SampleCount++;
							}
							SecondaryCounter++;
							if (SecondaryCounter>63)
								break;
						}
						if (SecondaryCounter>63)
							break;
					}
					blocker_depth/=float(SampleCount);
				}	


				//if ((blocker_depth-SMCoords.z)>0.000001)
				{
					const vec2 EPS=abs(LightSize/vec2(TextureSize)*(blocker_depth-SMCoords.z)/(1-blocker_depth));
					int SampleCount=0;
					vec2 tempSMcoords;
					ShadowValue=0.0;
					for (float j=-EPS.x;j<EPS.x;j+=EPS.x*0.25)
					{
						for (float k=-EPS.y;k<EPS.y;k+=EPS.y*0.25)
						{
							SampleCount++;
							tempSMcoords=SMCoords.xy+vec2(j,k);
							if (!(tempSMcoords.x<0.0001||tempSMcoords.x>0.9999||tempSMcoords.y<0.0001||tempSMcoords.y>0.9999))
								ShadowValue+=BilinearShadowTest(tempSMcoords,SMCoords.z,SMTex_Sampler[Cascade],i,Light[i].max_radius,TextureSize);
							else
								ShadowValue+=1.0;
							if (SampleCount>63)
								break;
						}
					if (SampleCount>63)
							break;
					}
					if (SampleCount!=0)
					 ShadowValue=ShadowValue/float(SampleCount);
				 }
					

#endif
			}
			
		}

		Output1+=vec4((LightColor*Albedo)*BRDF/Attenuation*ShadowValue*Cut_Off_Effect,Alpha);

	}
}