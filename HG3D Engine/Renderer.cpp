#include "HG3D Engine.h"
#include "..\File_HG_FS\File_HG_FS.h"
#include "..\Shared Headers\hstring.h"
#include "GLew.h"
static unsigned long int renderer_class_nums = 0;
static unsigned long int shader_nums = 0;
/******************************************************************/
/**************************test console****************************/
/******************************************************************/
/******************************************************************/
string Final_str[2];
/******************************************************************/
/******************************************************************/
/**************************test console****************************/
/******************************************************************/
GLuint LoadShaders(const char *V_Shader_Path, const char *F_Shader_Path)
{
	GLint Result = GL_FALSE;
	GLuint ProgramID = glCreateProgram();//creat program
	File_HG_FS::File V_Shader, F_Shader;//shader's file
	GLuint VertexShaderID = 0, FragmentShaderID = 0;
	Final_str[0] = "vertex shader #";
	Final_str[0] = Final_str[0] + inttostring(shader_nums);
	Final_str[0] = Final_str[0] + " log: \n";
	if (V_Shader_Path != NULL) //check if the path exists
	{
		V_Shader.open(V_Shader_Path);
		VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShaderID, 1, &V_Shader.data, (int*)&V_Shader.size);
		glCompileShader(VertexShaderID);
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		if (Result == GL_FALSE)
		{
			GLint maxLength = 0;
			string Log;
			Log = "";
			glGetShaderInfoLog(VertexShaderID, 1000, &maxLength, Log.string1); //get the log
			Final_str[0] = Final_str[0] + Log; //add the log to the string
			Final_str[0] = Final_str[0] + "\n\n\n"; //add triple new lines
		}
		else
			Final_str[0] = Final_str[0] + "well compiled. \n";  //every thing went well
		glAttachShader(ProgramID, VertexShaderID);
	}
	else
		Final_str[0] = Final_str[0] + "not found. \n"; //no such path idiot
	Final_str[1] = "fragment shader #"; 
	Final_str[1] = Final_str[1] + inttostring(shader_nums);
	Final_str[1] = Final_str[1] + " log: \n";
	if (F_Shader_Path != NULL)
	{
		F_Shader.open(F_Shader_Path); //check if the path exists
		FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShaderID, 1, &F_Shader.data, (int*)&F_Shader.size);
		glCompileShader(FragmentShaderID);
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		if (Result == GL_FALSE)
		{
			GLsizei maxLength = 0;
			string Log;
			Log = "";
			glGetShaderInfoLog(FragmentShaderID, 1000, &maxLength, Log.string1); //get the log
			Final_str[1] = Final_str[1] + Log; //add the log to the string
			Final_str[1] = Final_str[1] + "\n\n\n"; //add triple new lines
		}
		else
			Final_str[1] = Final_str[1] + "well compiled. \n"; //every thing went well
		glAttachShader(ProgramID, FragmentShaderID);
	}
	else
		Final_str[1] = Final_str[1] + "not found. \n";//no such path idiot
	glBindAttribLocation(ProgramID, 0, "Vertex");
	glBindAttribLocation(ProgramID, 1, "Normal");
	glBindAttribLocation(ProgramID, 2, "Coord");
	glBindAttribLocation(ProgramID, 3, "bone_ID");
	glBindFragDataLocation(ProgramID, 0, "Output1");//bind the out put 
	glBindFragDataLocation(ProgramID, 1, "Output2");//bind the out put 
	glBindFragDataLocation(ProgramID, 2, "Output3");//bind the out put 
	glLinkProgram(ProgramID);//link shaders
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	if (V_Shader_Path != NULL)
		glDeleteShader(VertexShaderID);//delete if only one was created
	if (F_Shader_Path != NULL)
		glDeleteShader(FragmentShaderID);//delete if only one was created
	shader_nums++;
	return ProgramID;
}
//end of shader loader
void SetupPixelFormat_WND_DC(HDC hDC) //setup pixel format
{
	int nPixelFormat;
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),          //size of structure
		1,                                      //default version 
		PFD_DRAW_TO_WINDOW |                    //window drawing support
		PFD_SUPPORT_OPENGL |                    //opengl support
		PFD_DOUBLEBUFFER,                       //double buffering support
		PFD_TYPE_RGBA,                          //RGBA color mode
		32,                                     //32 bit color mode
		0, 0, 0, 0, 0, 0,                       //ignore color bits
		0,                                      //no alpha buffer
		1,                                      //ignore shift bit
		0,                                      //no accumulation buffer
		0, 0, 0, 0,                             //ignore accumulation bits
		32,                                     //32 bit z-buffer size
		0,                                      //no stencil buffer
		0,                                      //no aux buffer
		PFD_MAIN_PLANE,                         //main drawing plane
		0,                                      //reserved
		0, 0, 0 };                              //layer masks ignored
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);                            //layer masks ignored
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}


