#version 450

#define PCF

layout(early_fragment_tests) in; //later will be added if it was availble

#ifdef VSM
	out vec2 Output1;
#endif
#ifdef PCF
	out float Output1;
#endif

void main()
{
#ifdef VSM
	Output1=vec2(gl_FragCoord.z,gl_FragCoord.z*gl_FragCoord.z);
#endif
}