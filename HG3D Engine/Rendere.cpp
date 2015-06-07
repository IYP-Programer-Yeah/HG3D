#include "HG3D Engine.h"
#include "..\File_HG_FS\File_HG_FS.h"
#include "..\Shared Headers\hstring.h"
#include "GLew.h"
static unsigned long int renderer_class_nums = 0;
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
	Final_str[0] = "vertex shader log: \n";
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
			glGetShaderInfoLog(VertexShaderID, sizeof(Log.string1), &maxLength, Log.string1);//get the log
			Final_str[0] = Final_str[0] + Log;//add the log to the string
			Final_str[0] = Final_str[0] + "\n\n\n";//add triple new lines
		}
		else
			Final_str[0] = Final_str[0] + "well compiled. \n";//every thing went well
		glAttachShader(ProgramID, VertexShaderID);
	}
	else
		Final_str[0] = Final_str[0] + "not found. \n";//no such path idiot
	Final_str[1] = "fragment shader log: \n";
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
			glGetShaderInfoLog(FragmentShaderID, sizeof(Log.string1)/2, &maxLength, Log.string1);//get the log
			Final_str[1] = Final_str[1] + Log;//add the log to the string
			Final_str[1] = Final_str[1] + "\n\n\n";//add triple new lines
		}
		else
			Final_str[1] = Final_str[1] + "well compiled. \n";//every thing went well
		glAttachShader(ProgramID, FragmentShaderID);
	}
	else
		Final_str[1] = Final_str[1] + "not found. \n";//no such path idiot
	glBindAttribLocation(ProgramID, 0, "Vertex");
	glBindAttribLocation(ProgramID, 1, "Normal");
	glBindAttribLocation(ProgramID, 2, "Coord");
	glBindFragDataLocation(ProgramID, 0, "Output1");//bind the out put 
	glLinkProgram(ProgramID);//link shaders
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	if (V_Shader_Path != NULL)
		glDeleteShader(VertexShaderID);//delete if only one was created
	if (F_Shader_Path != NULL)
		glDeleteShader(FragmentShaderID);//delete if only one was created
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
		24,                                     //16 bit z-buffer size
		0,                                      //no stencil buffer
		0,                                      //no aux buffer
		PFD_MAIN_PLANE,                         //main drawing plane
		0,                                      //reserved
		0, 0, 0 };                              //layer masks ignored
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);                            //layer masks ignored
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}
namespace HG3D_Engine
{
	void Renderer::init()
	{
		mesh_nums = 0;
		vert_nums = 0;
		total_size = 0;            //initialize values
		cameras_nums = 0;
		current_camera_nums = 0;
		rendere_ID = renderer_class_nums;//give it an ID
		renderer_class_nums++;//a renderer added
		SetupPixelFormat_WND_DC(hdc);//setup the pixel format
		hrc=wglCreateContext(hdc); //creat a render context
		wglMakeCurrent(hdc, hrc);//make the rc current
		Init_glew();//initialize glew functions
		glEnable(GL_DEPTH_TEST); //enable 
		glDepthFunc(GL_LEQUAL); //less or equal(closer) z are writen
		glEnable(GL_CULL_FACE); //cull the back face 
		glCullFace(GL_BACK);//cull the back face

		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		/******************************************************************/
		AllocConsole();
		myConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		string tempstring;
		tempstring = "Renderer ID: ";
		tempstring = tempstring + inttostring(rendere_ID);
		tempstring = tempstring + '\n';
		WriteConsole(myConsoleHandle, tempstring.string1, strlen(tempstring.string1), &cCharsWritten, NULL);
		/******************************************************************/
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/



		Mesh sponza0;
		sponza0.load_mesh("..\\HG3D 2.1\\Resource\\Models\\horse.obj");
		sponza0.clear_last_buff = 0;
		sponza0.update_vbo();

		GLuint shader=LoadShaders("..\\HG3D Engine\\VS00.txt", "..\\HG3D Engine\\FS00.txt");
		WriteConsole(myConsoleHandle, Final_str[0].string1, strlen(Final_str[0].string1), &cCharsWritten, NULL);
		WriteConsole(myConsoleHandle, Final_str[1].string1, strlen(Final_str[1].string1), &cCharsWritten, NULL);

		glUseProgram(shader);
		_4x4matrix proj = Projection(-1.0, 1.0, -1.0, 1.0, 1, 10000);
		glUniformMatrix4fv(glGetUniformLocation(shader, "ProjectionMatrix"), 1, 0, proj.x);
		vector up, forward;
		up.build((long double) 0.0, (long double) 1.0, (long double) 0.0);
		forward.build((long double)-1.0, (long double) 0.0, (long double) 0.0);
		point campos;
		campos.build(sponza0.mesh_center.x + 11.0, 0.0, 0.0);//(float)sponza0.mesh_center.y+0.0, (float)sponza0.mesh_center.z+0.0);
		_4x4matrix view = LookAt(campos, forward, up);
		sponza0.model_matrix.LoadScaler(100.0, 100.0, 100.0);
		_4x4matrix view_X_model = sponza0.model_matrix*view;
		glUniformMatrix4fv(glGetUniformLocation(shader, "ModelViewMatrix"), 1, 0, view_X_model.x);
		glClearDepth(1);
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0,0,1366, 768);
		glBindVertexArray(sponza0.VAO_ID);
		glDrawElements(GL_TRIANGLES, sponza0.vert_nums, GL_UNSIGNED_INT, sponza0.faces);
		SwapBuffers(hdc);
	}
}