void CheckForGLErrors()
{
	GLuint something = glGetError();
	if (something == GL_INVALID_ENUM)
	{
		exit(7);
	}
	if (something == GL_INVALID_VALUE)
	{
		exit(8);
	}
	if (something == GL_INVALID_OPERATION)
	{
		exit(9);
	}
	if (something == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		exit(10);
	}
	if (something == GL_OUT_OF_MEMORY)
	{
		exit(11);
	}
	if (something == GL_INVALID_ENUM)
	{
		exit(12);
	}
	if (something == GL_STACK_UNDERFLOW)
	{
		exit(13);
	}
	if (something == GL_STACK_OVERFLOW)
	{
		exit(14);
	}
}
void CheckFBOStat()
{

	GLuint something = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (something == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
		exit(5);

	if (something == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
		exit(3);

	if (something == GL_FRAMEBUFFER_UNSUPPORTED)
		exit(4);
}

namespace HG3D_Engine
{
	template <class TypeA, class TypeB>  bool __declspec(dllexport) __fastcall Couple<TypeA,TypeB>::operator > (Couple<TypeA, TypeB> inp)
	{
		return B > inp.B;
	}
	template <class TypeA, class TypeB>  bool __declspec(dllexport) __fastcall Couple<TypeA, TypeB>::operator < (Couple<TypeA, TypeB> inp)
	{
		return B < inp.B;
	}
	template <class TypeA, class TypeB>  bool __declspec(dllexport) __fastcall Couple<TypeA, TypeB>::operator == (Couple<TypeA, TypeB> inp)
	{
		return (B == inp.B) && (A == inp.A);
	}
	template <class TypeA, class TypeB>  void __declspec(dllexport) __fastcall Couple<TypeA, TypeB>::operator = (Couple<TypeA, TypeB> inp)
	{
		A = inp.A;
		B = inp.B;
	}

	void camera::init()//update camera
	{
#ifdef DeferredSM
		for (int i = 0; i < MaxShadowmapsNums; i++)
			glGenTextures(1, &DeferredShadowText[i]);//generate the texture so you can free it
		glGenTextures(1, &DeferredFilteredShadowText);//generate the texture so you can free it
		/****************************************************************/
		/****************************************************************/
		/***********seting up the deferred shadow map FBO & RBO**********/
		/****************************************************************/
		glGenRenderbuffers(1, &DeferredShadow_RBO_ID);
		glBindRenderbuffer(GL_RENDERBUFFER, DeferredShadow_RBO_ID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Shadowmap_Res, Shadowmap_Res);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glGenFramebuffers(1, &DeferredShadow_FBO_ID);//set the fbo
		glBindFramebuffer(GL_FRAMEBUFFER, DeferredShadow_FBO_ID);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DeferredShadow_RBO_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		/****************************************************************/
		/***********seting up the deferred shadow map FBO & RBO**********/
		/****************************************************************/
		/****************************************************************/
#endif
#ifdef Deferred
		/****************************************************************/
		/****************************************************************/
		/*********************seting up the GBuffer FBO******************/
		/****************************************************************/
		glGenFramebuffers(1, &GBuffer_FBO_ID);//set the fbo
		/****************************************************************/
		/*********************seting up the GBuffer FBO******************/
		/****************************************************************/
		/****************************************************************/
		glGenTextures(1, &GBuffer_Depth);//generate the texture so you can free it
		for (unsigned int i = 0; i < GBufferTextNums; i++)
			glGenTextures(1, &GBufferID[i]);//generate the texture so you can free it

		CurrentTexWidth = -1;//set the current width and height of the GBuffer textures
		CurrentTexHeight = -1;

		needs_update = true;
#endif
	}
	void camera::update_camera()//update camera
	{
		ViewMatrix = LookAt(camera_position, forward, up);//update view matrix
		ProjectionMatrix = Projection(Left, Right, Buttom, Top, Near, Far);//update projection
#ifdef DeferredSM
		/****************************************************************/
		/****************************************************************/
		/************seting up the deferred shadow map textures**********/
		/****************************************************************/
		for (int i = 0; i < MaxShadowmapsNums; i++)
		{
			glBindTexture(GL_TEXTURE_3D, DeferredShadowText[i]);//bind the texture
			glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE8, camera_viewport[2], camera_viewport[3], MaxSoftShadowDepth, 0, GL_LUMINANCE, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//linear filter (we'll use it)
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //clamping dosnt really matter since we wont read out of the range
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

		glBindTexture(GL_TEXTURE_3D, DeferredFilteredShadowText);//bind the texture
		glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE8, camera_viewport[2], camera_viewport[3], MaxSoftShadowDepth, 0, GL_LUMINANCE, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//linear filter (we'll use it)
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //clamping dosnt really matter since we wont read out of the range
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		/****************************************************************/
		/************seting up the deferred shadow map textures**********/
		/****************************************************************/
		/****************************************************************/
#endif
#ifdef Deferred
		if (CurrentTexWidth != camera_viewport[2] || CurrentTexHeight != camera_viewport[3])
		{
			//depth GBuffer
			glBindTexture(GL_TEXTURE_2D, GBuffer_FBO_ID);//bind the texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, camera_viewport[2], camera_viewport[3], 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//linear filter (we'll use it)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //clamping dosnt really matter since we wont read out of the range
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//normal GBuffer
			glBindTexture(GL_TEXTURE_2D, GBufferID[0]);//bind the texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, camera_viewport[2], camera_viewport[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//linear filter (we'll use it)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //clamping dosnt really matter since we wont read out of the range
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//texture color GBuffer
			glBindTexture(GL_TEXTURE_2D, GBufferID[1]);//bind the texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, camera_viewport[2], camera_viewport[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//linear filter (we'll use it)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //clamping dosnt really matter since we wont read out of the range
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//Mat ID GBuffer
			glBindTexture(GL_TEXTURE_2D, GBufferID[2]);//bind the texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, camera_viewport[2], camera_viewport[3], 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//linear filter (we'll use it)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //clamping dosnt really matter since we wont read out of the range
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glBindFramebuffer(GL_FRAMEBUFFER, GBuffer_FBO_ID);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GBuffer_Depth, 0);//set the texture as depth buffer and start drawing
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GBufferID[0], 0);//set the texture as color attachment and start drawing
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GBufferID[1], 0);//set the texture as color attachment and start drawing
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GBufferID[2], 0);//set the texture as color attachment and start drawing
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			CurrentTexWidth = camera_viewport[2];//set the current width and height of the GBuffer textures
			CurrentTexHeight = camera_viewport[3];
		}
#endif
		needs_update = 0;
	}
	void camera::fps_camera(float pitch, float yaw, vector head_up)//update camera
	{
		_4x4matrix rotationP;
		_4x4matrix rotationY;
		point o;
		o.build(0.0f, 0.0f, 0.0f);
		rotationP.LoadRotation(head_up, o, pitch);//calculate rotaton matrix for pitch
		rotationY.LoadRotation(cross(head_up, forward), o, yaw);//calculate rotaton matrix for yaw
		forward = rotationY*(rotationP*forward);//multiply by mats pitch first
		up = rotationY*(rotationP*up);//multiply by mats pitch first
		needs_update = 1;
	}

	void texture::build(unsigned char *irgba, unsigned int w, unsigned int h, unsigned short int NOC, bool icompressed, unsigned long int icompression_method, unsigned long int idata_size)
	{
		width = w;
		height = h;//set the input
		number_of_components = NOC;
		if (!icompressed)
			total_size = width*height*number_of_components;//get the total memory size
		else
			total_size = idata_size;
		rgba = (unsigned char*)malloc(total_size);//allocate memory
		for (register unsigned long int i = 0; i < total_size; i++)//copy the data
			rgba[i] = irgba[i];
		needs_update = 1;//this will need updat after building
		generate_mipmaps = 1;//yes you probaly want them
		Repeat_X = 0;
		Repeat_Y = 0;
		glGenTextures(1, &texture_name);//generate the texture so you can free it
		compressed = icompressed;
		compression_method = icompression_method;
	}
	void texture::free_data()
	{
		free(rgba);//free allocation
		glDeleteTextures(1, &texture_name);//delete the texture
	}
	void texture::update()
	{
		glBindTexture(GL_TEXTURE_2D, texture_name);//bind the texture to update the data
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Repeat_X ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Repeat_Y ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (generate_mipmaps)//set min filter to use the mipmap if it was wanted
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (!compressed)//is it compressed data?
		{
			unsigned long int format = GL_RGBA;
			if (number_of_components == 3)
				format = GL_RGB;
			else if (number_of_components == 2)
				format = GL_RG;
			else if (number_of_components == 1)
				format = GL_R;
			if (generate_mipmaps)//creat the miomaps if wanted
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, rgba);//set the data
			/*if (generate_mipmaps)//creat the miomaps if wanted
				gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, rgba);*/
		}
		else
		{
			if (generate_mipmaps)//creat the miomaps if wanted
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, true);
			glCompressedTexImage2D(GL_TEXTURE_2D, 0, compression_method, width, height, 0, total_size, rgba);
		}
		glBindTexture(GL_TEXTURE_2D, 0);//unbind the texture 
		needs_update = 0;//just updated no need to redo it
	}
	void texture::clone_NMA(texture input)
	{
		texture_name = input.texture_name;//clone  every thing dont reallocate new memory
		Repeat_X = input.Repeat_X;
		needs_update = input.needs_update;
		generate_mipmaps = input.generate_mipmaps;
		number_of_components = input.number_of_components;
		width = input.width;
		height = input.height;
		total_size = input.total_size;
		rgba = input.rgba;//get the pointer the data is still usefull
	}

	void light::build()//initialize the way that the light be instantly usable
	{
		for (register int i = 0; i < 3; i++)
		{
			light_position[i] = 0.0f;
			light_color[i] = 1.0f;
			Attenuation[i] = 0.0f;
			direction[i] = 0.0f;
		}
		Attenuation[0] = 1.0f;
		max_radius = 10000.0f;
		cut_off_cos = -1.0f;
		edge_cut_off_cos_delta = 0.0f;
		light_enabled = 0;
		update_shadow_maps = 1;
		shadow_map = 1;
	}
	void light::calculate_max_radius()
	{
		if (Attenuation[2] == 0.0f && Attenuation[1] == 0.0f)
			max_radius = 10000.0f;//it will never not be contirbuting
		else
		{
			if (Attenuation[2] == 0.0f)
			{
				max_radius = (256.0f - Attenuation[0]) / Attenuation[1];
				return;
			}
			float min_value_rad = -1.0f * Attenuation[1] / Attenuation[2] / 2.0f;//the minimum value's occurrence radius 
			if (min_value_rad*(min_value_rad * Attenuation[2] + Attenuation[1]) + Attenuation[0] > 256.0f)
				max_radius = 0.0f;//it will always be more that 256
			else
			{
				float delta = Attenuation[1] * Attenuation[1] + 4.0f * (256.0f - Attenuation[0])*Attenuation[2];
				if (delta < 0.0f)
					max_radius = 10000.0f;//it will never not be contirbuting;
				else
					max_radius = min_value_rad + sqrt(delta) / (2.0f * Attenuation[2]);//what ever the color after this atinoution the light contribution is 0
			}
		}
	}
	void light::operator=(light input)//needed in add light to duplicate the light data
	{
		for (register int i = 0; i < 3; i++)
		{
			light_position[i] = input.light_position[i];
			light_color[i] = input.light_color[i];
			Attenuation[i] = input.Attenuation[i];
			direction[i] = input.direction[i];
		}
		max_radius = input.max_radius;
		cut_off_cos = input.cut_off_cos;
		edge_cut_off_cos_delta = input.edge_cut_off_cos_delta;
		light_enabled = input.light_enabled;
		shadow_map = input.shadow_map;
		update_shadow_maps = input.update_shadow_maps;
	}

