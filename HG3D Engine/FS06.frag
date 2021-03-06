//AA shader
#version 450

#define MaxLineLength 32

out vec3 Output1;

uniform sampler2D AliasedImage;
uniform sampler2D EdgeTexture;

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

ivec4 GPBD(sampler2D Edge, vec2 Coordinates, vec2 DX, vec2 DY)
{
    float bordera = texture(Edge,Coordinates-DX*0.25).x;
    float borderb = texture(Edge,Coordinates-DY*0.25).y;

    ivec4 reta = (bordera > 0.9f) ? ivec4(0,1,0,1) : 
                    ((bordera > 0.5f) ? ivec4(0,1,0,0) : 
                    ((bordera > 0.1f) ? ivec4(0,0,0,1) : ivec4(0)));
                    
    ivec4 retb = (borderb > 0.9f) ? ivec4(1,0,1,0) : 
                    ((borderb > 0.5f) ? ivec4(1,0,0,0) : 
                    ((borderb > 0.1f) ? ivec4(0,0,1,0) : ivec4(0)));
        
    return reta + retb;
}

ivec4 LUT[4];

ivec4 FCE_Setup_LUT(int Dir, vec2 Coordinates, vec2 DX, vec2 DY, sampler2D Texture)
{
	vec4 direction=ivec4(0);
	direction[Dir]=1;
	const vec2 EdgeMask=(direction.x+direction.z)!=0?vec2(0,1):vec2(1,0);//checked
	const vec2 SideMask=(direction.x+direction.z)!=0?vec2(1,0):vec2(0,1);//checked
	
	const vec2 Step=((direction.x+direction.z)!=0?DY:DX)*((direction.x+direction.y)!=0?1.0f:-1.0f);//checked
	const vec2 OrthoStep=(direction.x+direction.z)!=0?DX:DY;//checked
	
	vec2 CurrentPos=(direction.x+direction.y)!=0?Coordinates:(Coordinates+Step);//checked
	
	ivec4 ret=ivec4(0);//checked
	bool FSR=false;//checked
	bool SSR=false;//checked

	int StepCount=1;//checked

	vec2 Center,FS,SS;

	bool CenterFound=false;

	while(!FSR||!SSR)
	{
		Center=texture(Texture,CurrentPos).xy;//checked

		CenterFound=dot(Center*EdgeMask,vec2(1.0f))==1.0f;

		if (!FSR)//checked
		{
			if (CenterFound)//checked
				ret.y=StepCount;//checked
			FS=texture(Texture,CurrentPos-OrthoStep).xy;//checked
			if (dot(FS,EdgeMask)==1.0f)//checked
			{
				ret.x=StepCount;//checked
				FSR=true;//checked
			}
			else if (dot(FS*SideMask,vec2(1.0f))==0.0f&&!CenterFound)//checked
			{
				ret.x=StepCount+MaxLineLength;//checked
				FSR=true;//checked
			}
		}
		if (!SSR)//checked
		{
			if (CenterFound)//checked
				ret.z=StepCount;//checked
			SS=texture(Texture,CurrentPos+OrthoStep).xy;//checked
			if (dot(SS,EdgeMask)==1.0f)//checked
			{
				ret.w=StepCount;//checked
				SSR=true;//checked
			}
			else if (dot(Center*SideMask,vec2(1.0f))==0.0f&&!CenterFound)//checked
			{
				ret.w=StepCount+MaxLineLength;//checked
				SSR=true;//checked
			}
		}

		if (CenterFound)//checked
			return ret;//checked

		if (MaxLineLength==StepCount)//checked
			return ret;//checked
		StepCount++;//checked
		CurrentPos+=Step;//checked
	}
		
	return ret;
}

ivec3 FCE(ivec4 direction, ivec2 Sides)
{
	ivec3 ret;
	for (int i=0;i<4;i++)
		if (direction[i]==1)
		{
			ret=ivec3(0);
			if (Sides.x==1&&Sides.y==1)
			{
				ret.y=max(LUT[i].y,LUT[i].z);
				ret.x=LUT[i].x;
				ret.z=LUT[i].w;
			}
			else if (Sides.x==1)
			{
				ret.x=LUT[i].x;
				ret.y=LUT[i].y;
			}
			else if (Sides.y==1)
			{
				ret.y=LUT[i].z;
				ret.z=LUT[i].w;
			}
		}
	return ret;
}

