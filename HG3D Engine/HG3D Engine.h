#include <Windows.h>
#include <gl/glew.h>
#include <gl/gl.h> 
#include <gl/glu.h>
#include <math.h>
#include <xmmintrin.h>

// load GL libs
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#ifndef HG3D_ENG  //check if included before
#define HG3D_ENG


#define Vert_Pos_Size      12                                //size of positions
#define Vert_Normal_Size   12                                //size of normals
#define Vert_Coord_Size    8                                 //size of coord
#define vertex_size        32                                //vertex size
#define face_size          12								 //face size

namespace HG3D_Engine
{
	//math libs
	class point //point class
	{
	public:
		long double x, y, z;
		void __declspec(dllexport) __fastcall build(long double ix, long double iy, long double iz); //build point
		void __declspec(dllexport) __fastcall build(float ix, float iy, float iz); //build point with float (idiot nvidia)
		void __declspec(dllexport) __fastcall move(long double ax = long double(0.0), long double ay = long double(0.0), long double az = long double(0.0)); //move point
		void __declspec(dllexport) __fastcall rotate(point from, long double ax = long double(0.0), long double ay = long double(0.0), long double az = long double(0.0)); //rotate point
		void __declspec(dllexport) __fastcall move(float ax = 0.0f, float ay = 0.0f, float az = 0.0f); //move point float
		void __declspec(dllexport) __fastcall rotate(point from, float ax = 0.0f, float ay = 0.0f, float az = 0.0f); //rotate point float
		void __declspec(dllexport) __fastcall operator =(point intery); //operator =
	};
	//end of point class
	class vector //3d vector class
	{
	public:
		long double x, y, z;
		void __declspec(dllexport) __fastcall build(point start, point end);//point to point vector
		void __declspec(dllexport) __fastcall build(long double ix, long double iy, long double iz);//simpile build
		void __declspec(dllexport) __fastcall build(float ix, float iy, float iz);//simpile build with float
		void __declspec(dllexport) __fastcall operator =(vector intery);//set value
		vector __declspec(dllexport) __fastcall operator +(vector entry);//vector vector sum
		vector __declspec(dllexport) __fastcall operator *(long double entry);//vector number multipliction
		vector __declspec(dllexport) __fastcall operator /(long double entry);//vector number division
		vector __declspec(dllexport) __fastcall operator *(float entry);//vector number multipliction
		vector __declspec(dllexport) __fastcall operator /(float entry);//vector number division
		long double __declspec(dllexport) __fastcall getsize();//get vector size
		long double __declspec(dllexport) __fastcall getsizeSq();//get vector size squared
	};
	//end of 3d vector class
	long double __declspec(dllexport) __fastcall dot(vector v1, vector v2); //dot product

	vector __declspec(dllexport) __fastcall cross(vector v1, vector v2); //cross product

	vector __declspec(dllexport) __fastcall normalize(vector v1); //normalize

	class _4x4matrix //4*4 matrix
	{
	public:
		union
		{
#ifdef _M_X64
			__declspec(align(16)) float x[16];
			__declspec(align(16)) float xy[4][4];
			__declspec(align(16)) __m128 data_row[4];
#else
			float x[16];
			float xy[4][4];
#endif
			
		};
		_4x4matrix __declspec(dllexport) __fastcall operator * (_4x4matrix input);//mat4 multiply
		vector __declspec(dllexport) __fastcall operator * (vector input);//mat4 multiply by vector
		point __declspec(dllexport) __fastcall operator * (point input);//mat4 multiply by point
		void __declspec(dllexport) __fastcall operator = (_4x4matrix input);//=
		void __declspec(dllexport) __fastcall LoadIdentity();//load matric with i
		void __declspec(dllexport) __fastcall LoadZero();//load matric with 0
		void __declspec(dllexport) __fastcall LoadScaler(float sx, float sy, float sz);//load scaler matrix
		void __declspec(dllexport) __fastcall LoadTranslate(float sx, float sy, float sz);//load translate matrix
		void __declspec(dllexport) __fastcall LoadRotation(vector axis, point origin, float theta);//load rotation matrix
	};
	//end of rotation class
	_4x4matrix LookAt(point cam_pos,vector forward,vector up);//get view mat

	_4x4matrix Projection(float left, float right, float buttom, float top, float nearz, float farz);//projection matrix