#ifdef Deferred
	void Renderer::init()
	{
		mesh_nums = 0;
		vert_nums = 0;
		total_size = 0;            //initialize values
		cameras_nums = 0;
		current_camera_nums = 0;
		last_light_ID = 0;
		texture_nums = 0;
		light_data_changed = 0;
		meshes = (Mesh*)malloc(0);
		cameras = (camera*)malloc(0);//so they can be freed
		textures = (texture*)malloc(0);
		current_cameras = (unsigned long int*)malloc(0);
		mesh_draw_order = (Couple<unsigned long int, long double>*)malloc(0);
		for (register int i = 0; i < 100; i++)
			lights[i].build();
		renderer_ID = renderer_class_nums;//give it an ID
		renderer_class_nums++;//a renderer added
		SetupPixelFormat_WND_DC(hdc);//setup the pixel format
		hrc = wglCreateContext(hdc); //creat a render context
		wglMakeCurrent(hdc, hrc);//make the rc current
		Init_glew();//initialize glew functions
		glEnable(GL_DEPTH_TEST); //enable 
		glDepthFunc(GL_LEQUAL); //less or equal(closer) z are writen
		glEnable(GL_CULL_FACE); //cull the back face 
		glCullFace(GL_BACK);//cull the back face
		glClearDepth(1.0f);//clear the depth with 1



		/******************************************************************/
		/***************************Gen The UBO****************************/
		/******************************************************************/
		/******************************************************************/
		//generate the light UBO
		glGenBuffers(1, &light_data_UBO_ID);
		glBindBufferBase(GL_UNIFORM_BUFFER, lights_UBO_binding_point, light_data_UBO_ID);



		/******************************************************************/
		/***************************Gen The UBO****************************/
		/******************************************************************/
		/******************************************************************/

		/******************************************************************/
		/******************************************************************/
		/**********************load the screen quad************************/
		/******************************************************************/
		float ScreenQuad[] = { -1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f };
		glGenVertexArrays(1, &ScreenQuadVAO);//make a new VAO
		glBindVertexArray(ScreenQuadVAO);//bind vao
		glGenBuffers(1, &ScreenQuadVBO);//make a new buffer
		glBindBuffer(GL_ARRAY_BUFFER, ScreenQuadVBO);//binde vbo
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, ScreenQuad, GL_STATIC_DRAW);//send data to vbo
		glEnableVertexAttribArray(0);//set the index 0
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);//set position first pos: 0-Vert_Pos_Size
		glBindBuffer(GL_ARRAY_BUFFER, 0);//unbind vbo
		glBindVertexArray(0);//unbind vao
		/******************************************************************/
		/**********************load the screen quad************************/
		/******************************************************************/
		/******************************************************************/
		//load the GBuffer mapping shader
		Shaders[2] = LoadShaders("..\\HG3D Engine\\VS02.txt", "..\\HG3D Engine\\FS02.txt");//load shaders
		Model_Matrix_Location[2] = glGetUniformLocation(Shaders[2], "ModelMatrix");
		Normal_Matrix_Location[2] = glGetUniformLocation(Shaders[2], "NormalMatrix");
		Projection_Matrix_Location[2] = glGetUniformLocation(Shaders[2], "ProjectionMatrix");
		View_Matrix_Location[2] = glGetUniformLocation(Shaders[2], "ViewMatrix");
		Mat_ID_Location[2] = glGetUniformLocation(Shaders[2], "MatID");

		Light_Block_Index[2] = glGetUniformBlockIndex(Shaders[2], "lights");//get lights index
		glUniformBlockBinding(Shaders[2], Light_Block_Index[2], lights_UBO_binding_point);
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		/******************************************************************/
		AllocConsole();
		myConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		string tempstring;
		tempstring = "Renderer ID: ";
		tempstring = tempstring + inttostring(renderer_ID);
		tempstring = tempstring + '\n';
		WriteConsole(myConsoleHandle, tempstring.string1, (DWORD)strlen(tempstring.string1), &cCharsWritten, NULL);
		WriteConsole(myConsoleHandle, Final_str[0].string1, (DWORD)strlen(Final_str[0].string1), &cCharsWritten, NULL);
		WriteConsole(myConsoleHandle, Final_str[1].string1, (DWORD)strlen(Final_str[1].string1), &cCharsWritten, NULL);

		//load the gather shader
		Shaders[3] = LoadShaders("..\\HG3D Engine\\VS03.txt", "..\\HG3D Engine\\FS03.txt");//load shaders
		GBuffer_Sampler_loaction[3] = glGetUniformLocation(Shaders[3], "GBuffer");
		GBuffer_Mat_ID_Sampler_loaction[3] = glGetUniformLocation(Shaders[3], "GBufferMatID");
		GBuffer_Depth_Buffer_Sampler_loaction[3] = glGetUniformLocation(Shaders[3], "GBufferDepthMap");
		Camera_Position_Location[3] = glGetUniformLocation(Shaders[3], "CameraPos");
		Camera_Far_Location[3] = glGetUniformLocation(Shaders[3], "CameraFar");
		Camera_Near_Location[3] = glGetUniformLocation(Shaders[3], "CameraNear");

		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		/******************************************************************/
		WriteConsole(myConsoleHandle, Final_str[0].string1, (DWORD)strlen(Final_str[0].string1), &cCharsWritten, NULL);
		WriteConsole(myConsoleHandle, Final_str[1].string1, (DWORD)strlen(Final_str[1].string1), &cCharsWritten, NULL);


		/********************the FPS data, test console********************/
		COORD textcoord, pos;
		CHAR_INFO stringdata[20];
		_SMALL_RECT recta;

		Final_str[0] = "FPS: ";
		textcoord.X = (SHORT)strlen(Final_str[0].string1);
		textcoord.Y = 1;
		pos.X = 0;
		pos.Y = 0;
		for (SHORT charachter = 0; charachter < textcoord.X; charachter++)
		{
			stringdata[charachter].Attributes = 10;
			stringdata[charachter].Char.AsciiChar = Final_str[0].string1[charachter];
		}
		recta.Top = (SHORT)renderer_ID * 2;
		recta.Bottom = (SHORT)renderer_ID * 2 + 1;
		recta.Right = textcoord.X - 1 + 40;
		recta.Left = 0 + 40;
		WriteConsoleOutput(myConsoleHandle, stringdata, textcoord, pos, &recta);


		Final_str[0] = "Rendered Objects: ";
		textcoord.X = (SHORT)strlen(Final_str[0].string1);
		textcoord.Y = 1;
		pos.X = 0;
		pos.Y = 0;
		for (SHORT charachter = 0; charachter < textcoord.X; charachter++)
		{
			stringdata[charachter].Attributes = 11;
			stringdata[charachter].Char.AsciiChar = Final_str[0].string1[charachter];
		}
		recta.Top = (SHORT)renderer_ID * 2 + 1;
		recta.Bottom = (SHORT)renderer_ID * 2 + 2;
		recta.Right = textcoord.X - 1 + 40;
		recta.Left = 0 + 40;
		WriteConsoleOutput(myConsoleHandle, stringdata, textcoord, pos, &recta);
		/******************************************************************/
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/


	}