void main()
{
	const vec2 DX=vec2(1.0f,0.0f)/CurrentCamera.veiwport_dimensions.z;
	const vec2 DY=vec2(0.0f,1.0f)/CurrentCamera.veiwport_dimensions.w;
	
	const vec2 FragCoord=gl_FragCoord.xy/CurrentCamera.veiwport_dimensions.zw;

	ivec4 CurrentPoint=GPBD(EdgeTexture,FragCoord,DX,DY);

	vec3 colors[5];
	colors[0]=texture(AliasedImage,FragCoord).xyz;
	Output1=colors[0];
	if (dot(CurrentPoint,CurrentPoint)==0)
		return;
	
	for (int i=0;i<4;i++)
		LUT[i]=FCE_Setup_LUT(i, FragCoord, DX, DY, EdgeTexture);

	colors[1]=texture(AliasedImage,FragCoord+DY).xyz;
	colors[2]=texture(AliasedImage,FragCoord+DX).xyz;
	colors[3]=texture(AliasedImage,FragCoord-DY).xyz;
	colors[4]=texture(AliasedImage,FragCoord-DX).xyz;

	int NumberOfEdges=CurrentPoint.x+CurrentPoint.y+CurrentPoint.z+CurrentPoint.w;

	if (NumberOfEdges==4)
	{
		Output1=(colors[0]*4.0f+colors[1]+colors[2]+colors[3]+colors[4])/8.0f;
		return;
	}
	else if (NumberOfEdges>=2)
	{
		if ((CurrentPoint.y+CurrentPoint.w)==2||(CurrentPoint.x+CurrentPoint.z)==2)
		{
			ivec3 End,Start;

			ivec4 EDirection, SDirection;
			ivec2 Sides;

			Sides=ivec2(1,1);

			if ((CurrentPoint.y+CurrentPoint.w)==2)
			{
				SDirection=ivec4(0,0,1,0);
				EDirection=ivec4(1,0,0,0);
			}
			else
			{
				SDirection=ivec4(0,0,0,1);
				EDirection=ivec4(0,1,0,0);
			}

			End=FCE(EDirection, Sides);
			Start=FCE(SDirection, Sides);
			if (End.y+Start.y==0)
				return;

			int EFC=0,//end first side condition
			SFC=0,//start first side condition
			ESC=0,//end second side condition
			SSC=0;//start second side condition

			if (End.z!=0&&End.z<=MaxLineLength)
			{
				ESC=1;
				if (End.y>0)
					EFC=1;
			}
			if (End.x!=0&&End.x<=MaxLineLength)
			{
				EFC=-1;
				if (End.y>0)
					ESC=-1;
			}
			if (End.x==0&&End.z==0&&End.y>0)
			{
				ESC=-1;
				EFC=1;
			}

			if (Start.z!=0&&Start.z<=MaxLineLength)
			{
				SSC=1;
				if (Start.y>0)
					SFC=1;
			}
			if (Start.x!=0&&Start.x<=MaxLineLength)
			{
				SFC=-1;
				if (Start.y>0)
					SSC=-1;
			}
			if (Start.x==0&&Start.z==0&&Start.y>0)
			{
				SSC=-1;
				SFC=1;
			}

			if (SSC==-1&&SFC==1&&ESC==-1&&EFC==1)
				return;

			vec2 Portions=vec2(0.0f);


			if (SSC==-1&&ESC!=0)
			{
				float HalfWay=float((ESC==1?End.z:End.y)+Start.y-1)/2.0f;
				float delta=HalfWay-float(Start.y);
				if (delta>-0.1f)
				{
					Portions.y+=0.25f*(delta+delta+1.0f)/HalfWay;
				}
				else if (delta>-0.8f)
				{
					Portions.y+=0.125f/HalfWay;
				}
			}
			if (ESC==-1&&SSC!=0)
			{
				float HalfWay=float((SSC==1?Start.z:Start.y)+End.y-1)/2.0f;
				float delta=HalfWay-float(End.y);
				if (delta>-0.1f)
				{
					Portions.y+=0.25f*(delta+delta+1.0f)/HalfWay;
				}
				else if (delta>-0.8f)
				{
					Portions.y+=0.125f/HalfWay;
				}
			}

			if (SFC==1&&EFC!=0)
			{
				float HalfWay=float((EFC==1?End.y:End.x)+Start.y-1)/2.0f;
				float delta=HalfWay-float(Start.y);
				if (delta>-0.1f)
				{
					Portions.x+=0.25f*(delta+delta+1.0f)/HalfWay;
				}
				else if (delta>-0.8f)
				{
					Portions.x+=0.125f/HalfWay;
				}
			}
			if (EFC==1&&SFC!=0)
			{
				float HalfWay=float((SFC==1?Start.y:Start.x)+End.y-1)/2.0f;
				float delta=HalfWay-float(End.y);
				if (delta>-0.1f)
				{
					Portions.x+=0.25f*(delta+delta+1.0f)/HalfWay;
				}
				else if (delta>-0.8f)
				{
					Portions.x+=0.125f/HalfWay;
				}
			}



			if (SSC==-1&&End.z>MaxLineLength)
			{
				float EndDist=End.z-MaxLineLength;
				float Whole=float(EndDist+Start.y-1);

				Portions.y+=0.25f*(EndDist+EndDist-1.0f)/Whole;
			}
			if (ESC==-1&&Start.z>MaxLineLength)
			{
				float EndDist=Start.z-MaxLineLength;
				float Whole=float(EndDist+End.y-1);

				Portions.y+=0.25f*(EndDist+EndDist-1.0f)/Whole;
			}

						
						
			if (SFC==1&&End.x>MaxLineLength)
			{
				float EndDist=End.x-MaxLineLength;
				float Whole=float(EndDist+Start.y-1);

				Portions.x+=0.25f*(EndDist+EndDist-1.0f)/Whole;
			}
			if (EFC==1&&Start.x>MaxLineLength)
			{
				float EndDist=Start.x-MaxLineLength;
				float Whole=float(EndDist+End.y-1);

				Portions.x+=0.25f*(EndDist+EndDist-1.0f)/Whole;
			}

			if (dot(CurrentPoint,ivec4(1,0,1,0))==2)
				Output1=colors[0]*(1.0f-Portions.x-Portions.y)+colors[3]*Portions.x+colors[1]*Portions.y;
			else
				Output1=colors[0]*(1.0f-Portions.x-Portions.y)+colors[4]*Portions.x+colors[2]*Portions.y;
			
			
			return;
		}
		else  if (NumberOfEdges==2)
		{
			ivec3 FEnd,SEnd;

			ivec4 FDirection, SDirection;
			ivec2 FSides;
			ivec2 SSides;

			float SPortion=0,FPortion=0;

			if (CurrentPoint.x==1)
			{
				FDirection=ivec4(0,0,1,0);
				SSides=ivec2(0,1);
			}
			else
			{
				FDirection=ivec4(1,0,0,0);
				SSides=ivec2(1,0);
			}
			if (CurrentPoint.y==1)
			{
				SDirection=ivec4(0,0,0,1);
				FSides=ivec2(0,1);
			}
			else
			{
				SDirection=ivec4(0,1,0,0);
				FSides=ivec2(1,0);
			}
			FEnd=FCE(FDirection, FSides);
			SEnd=FCE(SDirection, SSides);

			float FSDist=max(FEnd.x,max(FEnd.y,FEnd.z));
			float SSDist=max(SEnd.x,max(SEnd.y,SEnd.z));

			bool FL=FSDist>MaxLineLength;
			bool SL=SSDist>MaxLineLength;

			if (FL)
				FSDist-=MaxLineLength;

			if (SL)
				SSDist-=MaxLineLength;

			if (FSDist==1.0f&&SSDist>0.0f)
			{
				if (SL)
				{
					SPortion!=0.5f-0.25f/(SSDist);
				}
				else
				{
					float HalfWay=float(SSDist)/2.0f;
					
					if (SSDist==1)
					{
						SPortion=0.125;
					}
					else
					{
						SPortion=0.5f-0.25f/(HalfWay);
					}
				}
			}

			if (SSDist==1.0f&&FSDist>0.0f)
			{
				if (SL)
				{
					FPortion!=0.5f-0.25f/(FSDist);
				}
				else
				{
					float HalfWay=float(FSDist)/2.0f;
					
					if (FSDist==1)
					{
						FPortion=0.125;
					}
					else
					{
						FPortion=0.5f-0.25f/(HalfWay);
					}
				}
			}
			Output1=colors[0]*(1.0f-FPortion-SPortion);
			if (CurrentPoint.x==1)
				Output1+=colors[1]*SPortion;
			if (CurrentPoint.z==1)
				Output1+=colors[3]*SPortion;

			if (CurrentPoint.y==1)
				Output1+=colors[2]*FPortion;
			if (CurrentPoint.w==1)
				Output1+=colors[4]*FPortion;
		}
	}
	else if (NumberOfEdges==1)
	{
		ivec3 End,Start;
		ivec4 EDirection, SDirection;
		ivec2 Sides;

		float Portion=0.0f;

		if (CurrentPoint.x==1)
		{
			SDirection=ivec4(0,0,0,1);
			EDirection=ivec4(0,1,0,0);
			Sides=ivec2(0,1);
		}
		else if (CurrentPoint.y==1)
		{
			SDirection=ivec4(0,0,1,0);
			EDirection=ivec4(1,0,0,0);
			Sides=ivec2(0,1);
		}
		else if (CurrentPoint.z==1)
		{
			SDirection=ivec4(0,0,0,1);
			EDirection=ivec4(0,1,0,0);
			Sides=ivec2(1,0);
		}
		else
		{
			SDirection=ivec4(0,0,1,0);
			EDirection=ivec4(1,0,0,0);
			Sides=ivec2(1,0);
		}

		End=FCE(EDirection, Sides);
		Start=FCE(SDirection, Sides);

		float EDist=max(End.x,max(End.y,End.z));
		float SDist=max(Start.x,max(Start.y,Start.z));

		bool EL=EDist>MaxLineLength;
		bool SL=SDist>MaxLineLength;

		if (EL)
			EDist-=MaxLineLength;

		if (SL)
			SDist-=MaxLineLength;

		if (End.y>0)
		{
			float Whole=float(EDist+SDist-1);
			if (SL)
				Portion=0.25f*float(SDist+SDist-1.0f)/Whole;
			else if (SDist>0.0f)
			{
				float HalfWay=Whole/2.0f;
				float delta=HalfWay-EDist;
				if (delta>-0.1f)
				{
					Portion+=0.25f*(delta+delta+1.0f)/HalfWay;
				}
				else if (delta>-0.8f)
				{
					Portion+=0.125f/HalfWay;
				}
			}
		}

		if (Start.y>0)
		{
			float Whole=float(EDist+SDist-1);
			if (EL)
				Portion=0.25f*float(EDist+EDist-1.0f)/Whole;
			else if (EDist>0.0f)
			{
				float HalfWay=Whole/2.0f;
				float delta=HalfWay-SDist;
				if (delta>-0.1f)
				{
					Portion+=0.25f*(delta+delta+1.0f)/HalfWay;
				}
				else if (delta>-0.8f)
				{
					Portion+=0.125f/HalfWay;
				}
			}
		}

		Output1=colors[0]*(1.0f-Portion);
		if (CurrentPoint.x==1)
			Output1+=colors[1]*Portion;
		if (CurrentPoint.z==1)
			Output1+=colors[3]*Portion;

		if (CurrentPoint.y==1)
			Output1+=colors[2]*Portion;
		if (CurrentPoint.w==1)
			Output1+=colors[4]*Portion;

		return;
	}

}