	_4x4matrix __declspec(dllexport) __fastcall InverseTranspose(_4x4matrix input);//invers mat

	_4x4matrix __declspec(dllexport) __fastcall Transpose(_4x4matrix input);//invers mat
	//end of math libs

	//mesh libs
	class face //face class
	{
	public:
		unsigned long int index[3];   //face indicies
		void __declspec(dllexport) operator=(face input);//= operator
	};
	//end of face class
	class vertex //vertex class
	{
	public:
		float x, y, z;       //positions
		float nx, ny, nz;    //normals
		float cx, cy;        //coords
		void __declspec(dllexport) operator=(vertex input);//= operator
	};
	//end of vertex class
	class Mat//material class
	{
	public:
		int illum;//the illumination method

		float Ambient[3];//ambient color
		float Specular[3];//specular color
		float Diffuse[3];//diffuse color
		float Emission[3];//Emission color
		float TF[3];//transmision filter color
		float dissolve;//transparancy 
		float Spec_Exponent;//specular reflection exponent range 1 to 10000
		float optical_density;//optical density (light bending throgh the mat) range 0.001 to 1
		float roughness;//roughness of the surface
		float sharpness;//sharpness of the reflection 0 to 1000

		void __declspec(dllexport) operator=(Mat input);//= operator

	};
	//end of mat class
	class Mesh//mesh class
	{
	public:

		Mat meshmat;  //mesh material

		unsigned long int vert_nums;      //number of verts
		unsigned long int faces_nums;     //number of faces
		unsigned long int total_size;     //total size of the verts data

		unsigned long int *indices;      //vert indices

		unsigned long int text_ID_diff;               //diffuse texture ID
		unsigned long int text_ID_spec;               //specular texture ID
		unsigned long int text_ID_normal;             //normal map texure ID
		unsigned long int text_ID_height;             //height map texture ID
		unsigned long int text_ID_mask;               //mask texture ID
		unsigned long int text_ID_alphamap;			  //apha map texture ID
		unsigned long int text_ID_dispmap;			  //displacement map texture ID
		unsigned long int text_ID_TFmap;			  //transmision filter map texture ID

		unsigned char MeshID[4];             //mesh id for defered shading

		bool have_diff_text;              //have diffuse texture? 
		bool have_spec_text;              //have specular texture? 
		bool have_NM_text;                //have normals map texture? 
		bool have_HM_text;                //have height map texture? 
		bool have_mask_text;              //have mask texture? 
		bool have_alphamap_text;		  //have alpha map texture?
		bool have_dispmap_text;			  //have displacement map texture?
		bool have_TFmap_text;			  //have transmision filter map texture?

		bool hidden;                      //is mesh hidden?
		bool clear_last_buff;             //clear last buffer in updat?
		bool needs_update;                //mesh needs an update
		bool subdata_changed;			  //a part of data is changed by user

		point mesh_cube[8];               //mesh's container cube
		point mesh_center;                //mesh-cube's center

		_4x4matrix model_matrix;          //model matrix will be sent to shader

		face *faces;                      //mesh faces
		vertex *verts;                    //mesh verts

		GLuint VAO_ID;                    //mesh vao ID
		GLuint VBO_ID;                    //mesh vbo ID

		void __declspec(dllexport) load_mesh(char *path);       //.obj mesh loader 3 verts for each face
		void __declspec(dllexport) load_optimized(char *path);  //.obj mesh loader reuse the verts (time consumeing process)
		void __declspec(dllexport) free_mesh();                 //free the mesh memory(cpu+gpu)

		void __declspec(dllexport) scale_model(float x, float y, float z);						//scale the model only
		void __declspec(dllexport) rotate_model_AIC(float theta, vector axis);					//about it's center
		void __declspec(dllexport) rotate_model(float theta, point origin, vector axis);		//about origin
		void __declspec(dllexport) move(vector movement);										//about origin

		void __declspec(dllexport) update_vbo();				//update mesh vbo
		void __declspec(dllexport) remap_vbo();					//update the subdata
		void __declspec(dllexport) operator=(Mesh input);		//= operator
		void __declspec(dllexport) clone_NMA(Mesh input);		//clone with the same memory allocation (no new memory is allocated)

		//NT = awesomeness
		vector m_Translation;
	};
	//end of mesh class
	//end of mesh libs