#endif
	unsigned long int Renderer::add_mesh(char* path)	//add a mesh 
	{
		mesh_nums++;//add number of meshes by 1
		Mesh *meshes_the_next = (Mesh *)malloc(sizeof(Mesh)*mesh_nums);//allocate new meshes' memory


		for (register unsigned long int i = 0; i < mesh_nums - 1; i++)
			meshes_the_next[i].clone_NMA(meshes[i]);//clone last data


		meshes_the_next[mesh_nums - 1].clear_last_buff = 0;//don't clear last buffer this is first run of this mesh
		meshes_the_next[mesh_nums - 1].load_mesh(path);//load mesh from path

		unsigned long int temp = mesh_nums - 1;
		meshes_the_next[mesh_nums - 1].MeshID[3] = unsigned char(temp % 256);//mesh id to be writen as 4 8 byte int
		temp = temp / 256;
		meshes_the_next[mesh_nums - 1].MeshID[2] = unsigned char(temp % 256);
		temp = temp / 256;
		meshes_the_next[mesh_nums - 1].MeshID[1] = unsigned char(temp % 256);
		temp = temp / 256;
		meshes_the_next[mesh_nums - 1].MeshID[0] = unsigned char(temp % 256);

		total_size += meshes_the_next[mesh_nums - 1].total_size;//update total size
		vert_nums += meshes_the_next[mesh_nums - 1].vert_nums;//update total vert nums
#ifdef Deferred
		free(mesh_draw_order);//it'll ve re allocated
		mesh_draw_order = (Couple<unsigned long int, long double>*)malloc(sizeof(Couple<unsigned long int, long double>)*mesh_nums);
#endif

		free(meshes);//free last data
		meshes = meshes_the_next;//replace the pointer to new mesh data

		return mesh_nums - 1;//return the added mesh's ID
	}
	unsigned long int Renderer::add_camera()//update camera 
	{
		cameras_nums++;//add number of textures by 1
		camera *cameras_the_next = (camera *)malloc(sizeof(camera)*cameras_nums);//allocate new meshes' memory


		for (register unsigned long int i = 0; i < cameras_nums - 1; i++)
			cameras_the_next[i] = cameras[i];//clone last data


		cameras_the_next[cameras_nums - 1].Near = 1.0f;//init to default
		cameras_the_next[cameras_nums - 1].Far = 1000.0f;//init to default
		cameras_the_next[cameras_nums - 1].Left = -1.0f;//init to default
		cameras_the_next[cameras_nums - 1].Right = 1.0f;//init to default
		cameras_the_next[cameras_nums - 1].Top = 1.0f;//init to default
		cameras_the_next[cameras_nums - 1].Buttom = -1.0f;//init to default
		cameras_the_next[cameras_nums - 1].forward.build(0.0f, 0.0f, 1.0f);//init to default
		cameras_the_next[cameras_nums - 1].up.build(0.0f, 1.0f, 0.0f);//init to default
		cameras_the_next[cameras_nums - 1].camera_position.build(0.0f, 0.0f, 0.0f);//init to default
		cameras_the_next[cameras_nums - 1].camera_viewport[0] = 0;//init to default
		cameras_the_next[cameras_nums - 1].camera_viewport[1] = 0;//init to default
		cameras_the_next[cameras_nums - 1].camera_viewport[2] = 600;//init to default
		cameras_the_next[cameras_nums - 1].camera_viewport[3] = 600;//init to default
		cameras_the_next[cameras_nums - 1].needs_update = true;

		free(cameras);//free last data
		cameras = cameras_the_next;//replace the pointer to new camera data

		wglMakeCurrent(hdc, hrc);//make the rc current
		cameras[cameras_nums - 1].init();//init the camera view port dependent textures
		cameras[cameras_nums - 1].update_camera();//update camera
		wglMakeCurrent(hdc, 0);//make the rc current

		return cameras_nums - 1;//return the added camera's ID
	}
	void Renderer::add_current_camera(unsigned long int camera_ID)
	{
		if (camera_ID >= cameras_nums)
			return;
		current_camera_nums++;//add the numbers
		unsigned long int *temp_current_camera = (unsigned long int*)malloc(sizeof(unsigned long int)*current_camera_nums);//allocated new data
		for (register unsigned long int i = 0; i < current_camera_nums - 1; i++)
			temp_current_camera[i] = current_cameras[i];//copy last data
		free(current_cameras);//free last data
		temp_current_camera[current_camera_nums - 1] = camera_ID;//add new camera
		current_cameras = temp_current_camera;//pass the new pointer
	}
	void Renderer::delete_current_camera(unsigned long int camera_ID)
	{
		if (camera_ID >= cameras_nums)
			return;
		unsigned long int TCCID = 0;//the id of the cammera in current cameras
		bool camre_in_the_list = 0;//is camera in the list
		for (register unsigned long int i = 0; i < current_camera_nums; i++)
			if (camera_ID == current_cameras[i])
			{
				TCCID = i;
				camre_in_the_list = 1;
			}
		if (camre_in_the_list)
		{
			unsigned long int *temp_current_camera = (unsigned long int*)malloc(sizeof(unsigned long int)*(current_camera_nums - 1));
			for (register unsigned long int i = 0; i < current_camera_nums - 1; i++)
				if (i >= TCCID)
					temp_current_camera[i] = current_cameras[i + 1];
				else
					temp_current_camera[i] = current_cameras[i];
			free(current_cameras);
			current_cameras = temp_current_camera;
			current_camera_nums--;
		}
	}
	unsigned long int Renderer::add_texture(unsigned char *irgba, unsigned int w, unsigned int h, unsigned short int NOC, bool icompressed, unsigned long int icompression_method, unsigned long int idata_size)
	{
		texture_nums++;//add number of cameras by 1
		texture *textures_the_next = (texture *)malloc(sizeof(texture) * texture_nums);//allocate new meshes' memory


		for (register unsigned long int i = 0; i < texture_nums - 1; i++)
			textures_the_next[i].clone_NMA(textures[i]);//clone last data with no new memory allocation

		wglMakeCurrent(hdc, hrc);//make the rc current
		textures_the_next[texture_nums - 1].build(irgba, w, h, NOC, icompressed, icompression_method, icompression_method);//build the texture
		wglMakeCurrent(hdc, 0);//make the rc current

		free(textures);//free last data
		textures = textures_the_next;//replace the pointer to new camera data
		return texture_nums - 1;//return the added camera's ID

	}
