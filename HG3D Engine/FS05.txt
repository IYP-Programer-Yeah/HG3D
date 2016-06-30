//edge detection shader
#version 450

out vec3 Output1;

uniform sampler2D AliasedImage;

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
float luma(vec3 RGB)
{
	return 0.2126*RGB.x + 0.7152*RGB.y + 0.0722*RGB.z;
}

void main()
{
	const vec2 FragCoord=gl_FragCoord.xy/CurrentCamera.veiwport_dimensions.zw;

	const vec2 DiffX=vec2(1.0f,0.0f)/CurrentCamera.veiwport_dimensions.z;
	const vec2 DiffY=vec2(0.0f,1.0f)/CurrentCamera.veiwport_dimensions.w;

	vec3 colors[9];
	colors[0]=texture(AliasedImage,FragCoord).xyz;
	colors[1]=texture(AliasedImage,FragCoord+DiffX).xyz;
	colors[2]=texture(AliasedImage,FragCoord+DiffX*2.0f).xyz;
	colors[3]=texture(AliasedImage,FragCoord+DiffY).xyz;
	colors[4]=texture(AliasedImage,FragCoord+DiffY*2.0f).xyz;
	colors[5]=texture(AliasedImage,FragCoord-DiffX).xyz;
	colors[6]=texture(AliasedImage,FragCoord-DiffY).xyz;
	colors[7]=texture(AliasedImage,FragCoord-DiffX*2.0f).xyz;
	colors[8]=texture(AliasedImage,FragCoord-DiffY*2.0f).xyz;
	float c[8];
	c[0]=luma(abs(colors[0]-colors[1]));
	c[1]=luma(abs(colors[1]-colors[2]));
	c[2]=luma(abs(colors[0]-colors[3]));
	c[3]=luma(abs(colors[3]-colors[4]));
	c[4]=luma(abs(colors[0]-colors[5]));
	c[5]=luma(abs(colors[0]-colors[6]));
	c[6]=luma(abs(colors[7]-colors[5]));
	c[7]=luma(abs(colors[8]-colors[6]));
	const float MaxC=max(max(c[0],c[2]),max(c[4],c[5]));
	const float MaxCT=max(MaxC,max(c[3],c[7]));
	const float MaxCL=max(MaxC,max(c[1],c[6]));

	Output1=vec3(0.0,0.0,0.0);
	if (c[0]>0.05f&&c[0]>MaxCL*0.5f)
	{
		Output1+=vec3(1.0,0.0,0.0);
	}
	if (c[2]>0.05f&&c[2]>MaxCT*0.5f)
	{
		Output1+=vec3(0.0,1.0,0.0);
	}
}