	//renderer libs
	class camera//camera class
	{
	public:
		_4x4matrix ViewMatrix;//modelview matrix
		_4x4matrix ProjectionMatrix;//projection

		float Left, Right, Buttom, Top, Near, Far;//projection property
		GLint camera_viewport[4];//view port property

		point camera_position;//camera position

		vector forward;//forwar vector
		vector up;//up vector

		bool needs_update;//camera needs an update
		void __declspec(dllexport) update_camera();//update camera
		void __declspec(dllexport) fps_camera(float pitch,float yaw,vector head_up);//pith and yaw the camera in fps mode

	};
	//end of camera class
	//texture class
	class texture
	{
	public:
		GLuint texture_name;//texture name given by the gl

		bool Repeat_X, Repeat_Y;//does it repeat over the face or it's clamped to edgs?
		bool needs_update;// check if needs update
		bool generate_mipmaps;//do you nrrd mipmaps

		unsigned short int number_of_components;// number of components

		unsigned char *rgba;//the rgba better exist for later change

		unsigned int width, height;//width and height

		unsigned long int total_size;
		
		void __declspec(dllexport) build(unsigned char *irgba, unsigned int w, unsigned int h, unsigned short int NOC);//will copy the irgba to rgb free your own allocation
		void __declspec(dllexport) free_data();//free all alloction as well as gpu memory
		void __declspec(dllexport) update(); //update the data
		void __declspec(dllexport) clone_NMA(texture input);//clone with the same memory allocation (no new memory is allocated)
	};
	//end of texture
	//light class
	class light
	{
	public:
		float light_position[3];//light pos
		float light_color[3];//light color
		float Attenuation[3];//attenuation values
		float direction[3];//the direction of light

		float max_radius;//the maximum radius of light contribution
		float cut_off_cos;//cosine of the the the cut of angle 

		bool light_enabled;//is the light enabled
		bool shadow_map;//does it use shadow maps?
		bool update_shadow_maps;//does the shadow map need update
		
		void __declspec(dllexport) build();//just initializes the data
		void __declspec(dllexport) calculate_max_radius();//calculate the max radius of contribution
		void __declspec(dllexport) operator=(light input);//copy the data to and other light
	};
	//end of light class
	class Renderer //renderer class
	{
	public:
		HDC hdc;								//device context

		HGLRC hrc;								//render condext

		Mesh *meshes;							//meshes

		camera *cameras;						//allow sveral cameras

		texture *textures;						//stack of textures and 

		light lights[100];						//no more than 12 ligts at a scene will be rendered

		unsigned long int current_camera_nums;	//number of current used camera
		unsigned long int cameras_nums;			//number of cameras
		unsigned long int mesh_nums;			//number of meshs
		unsigned long int vert_nums;			//total number of verts
		unsigned long int total_size;			//total size of the verts data
		unsigned long int rendere_ID;			//will be initialized in init
		unsigned long int texture_nums;         //number of textures
		unsigned long int last_light_ID;        //last light id

		unsigned long int *mesh_draw_order;		//the order in which meshs are drawn
		unsigned long int *current_cameras;		//current used cameras

		GLuint Shaders[100];					//max 100 shaders 20 is rendered in the scene

		void __declspec(dllexport) add_current_camera(unsigned long int camera_ID);	//add a current camera
		void __declspec(dllexport) delete_current_camera(unsigned long int camera_ID);	//delete a current camera
		
		unsigned long int __declspec(dllexport) add_camera();	//add a camera
		unsigned long int __declspec(dllexport) add_texture(unsigned char *irgba, unsigned int w, unsigned int h, unsigned short int NOC);	//add a texture
		unsigned long int __declspec(dllexport) add_mesh(char* path);	//add a mesh 

		void __declspec(dllexport) init();		//initialize function
		void __declspec(dllexport) render();	//render the scene using current cameras



		/******************************************************************/
		/**************************test functions**************************/
		/******************************************************************/
		/******************************************************************/
		void __declspec(dllexport) test_render();
		/******************************************************************/
		/******************************************************************/
		/**************************test functions**************************/
		/******************************************************************/

		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		/******************************************************************/
		HANDLE myConsoleHandle;
		DWORD cCharsWritten;
		/******************************************************************/
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
	};
	//end of renderer class
	//end of renderer libs
}
#endif