#ifdef Deferred
	void Renderer::render()
	{
		/***************************************************************/
		/***************************************************************/
		/**********************calculate lights*************************/
		/***************************************************************/
		unsigned long int number_of_lights = 0;
		unsigned long int shadowmapped_lights_num = 0;
		light lights_in_the_scene[MaxLightNums];
		unsigned long int lights_in_the_scene_index[MaxLightNums];
		for (register unsigned long int i = 0; i < last_light_ID; i++)//sort light by enablity 
			if (lights[i].light_enabled)
			{
				lights_in_the_scene_index[number_of_lights] = i;
				number_of_lights++;
				if (number_of_lights == MaxLightNums)//reached the limit?
					break;
			}
		unsigned long int temp_light;
		for (register unsigned long int i = 0; i < number_of_lights; i++)//sort light by shadow maping
			if (lights[lights_in_the_scene_index[i]].shadow_map)
			{
				if (lights_in_the_scene_index[i] != shadowmapped_lights_num)//no need to sort the lightis in place
				{
					temp_light = lights_in_the_scene_index[shadowmapped_lights_num];
					lights_in_the_scene_index[shadowmapped_lights_num] = lights_in_the_scene_index[i];
					lights_in_the_scene_index[i] = temp_light;
				}
				shadowmapped_lights_num++;//a light needs shadow map
				if (shadowmapped_lights_num == MaxShadowmapsNums)
					break;
			}
		/***************************************************************/
		/**********************calculate lights*************************/
		/***************************************************************/
		/***************************************************************/



		GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		wglMakeCurrent(hdc, hrc);
		for (unsigned long int camera_ID = 0; camera_ID < current_camera_nums; camera_ID++)
		{
			if (cameras[current_cameras[camera_ID]].needs_update)//camera needs update
				cameras[current_cameras[camera_ID]].update_camera();//update the camers
			
			camera curent_cam = cameras[current_cameras[camera_ID]];//for easier use

			/***************************************************************/
			/***************************************************************/
			/***********************frustum culling*************************/
			/***************************************************************/
			unsigned long int meshs_in_the_scene = 0;//number of objects in the scene

			point temp_point,ftemp_points[2];
			float AABB[2][3];

			for (register unsigned long int i = 0; i < mesh_nums; i++)//go through each mesh
			{
				for (register int j = 0; j < 8; j++)//go through the 8 points of the cube
				{
					temp_point = curent_cam.ViewMatrix*(meshes[i].model_matrix*meshes[i].mesh_cube[j]);
					if (j == 0)
					{
						//preset for comparision
						AABB[0][0] = float(temp_point.x);
						AABB[0][1] = float(temp_point.y);
						AABB[0][2] = float(temp_point.z);
						AABB[1][0] = float(temp_point.x);
						AABB[1][1] = float(temp_point.y);
						AABB[1][2] = float(temp_point.z);
					}
					else
					{
						//get the AABB values, max and min x and y
						AABB[0][0] = min(AABB[0][0], float(temp_point.x));
						AABB[0][1] = min(AABB[0][1], float(temp_point.y));
						AABB[0][2] = min(AABB[0][2], float(temp_point.z));
						AABB[1][0] = max(AABB[1][0], float(temp_point.x));
						AABB[1][1] = max(AABB[1][1], float(temp_point.y));
						AABB[1][2] = max(AABB[1][2], float(temp_point.z));
					}
					float Z_value = float(temp_point.z);//the point z
					if (Z_value <= -curent_cam.Near)//the point is out of the scene skip
						temp_point = curent_cam.ProjectionMatrix*temp_point;
					if (Z_value <= -curent_cam.Near&& temp_point.y <= 1.0 && temp_point.x <= 1.0 &&temp_point.y >= -1.0 && temp_point.x >= -1.0)//mesh is in the scene
					{
						mesh_draw_order[meshs_in_the_scene].A = i;//put the mesh to the draw order
						vector TempVector;
						TempVector.build(curent_cam.camera_position, meshes[i].model_matrix*meshes[i].mesh_center);
						mesh_draw_order[meshs_in_the_scene].B = TempVector.getsizeSq();
						meshs_in_the_scene++;//a mesh is added
						break;
					}
					else if (j == 7 && AABB[0][2] <= -curent_cam.Near&&AABB[1][2] >= -curent_cam.Far)//frustum in mesh
					{
						ftemp_points[0].build(AABB[0][0], AABB[0][1], max(AABB[0][2], -curent_cam.Far));//2 max points
						ftemp_points[1].build(AABB[1][0], AABB[1][1], max(AABB[0][2], -curent_cam.Far));
						ftemp_points[0] = curent_cam.ProjectionMatrix*ftemp_points[0];
						ftemp_points[1] = curent_cam.ProjectionMatrix*ftemp_points[1];
						if (((ftemp_points[0].x <= 1.0 && ftemp_points[1].x >= 1.0) || (ftemp_points[0].x <= -1.0 && ftemp_points[1].x >= -1.0) || (ftemp_points[0].x >= -1.0 && ftemp_points[1].x <= 1.0)) && ((ftemp_points[0].y <= 1.0 && ftemp_points[1].y >= 1.0) || (ftemp_points[0].y <= -1.0 && ftemp_points[1].y >= -1.0) || (ftemp_points[0].y >= -1.0 && ftemp_points[1].y <= 1.0)))//the frustum is in mesh
						{
							mesh_draw_order[meshs_in_the_scene].A = i;//put the mesh to the draw order
							vector TempVector;
							TempVector.build(curent_cam.camera_position, meshes[i].model_matrix*meshes[i].mesh_center);
							mesh_draw_order[meshs_in_the_scene].B = TempVector.getsizeSq();
							meshs_in_the_scene++;//a mesh is added
						}
					}
				}
			}
			/***************************************************************/
			/***********************frustum culling*************************/
			/***************************************************************/
			/***************************************************************/



			/***************************************************************/
			/***************************************************************/
			/********************front to back sorting**********************/
			/***************************************************************/
			std::sort(mesh_draw_order, mesh_draw_order + meshs_in_the_scene);
			/***************************************************************/
			/********************front to back sorting**********************/
			/***************************************************************/
			/***************************************************************/


			/******************************************************************/
			/**************************test console****************************/
			/******************************************************************/
			/******************************************************************/
			{
				static double S, E;

				static int counter;

				COORD textcoord, pos;
				CHAR_INFO stringdata[20];
				_SMALL_RECT recta;


				counter++;

				if (counter == 100)
				{
					counter = 0;
					E = clock();
					if (E - S > 0.001)
						Final_str[0] = inttostring(int(floor(100000.0 / (E - S))));
					else
						Final_str[0] = inttostring(60);
					Final_str[0] = Final_str[0] + "   ";
					S = clock();

					textcoord.X = (SHORT)strlen(Final_str[0].string1);
					textcoord.Y = 1;
					pos.X = 0;
					pos.Y = 0;
					for (SHORT charachter = 0; charachter < textcoord.X; charachter++)
					{
						stringdata[charachter].Attributes = 10;
						stringdata[charachter].Char.AsciiChar = Final_str[0].string1[charachter];
					}
					recta.Top = (SHORT)renderer_ID * 2;
					recta.Bottom = (SHORT)renderer_ID * 2 + 1;
					recta.Right = textcoord.X + 4 + 40;
					recta.Left = 5 + 40;
					WriteConsoleOutput(myConsoleHandle, stringdata, textcoord, pos, &recta);
				}

				Final_str[0] = inttostring(meshs_in_the_scene);
				Final_str[0] = Final_str[0] + "   ";
				textcoord.X = (SHORT)strlen(Final_str[0].string1);
				textcoord.Y = 1;
				pos.X = 0;
				pos.Y = 0;
				for (SHORT charachter = 0; charachter < textcoord.X; charachter++)
				{
					stringdata[charachter].Attributes = 11;
					stringdata[charachter].Char.AsciiChar = Final_str[0].string1[charachter];
				}
				recta.Top = (SHORT)renderer_ID * 2 + 1;
				recta.Bottom = (SHORT)renderer_ID * 2 + 2;
				recta.Right = textcoord.X + 17 + 40;
				recta.Left = 18 + 40;
				WriteConsoleOutput(myConsoleHandle, stringdata, textcoord, pos, &recta);
			}
			/******************************************************************/
			/******************************************************************/
			/**************************test console****************************/
			/******************************************************************/


			/***************************************************************/
			/***************************************************************/
			/***********************writing G-buffer************************/
			/***************************************************************/
			/*G-buffer:
			normals                RGB	MRT 1st texture
			roughness map          A	MRT 1st texture
			texture color          RGBA	MRT 2nd texture
			material/Mesh ID       R	MRT 3rd texture
			*/
			glEnable(GL_DEPTH_TEST); //enable 
			
			glBindFramebuffer(GL_FRAMEBUFFER, curent_cam.GBuffer_FBO_ID);

			glDrawBuffers(3, buffers);//will draw to 3 color buffer

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);
			glUseProgram(Shaders[2]);//G buffer shader
			glViewport(curent_cam.camera_viewport[0],//set the view port
				curent_cam.camera_viewport[1],
				curent_cam.camera_viewport[2],
				curent_cam.camera_viewport[3]);
			glUniformMatrix4fv(Projection_Matrix_Location[2], 1, 1, curent_cam.ProjectionMatrix.x);//set view and projection matrices
			glUniformMatrix4fv(View_Matrix_Location[2], 1, 1, curent_cam.ViewMatrix.x);
			for (register unsigned long int i = 0; i < meshs_in_the_scene; i++)//go through the meshs in the scene
			{
				glUniform1ui(Mat_ID_Location[2], mesh_draw_order[i].A);

				glUniformMatrix4fv(Model_Matrix_Location[2], 1, 1, meshes[mesh_draw_order[i].A].model_matrix.x);//set model and normal matrices
				glUniformMatrix4fv(Normal_Matrix_Location[2], 1, 0, Inverse(meshes[mesh_draw_order[i].A].model_matrix).x);
				if (meshes[mesh_draw_order[i].A].needs_update)//update data if needed
					meshes[mesh_draw_order[i].A].update_vbo();
				if (meshes[mesh_draw_order[i].A].subdata_changed)//update data if needed
					meshes[mesh_draw_order[i].A].remap_vbo();
				glBindVertexArray(meshes[mesh_draw_order[i].A].VAO_ID);//bind vao to draw
				glDrawElements(GL_TRIANGLES, meshes[mesh_draw_order[i].A].vert_nums, GL_UNSIGNED_INT, meshes[mesh_draw_order[i].A].indices);
				glBindVertexArray(0);
			}
			/***************************************************************/
			/***********************writing G-buffer************************/
			/***************************************************************/
			/***************************************************************/

			/***************************************************************/
			/***************************************************************/
			/**************************the gather pass**********************/
			/***************************************************************/

			glUseProgram(Shaders[3]);



			/***************************************************************/
			/***************************************************************/
			/***********************update cam data*************************/
			/***************************************************************/
			glUniform3f(Camera_Position_Location[3],
				float(curent_cam.camera_position.x),
				float(curent_cam.camera_position.y),
				float(curent_cam.camera_position.z));
			glUniform1f(Camera_Far_Location[3], curent_cam.Far);
			glUniform1f(Camera_Near_Location[3], curent_cam.Near);
			/***************************************************************/
			/***********************update cam data*************************/
			/***************************************************************/
			/***************************************************************/



			glUniform1i(GBuffer_Sampler_loaction[3], 0);//fill the samplers
			glUniform1i(GBuffer_Sampler_loaction[3] + 1, 1);
			glUniform1i(GBuffer_Mat_ID_Sampler_loaction[3], 2);
			glUniform1i(GBuffer_Depth_Buffer_Sampler_loaction[3], 3);

			//bind the GBuffer to samplers
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, curent_cam.GBufferID[0]);

			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, curent_cam.GBufferID[1]);

			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, curent_cam.GBufferID[2]);

			glActiveTexture(GL_TEXTURE0 + 3);
			glBindTexture(GL_TEXTURE_2D, curent_cam.GBuffer_Depth);

			glDisable(GL_DEPTH_TEST); //we dont need depth test here IDIOT

			glBindFramebuffer(GL_FRAMEBUFFER, 0);//back to main buffer
			glDrawBuffers(1, buffers);//back to drawing to one color buffer

			glViewport(curent_cam.camera_viewport[0],//set the view port
				curent_cam.camera_viewport[1],
				curent_cam.camera_viewport[2],
				curent_cam.camera_viewport[3]);


			glBindVertexArray(ScreenQuadVAO);//bind vao to draw
			glDrawArrays(GL_QUADS, 0, 4);
			glBindVertexArray(0);

			/***************************************************************/
			/**************************the gather pass**********************/
			/***************************************************************/
			/***************************************************************/

		}
		glFlush();
		SwapBuffers(hdc);
		wglMakeCurrent(hdc, 0);//make the rc current
	}
