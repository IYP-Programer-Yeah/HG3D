//blur shader
#version 450

#define BlurRadius	16

#define PI					3.14159f			//pi
#define Sigma				5.0f				//sigma
#define e					2.71828f			//e

#define GN(x)				(pow(e,-x*x/(2.0f*Sigma*Sigma))/pow(2.0f*PI*Sigma*Sigma,0.5f))

out vec3 Output1;

uniform sampler2D Texture;

#define Temp_Texture_W				384									//temp texture width
#define Temp_Texture_H				256									//temp texture height


uniform float Weight[BlurRadius]={	GN(0.0f),GN(1.0f),
									GN(2.0f),GN(3.0f),
									GN(4.0f),GN(5.0f),
									GN(6.0f),GN(7.0f),
									GN(8.0f),GN(9.0f),
									GN(10.0f),GN(11.0f),
									GN(12.0f),GN(13.0f),
									GN(14.0f),GN(15.0f)/*,
									GN(16.0f),GN(17.0f),
									GN(18.0f),GN(19.0f),
									GN(20.0f),GN(21.0f),
									GN(22.0f),GN(23.0f),
									GN(24.0f),GN(25.0f),
									GN(26.0f),GN(27.0f),
									GN(28.0f),GN(29.0f),
									GN(30.0f),GN(31.0f)*/
									};

uniform int BlurPass=0;

void main()
{
	ivec2 Diff=ivec2(0);
	Diff[BlurPass]=1;

	Output1=vec3(0.0f);
	ivec2 CurrentSample=ivec2(gl_FragCoord.xy);
	float Sum=0.0f;
	for (int i=0;i<BlurRadius;i++)
	{
		Output1+=texelFetch(Texture,CurrentSample,0).xyz*Weight[i];		
		CurrentSample+=Diff;

		Sum+=Weight[i];
	}
	CurrentSample=ivec2(gl_FragCoord.xy);
	for (int i=1;i<BlurRadius;i++)
	{
		CurrentSample-=Diff;
		Output1+=texelFetch(Texture,CurrentSample,0).xyz*Weight[i];		

		Sum+=Weight[i];
	}
	Output1/=Sum;	
}