#endif
	/******************************************************************/
	/**************************test functions**************************/
	/******************************************************************/
	/******************************************************************/
	void Renderer::test_init()
	{
		mesh_nums = 0;
		vert_nums = 0;
		total_size = 0;            //initialize values
		cameras_nums = 0;
		current_camera_nums = 0;
		last_light_ID = 0;
		texture_nums = 0;
		light_data_changed = 0;
		meshes = (Mesh*)malloc(0);
		cameras = (camera*)malloc(0);//so they can be freed
		textures = (texture*)malloc(0);
		current_cameras = (unsigned long int*)malloc(0);
		for (register int i = 0; i < 100; i++)
			lights[i].build();
		renderer_ID = renderer_class_nums;//give it an ID
		renderer_class_nums++;//a renderer added
		SetupPixelFormat_WND_DC(hdc);//setup the pixel format
		hrc = wglCreateContext(hdc); //creat a render context
		wglMakeCurrent(hdc, hrc);//make the rc current
		Init_glew();//initialize glew functions
		glEnable(GL_DEPTH_TEST); //enable 
		glDepthFunc(GL_LEQUAL); //less or equal(closer) z are writen
		glEnable(GL_CULL_FACE); //cull the back face 
		glCullFace(GL_BACK);//cull the back face
		glClearDepth(1.0f);//clear the depth with 1


		glGenBuffers(1, &light_data_UBO_ID);//generate the UBO
		glBindBufferBase(GL_UNIFORM_BUFFER, lights_UBO_binding_point, light_data_UBO_ID);

		glGenBuffers(1, &text_offset_UBO_ID);//generate the UBO
		glBindBufferBase(GL_UNIFORM_BUFFER, text_offsets_UBO_binding_point, text_offset_UBO_ID);

		Shaders[0] = LoadShaders("..\\HG3D Engine\\VS00.txt", "..\\HG3D Engine\\FS00.txt");//load shaders
		Light_Block_Index[0] = glGetUniformBlockIndex(Shaders[0], "lights");//get lights index
		glUniformBlockBinding(Shaders[0], Light_Block_Index[0], lights_UBO_binding_point);

		Text_Offset_Block_Index[0] = glGetUniformBlockIndex(Shaders[0], "TSOS");//get lights index
		glUniformBlockBinding(Shaders[0], Text_Offset_Block_Index[0], text_offsets_UBO_binding_point);

		//get uniforms loactions
		Model_Matrix_Location[0] = glGetUniformLocation(Shaders[0], "ModelMatrix");
		Normal_Matrix_Location[0] = glGetUniformLocation(Shaders[0], "NormalMatrix");
		Projection_Matrix_Location[0] = glGetUniformLocation(Shaders[0], "ProjectionMatrix");
		View_Matrix_Location[0] = glGetUniformLocation(Shaders[0], "ViewMatrix");
		Camera_Position_Location[0] = glGetUniformLocation(Shaders[0], "Camera_Position");
		Lights_Nums_Location[0] = glGetUniformLocation(Shaders[0], "Lights_Nums");
		Lights_Proj_View_Matrix_Location[0] = glGetUniformLocation(Shaders[0], "light_proj_view_matrix");
		Inv_Lights_Proj_View_Matrix_Location[0] = glGetUniformLocation(Shaders[0], "inv_light_proj_view_matrix");
		Shadowmap_Sampler_Location[0] = glGetUniformLocation(Shaders[0], "SMTex_Sampler");
#ifdef VSM
		float invsqrt2 = 1.0f / sqrt(2.0f);
		float invSMres = 1.0f / float(Shadowmap_Res)*2.0f;
		float text_offsets[64] = {
			invSMres,			 0.0f,				 0.0f, 0.0f,
			0.0f,				 invSMres,			 0.0f, 0.0f,
			invSMres*invsqrt2,	 invSMres*invsqrt2,	 0.0f, 0.0f,
			invSMres*invsqrt2,	 -invSMres*invsqrt2, 0.0f, 0.0f,
			-invSMres,			 0.0f,				 0.0f, 0.0f,
			0.0f,				 -invSMres,			 0.0f, 0.0f,
			-invSMres*invsqrt2,	 -invSMres*invsqrt2, 0.0f, 0.0f,
			-invSMres*invsqrt2,	 invSMres*invsqrt2,	 0.0f, 0.0f
		};
		glBindBuffer(GL_UNIFORM_BUFFER, text_offset_UBO_ID);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 64, (void*)text_offsets, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
#endif
#ifdef PCF
		float GNDF[3];
		float GDNFSum = 0.0f;
		float StandardDeviation = 0.840896f;
		for (int i = 0; i < 3; i++)
		{
			GNDF[i] = (float)exp(float(-min(i*i, 1)) / (2.0f * StandardDeviation*StandardDeviation)) / sqrt(2.0f * PI* StandardDeviation*StandardDeviation);
			if (i == 2)
				GNDF[i] *= GNDF[i];
			else
				GNDF[i] *= GNDF[0];
			if (i == 0)
				GDNFSum += GNDF[i];
			else
				GDNFSum += GNDF[i] * 4.0f;
		}
		for (int i = 0; i < 3; i++)
			GNDF[i] /= GDNFSum;
		float invSMres = 1.0f / float(Shadowmap_Res);
		float text_offsets[64] = {
			invSMres,			 0.0f,				 GNDF[1], GNDF[0],
			0.0f,				 invSMres,			 GNDF[1], 0.0f,
			invSMres,			 invSMres,			 GNDF[2], 0.0f,
			invSMres,			 -invSMres,			 GNDF[2], 0.0f,
			-invSMres,			 0.0f,				 GNDF[1], 0.0f,
			0.0f,				 -invSMres,			 GNDF[1], 0.0f,
			-invSMres,			 -invSMres,			 GNDF[2], 0.0f,
			-invSMres,			 invSMres,			 GNDF[2], 0.0f
		};
		glBindBuffer(GL_UNIFORM_BUFFER, text_offset_UBO_ID);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 64, (void*)text_offsets, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
#endif

		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		/******************************************************************/
		AllocConsole();
		myConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		string tempstring;
		tempstring = "Renderer ID: ";
		tempstring = tempstring + inttostring(renderer_ID);
		tempstring = tempstring + '\n';
		WriteConsole(myConsoleHandle, tempstring.string1, (DWORD)strlen(tempstring.string1), &cCharsWritten, NULL);
		WriteConsole(myConsoleHandle, Final_str[0].string1, (DWORD)strlen(Final_str[0].string1), &cCharsWritten, NULL);
		WriteConsole(myConsoleHandle, Final_str[1].string1, (DWORD)strlen(Final_str[1].string1), &cCharsWritten, NULL);
		/******************************************************************/
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		//load the shadow mapping shader
		Shaders[1] = LoadShaders("..\\HG3D Engine\\VS01.txt", "..\\HG3D Engine\\FS01.txt");//load shaders
		Model_Matrix_Location[1] = glGetUniformLocation(Shaders[1], "ModelMatrix");
		View_Matrix_Location[1] = glGetUniformLocation(Shaders[1], "ProjViewMatrix");
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		/******************************************************************/
		WriteConsole(myConsoleHandle, Final_str[0].string1, (DWORD)strlen(Final_str[0].string1), &cCharsWritten, NULL);
		WriteConsole(myConsoleHandle, Final_str[1].string1, (DWORD)strlen(Final_str[1].string1), &cCharsWritten, NULL);
		/******************************************************************/
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		/********************************************************************/
		/*					setting up shadow map textures					*/
		/********************************************************************/
		int LayerRes = Shadowmap_Res;
		for (register int i = 0; i < MaxCascadessNums; i++)
		{
			glGenTextures(1, &Shadow_Maps_Tex_ID[i]);//generate the texture so you can free it
			glBindTexture(GL_TEXTURE_2D_ARRAY, Shadow_Maps_Tex_ID[i]);//bind the texture
#ifdef PCF
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, LayerRes, LayerRes, MaxShadowmapsNums, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
#endif
#ifdef VSM
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RG32F, LayerRes, LayerRes, MaxShadowmapsNums, 0, GL_RG, GL_FLOAT, NULL);
#endif
#ifdef DeferredSM
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32, LayerRes, LayerRes, MaxShadowmapsNums, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
#endif
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//linear filter (we'll use it)
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //clamping dosnt really matter since we wont read out of the range
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			LayerRes /= 2;
		}
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);//unbind the texture
											  /********************************************************************/
											  /*					setting up shadow map textures					*/
											  /********************************************************************/
											  /********************************************************************/
											  /*						setting up shadow map fbo					*/
											  /********************************************************************/
		glGenFramebuffers(1, &Shadowmap_FBO_ID);//set the fbo
#ifdef VSM
		glGenRenderbuffers(1, &Shadowmap_RBO_ID);
		glBindRenderbuffer(GL_RENDERBUFFER, Shadowmap_RBO_ID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Shadowmap_Res, Shadowmap_Res);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, Shadowmap_FBO_ID);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Shadowmap_RBO_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
		/********************************************************************/
		/*						setting up shadow map fbo					*/
		/********************************************************************/
		wglMakeCurrent(hdc, 0);//make the rc current

	}
	void Renderer::test_render()
	{
		wglMakeCurrent(hdc, hrc);//make the rc current
		int number_of_lights = 0;
		int shadowmapped_lights_num = 0;
		light lights_in_the_scene[MaxLightNums];
		for (register unsigned long int i = 0; i < last_light_ID; i++)//sort light by enablity 
			if (lights[i].light_enabled)
			{
				lights_in_the_scene[number_of_lights] = lights[i];
				number_of_lights++;
				if (number_of_lights == MaxLightNums)//reached the limit?
					break;
			}
		light temp_light;
		for (register int i = 0; i < number_of_lights; i++)//sort light by shadow maping
			if (lights_in_the_scene[i].shadow_map)
			{
				if (i != shadowmapped_lights_num)//no need to sort the lightis in place
				{
					temp_light = lights_in_the_scene[shadowmapped_lights_num];
					lights_in_the_scene[shadowmapped_lights_num] = lights_in_the_scene[i];
					lights_in_the_scene[i] = temp_light;
				}
				shadowmapped_lights_num++;//a light needs shadow map
				if (shadowmapped_lights_num == MaxShadowmapsNums)
					break;
			}
		_4x4matrix light_proj_view_main_proj[MaxShadowmapsNums];
		_4x4matrix light_proj_view[MaxShadowmapsNums*MaxCascadessNums];

		//CSM: view frustum data
		point viewFrustums[(MaxCascadessNums + 1) * 4];//cascaded view frustms
		point ProjectedFrusums[(MaxCascadessNums + 1) * 4];//cascaded view frustms projectedd with light proj

		float NearZ, nearRatio;

		if (cameras[current_cameras[0]].needs_update)//update if needed
			cameras[current_cameras[0]].update_camera();

		//CSM: calc view frustum data
		for (register int i = 0; i <= MaxCascadessNums; i++)
		{
			NearZ = float(i*i) / float(MaxCascadessNums*MaxCascadessNums)*(-cameras[current_cameras[0]].Far + cameras[current_cameras[0]].Near) - cameras[current_cameras[0]].Near;
			nearRatio = NearZ / -cameras[current_cameras[0]].Near;
			viewFrustums[i * 4].build(nearRatio*cameras[current_cameras[0]].Right, nearRatio*cameras[current_cameras[0]].Top, NearZ);//calculate the cascaded view frustums in vew space

			point tempout = cameras[current_cameras[0]].ProjectionMatrix*viewFrustums[i * 4];

			viewFrustums[i * 4 + 1].build(nearRatio*cameras[current_cameras[0]].Left, nearRatio*cameras[current_cameras[0]].Top, NearZ);
			viewFrustums[i * 4 + 2].build(nearRatio*cameras[current_cameras[0]].Right, nearRatio*cameras[current_cameras[0]].Buttom, NearZ);
			viewFrustums[i * 4 + 3].build(nearRatio*cameras[current_cameras[0]].Left, nearRatio*cameras[current_cameras[0]].Buttom, NearZ);
		}
		_4x4matrix InvView = Inverse(cameras[current_cameras[0]].ViewMatrix);
		for (register int i = 0; i <= MaxCascadessNums * 4; i++)//transform frustums to world space
			viewFrustums[i] = InvView*viewFrustums[i];

		glUseProgram(Shaders[1]);

		glBindFramebuffer(GL_FRAMEBUFFER, Shadowmap_FBO_ID);//bind the frame buffer

		//declare temp variables
		point light_temp_pos;
		vector arbitrary_vecs[2], forward;
		arbitrary_vecs[0].build(1.0f, 0.0f, 0.0f);
		arbitrary_vecs[1].build(0.0f, 1.0f, 0.0f);

		int _2pow;

		for (register int i = 0; i < shadowmapped_lights_num; i++)
		{
			//calculate the light vew projection matrices
			light_temp_pos.build(lights_in_the_scene[i].light_position[0], lights_in_the_scene[i].light_position[1], lights_in_the_scene[i].light_position[2]);//build the light pos point
			forward.build(lights_in_the_scene[i].direction[0], lights_in_the_scene[i].direction[1], lights_in_the_scene[i].direction[2]);//build the light direction vector
			_4x4matrix temp_view_mat,light_view_inv;
			int the_crosser;
			if (abs(lights_in_the_scene[i].direction[1]) < 0.01f&&abs(lights_in_the_scene[i].direction[2]) < 0.01f)//check not to let later errors and then calculate the view matrix
				the_crosser = 1;
			else
				the_crosser = 0;
			temp_view_mat = LookAt(light_temp_pos, forward, cross(forward, arbitrary_vecs[the_crosser]));

			light_proj_view_main_proj[i] = Projection(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, lights_in_the_scene[i].max_radius);

			float deepestZ[MaxCascadessNums + 1];
			for (register int j = 0; j < (MaxCascadessNums + 1); j++)
				deepestZ[j] = -1.0f;
			//CSM: calculate CSM parts
			for (register int j = 0; j < (MaxCascadessNums + 1) * 4; j++)
			{
				ProjectedFrusums[j] = temp_view_mat * viewFrustums[j];
				if (float(ProjectedFrusums[j].z) < deepestZ[j/4])
					deepestZ[j/4] = float(ProjectedFrusums[j].z);
				ProjectedFrusums[j].z = -abs(ProjectedFrusums[j].z);
				ProjectedFrusums[j] = light_proj_view_main_proj[i] * ProjectedFrusums[j];

			}

			light_proj_view_main_proj[i] = light_proj_view_main_proj[i] * temp_view_mat;

			light_view_inv = Inverse(temp_view_mat);

			for (register int m = 0; m < MaxCascadessNums; m++)
			{
				_2pow = int(pow(2.0f, float(m)));
				glViewport(0, 0, Shadowmap_Res / _2pow, Shadowmap_Res / _2pow);//draw on the whole texture

				//CSM: find CSM squares and send to the shader
				float Exts[MaxCascadessNums][4];//save the max and min x and y for each frustum cascades

				Exts[m][0] = float(ProjectedFrusums[m * 4].x);
				Exts[m][1] = float(ProjectedFrusums[m * 4].x);
				Exts[m][2] = float(ProjectedFrusums[m * 4].y);
				Exts[m][3] = float(ProjectedFrusums[m * 4].y);
				for (register int l = 1; l < 8; l++)
				{
					Exts[m][0] = min(Exts[m][0], float(ProjectedFrusums[m * 4 + l].x));
					Exts[m][1] = max(Exts[m][1], float(ProjectedFrusums[m * 4 + l].x));
					Exts[m][2] = min(Exts[m][2], float(ProjectedFrusums[m * 4 + l].y));
					Exts[m][3] = max(Exts[m][3], float(ProjectedFrusums[m * 4 + l].y));
				}
				for (register int l = 0; l < 4; l++)
					Exts[m][l] = max(-1.0f, min(Exts[m][l], 1.0f));

				light_proj_view[i*MaxCascadessNums + m] = Projection(Exts[m][0], Exts[m][1], Exts[m][2], Exts[m][3], 1.0f, min(/*-min(deepestZ[m], deepestZ[m + 1]) + 100.0f*/10000000000.0f, lights_in_the_scene[i].max_radius))*temp_view_mat;

				glUniformMatrix4fv(View_Matrix_Location[1], 1, 1, light_proj_view[i*MaxCascadessNums + m].x);//set view matrix
				HG3D_Engine::point  a;
				a.build(0.0f, 100.0f, 50.0f- lights[0].max_radius);
				a = light_proj_view[i*MaxCascadessNums + m] * a;
#ifdef VSM
				glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Shadow_Maps_Tex_ID[m], 0, i);//set the texture as color buffer and start drawing
#endif
#ifdef PCF
				glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Shadow_Maps_Tex_ID[m], 0, i);//set the texture as depth buffer and start drawing
				glCullFace(GL_FRONT);//cull the back face
#endif
#ifdef DeferredSM
				glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Shadow_Maps_Tex_ID[m], 0, i);//set the texture as depth buffer and start drawing
#endif
				glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);//clear the depth

				for (register unsigned long int j = 0; j < mesh_nums; j++)
				{
					if (meshes[j].needs_update)//update data if needed
						meshes[j].update_vbo();
					if (meshes[j].subdata_changed)//update data if needed
						meshes[j].remap_vbo();
					glUniformMatrix4fv(Model_Matrix_Location[1], 1, 1, meshes[j].model_matrix.x);//set model matrix
					glBindVertexArray(meshes[j].VAO_ID);//bind vao to draw
					glDrawElements(GL_TRIANGLES, meshes[j].vert_nums, GL_UNSIGNED_INT, meshes[j].indices);
					glBindVertexArray(0);
				}
#ifdef PCF
				glCullFace(GL_BACK);//cull the back face
#endif
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//final image
		glUseProgram(Shaders[0]);
		if (textures[0].needs_update)
			textures[0].update();
		//send light data
		glUniform1i(Lights_Nums_Location[0], number_of_lights);//send light data to UBO
		glBindBuffer(GL_UNIFORM_BUFFER, light_data_UBO_ID);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(light) * MaxLightNums, (void*)lights_in_the_scene, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		for (int register i = 0; i < MaxCascadessNums; i++)
		{
			glUniform1i(Shadowmap_Sampler_Location[0] + i, i);//send shadow map samplers to shader
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D_ARRAY, Shadow_Maps_Tex_ID[i]);
		}
		glUniformMatrix4fv(Lights_Proj_View_Matrix_Location[0], MaxShadowmapsNums*MaxCascadessNums, 1, (float*)light_proj_view);//set view and projection matrices
		_4x4matrix inv_light_proj_view[MaxShadowmapsNums*MaxCascadessNums];
		for (int i = 0; i < MaxShadowmapsNums*MaxCascadessNums; i++)
			inv_light_proj_view[i] = Inverse(light_proj_view[i]);
		glUniformMatrix4fv(Inv_Lights_Proj_View_Matrix_Location[0], MaxShadowmapsNums*MaxCascadessNums, 1, (float*)inv_light_proj_view);//set view and projection matrices

		glClear(GL_COLOR_BUFFER_BIT);
		for (register unsigned long int j = 0; j < current_camera_nums; j++)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			if (cameras[current_cameras[j]].needs_update)
				cameras[current_cameras[j]].update_camera();

			glViewport(cameras[current_cameras[j]].camera_viewport[0],//set the view port
				cameras[current_cameras[j]].camera_viewport[1],
				cameras[current_cameras[j]].camera_viewport[2],
				cameras[current_cameras[j]].camera_viewport[3]);

			glUniformMatrix4fv(Projection_Matrix_Location[0], 1, 1, cameras[current_cameras[j]].ProjectionMatrix.x);//set view and projection matrices
			glUniformMatrix4fv(View_Matrix_Location[0], 1, 1, cameras[current_cameras[j]].ViewMatrix.x);

			glUniform3f(Camera_Position_Location[0],
				float(cameras[current_cameras[j]].camera_position.x),
				float(cameras[current_cameras[j]].camera_position.y),
				float(cameras[current_cameras[j]].camera_position.z));

			for (register unsigned long int i = 0; i < mesh_nums; i++)
			{
				if (meshes[i].needs_update)//update data if needed
					meshes[i].update_vbo();
				if (meshes[i].subdata_changed)//update data if needed
					meshes[i].remap_vbo();
				glUniformMatrix4fv(Model_Matrix_Location[0], 1, 1, meshes[i].model_matrix.x);//set model and normal matrices
				glUniformMatrix4fv(Normal_Matrix_Location[0], 1, 0, Inverse(meshes[i].model_matrix).x);
				glBindVertexArray(meshes[i].VAO_ID);//bind vao to draw
				glDrawElements(GL_TRIANGLES, meshes[i].vert_nums, GL_UNSIGNED_INT, meshes[i].indices);
				glBindVertexArray(0);
			}
		}



		/*glUseProgram(0);
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D_ARRAY);
		glViewport(0, 0, 1366, 768);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0f, 100000.0f);
		glMatrixMode(GL_MODELVIEW);
		glShadeModel(GL_SMOOTH);
		glLoadIdentity();
		gluLookAt(0, 0, -3, 0, 0, 1, 0, 1, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_DEPTH_BUFFER_BIT);

		GLfloat color[4];
		color[0] = 1;
		color[1] = 1;
		color[2] = 1;
		color[3] = 1;
		glColor4fv(color);
		glBegin(GL_TRIANGLES);
		float hwratio = 1.0;
		glTexCoord2f(0, 0);  glVertex2f(0, 0);
		glTexCoord2f(1, 0);  glVertex2f(1, 0);
		glTexCoord2f(1, 1);  glVertex2f(1, 1);
		glTexCoord2f(1, 1);  glVertex2f(1, 1);
		glTexCoord2f(0, 1);  glVertex2f(0, 1);
		glTexCoord2f(0, 0);  glVertex2f(0, 0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		glDisable(GL_TEXTURE_2D_ARRAY);
		glEnable(GL_CULL_FACE);//*/








		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		/******************************************************************/
		static double S, E;

		static int counter;

		COORD textcoord, pos;
		CHAR_INFO stringdata[20];
		_SMALL_RECT recta;


		counter++;

		if (counter == 100)
		{
			counter = 0;
			E = clock();
			if (E - S > 0.001)
				Final_str[0] = doubletostring(100000.0 / (E - S));
			else
				Final_str[0] = inttostring(60);
			Final_str[0] = Final_str[0] + "   ";
			S = clock();

			textcoord.X = (SHORT)strlen(Final_str[0].string1);
			textcoord.Y = 1;
			pos.X = 0;
			pos.Y = 0;
			for (SHORT charachter = 0; charachter < textcoord.X; charachter++)
			{
				stringdata[charachter].Attributes = 10;
				stringdata[charachter].Char.AsciiChar = Final_str[0].string1[charachter];
			}
			recta.Top = (SHORT)renderer_ID * 2;
			recta.Bottom = (SHORT)renderer_ID * 2 + 1;
			recta.Right = textcoord.X + 4 + 40;
			recta.Left = 5 + 40;
			WriteConsoleOutput(myConsoleHandle, stringdata, textcoord, pos, &recta);
		}
		/******************************************************************/
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		






		glFlush();
		SwapBuffers(hdc);
		wglMakeCurrent(hdc, 0);//make the rc current
	}
	/******************************************************************/
	/******************************************************************/
	/**************************test functions**************************/
	/******************************************************************/
}