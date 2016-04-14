#include <Windows.h>
#include <gl/glew.h>
#include <gl/gl.h> 
#include <gl/glu.h>
#include <math.h>
#include <xmmintrin.h>
#include <time.h>
#include <algorithm>

// load GL libs
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#ifndef HG3D_ENG  //check if included before
#define HG3D_ENG

#define Max_Facts					100									//max factorial input

#define Max_Band					10									//max ALP bands
#define Max_Calculated_Vlaues		10000								//max number of calculated values of the function

#ifndef PI
#define PI							3.1415926535897						//pi
#endif

#define Vert_Pos_Size				12									//size of positions
#define Vert_Normal_Size			3									//size of normals
#define Vert_Coord_Size				8									//size of coord
#define Vert_Bone_ID_Size			1									//size of coord
#define vertex_size					24									//vertex size
#define face_size					12									//face size

#define MaxLightNums				32									//maximum number of lights in the scene
#define MaxShadowmapsNums			8									//maximum number of the lights with shadow maps
#define MaxCascadessNums			1									//maximum number of cascades for each shadow map
#define MaxSoftShadowDepth			1									//maximum number of shadow maps for shadow mapping

#define MaxDepth 10e15f

#define Shadowmap_Res				4096								//shadow map resolution 

#define GBufferTextNums				3									//number of render targets in GBuffer

#define ShadingTechniquesNum		1

#define lights_UBO_binding_point				1				//the binding point of the lights
#define text_offsets_UBO_binding_point			2				//the binding point of the texture sampling offset
#define camera_data_UBO_binding_point			3				//the binding point of the camera data
#define mesh_data_UBO_binding_point				4				//the binding point of the mesh data

#define PCF//use PCF instead of VSMs
#define Deferred

#ifdef Deferred
//sampler for GBuffer textures
#define GBuffer0_Sampler			0			//normal map
#define GBuffer1_Sampler			1			//texure and other stuff
#define GBuffer2_Sampler			2			//material ID
#define GBuffer3_Sampler			3			//depth map
//samplers for prepass GBuffer generator
#define Diff_Tex_Sampler			0			//diffuse texture sampler
//samplers for bloom mix
#define High_Pass_Texture_Sampler	0			//high pass texture sampler
//
#define Bloom_Texture_Sampler		0			//bloom texture sampler
#define Normal_Texture_Sampler		1			//normal texture sampler
//samplers for AA
#define Aliased_Image_Sampler		0			//aiased image sampler
#define Edge_Texture_Sampler		1			//detected edges
#endif


namespace HG3D_Engine
{
	template <class TypeA, class TypeB> struct Couple
	{
		TypeA A;
		TypeB B;

		bool __declspec(dllexport) __fastcall operator > (Couple<TypeA, TypeB> inp);
		bool __declspec(dllexport) __fastcall operator < (Couple<TypeA, TypeB> inp);
		bool __declspec(dllexport) __fastcall operator == (Couple<TypeA, TypeB> inp);
		void __declspec(dllexport) __fastcall operator = (Couple<TypeA, TypeB> inp);
	};

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
	//end of matrix class
	_4x4matrix LookAt(point cam_pos,vector forward,vector up);//get view mat

	_4x4matrix Projection(float left, float right, float buttom, float top, float nearz, float farz);//projection matrix

	_4x4matrix __declspec(dllexport) __fastcall Inverse(_4x4matrix input);//invers mat

	_4x4matrix __declspec(dllexport) __fastcall Transpose(_4x4matrix input);//invers mat
	//factorial class
	class factorial
	{
	public:
		float *data;
		unsigned int Last_Fact;

		__declspec(dllexport) factorial();
		__declspec(dllexport) ~factorial();
		float __declspec(dllexport) get(unsigned int N);
	};
	//end of factorial class
	float __declspec(dllexport) fact(unsigned int N);
	//start of associated legendre polynomials class
	class ALP
	{
	public:
		unsigned long int last_L;//the last calculated function's band
		float *Functions; //the values
		float *XVlues;//the values for x

		__declspec(dllexport) ALP();
		__declspec(dllexport) ~ALP();

		void _declspec(dllexport) Calculate_Function(unsigned long int L, unsigned long int M, unsigned long int X);//calculate the function to the point givven
		unsigned long int _declspec(dllexport) Calculate_Coords(unsigned long int L, unsigned long int M, unsigned long int X);//calculate the exact point on the pointer
		float _declspec(dllexport) Get_Function(unsigned long int L, unsigned long int M, unsigned long int X);//get the function value

	};
	//end of associated legendre polynomials class
	double _declspec(dllexport) calcALP(int l, int m, double x);
	//start of SH class
	class SH
	{
	public:
		unsigned long int last_L;//the last calculated function's band
		float *Functions; //the values
		float *thetaVlues;//the values for theta
		float *philues;//the values for phi

		__declspec(dllexport) SH();
		__declspec(dllexport) ~SH();

		void _declspec(dllexport) Calculate_Function(unsigned long int L, unsigned long int M, unsigned long int theta, unsigned long int phi);//calculate the function to the point givven
		unsigned long int _declspec(dllexport) Calculate_Coords(unsigned long int L, unsigned long int M, unsigned long int theta, unsigned long int phi);//calculate the exact point on the pointer
		float _declspec(dllexport) Get_Function(unsigned long int L, unsigned long int M, unsigned long int theta, unsigned long int phi);//get the function value
	};

	double _declspec(dllexport) calcSH(int l, int m, double theta, double phi);

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
		float x, y, z;				//positions
		INT8 nx, ny, nz;			//normals
		UINT8 bone_ID;				//bone ID
		float cx, cy;				//coords
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
		float TF[3];//transmission filter color
		float dissolve;//transparancy 
		float Fresnel;//Fresnel of the material
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

		unsigned long int *indices;       //vert indices

		unsigned long int text_ID_diff;               //diffuse texture ID
		unsigned long int text_ID_roughness;          //roughness texture ID
		unsigned long int text_ID_normal;             //normal map texure ID
		unsigned long int text_ID_height;             //height map texture ID
		unsigned long int text_ID_mask;               //mask texture ID
		unsigned long int text_ID_alphamap;			  //apha map texture ID
		unsigned long int text_ID_dispmap;			  //displacement map texture ID
		unsigned long int text_ID_TFmap;			  //transmision filter map texture ID

		unsigned char MeshID[4];          //mesh id for defered shading

		bool have_diff_text;              //have diffuse texture? 
		bool have_roughness_text;         //have roughness texture? 
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
	};
	struct MeshData
	{
		_4x4matrix model_matrix;          //model matrix will be sent to shader
		_4x4matrix normal_matrix;
		unsigned int MatID;
		unsigned int have_diff_text;              //have diffuse texture? 
		unsigned int have_roughness_text;         //have roughness texture? 
		unsigned int have_NM_text;                //have normals map texture? 
		unsigned int have_HM_text;                //have height map texture? 
		unsigned int have_mask_text;              //have mask texture? 
		unsigned int have_alphamap_text;		  //have alpha map texture?
		unsigned int have_dispmap_text;			  //have displacement map texture?
		unsigned int have_TFmap_text;			  //have transmision filter map texture?
		unsigned int Padding[23];
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

#ifdef Deferred
		GLint CurrentTexWidth;//the current text width fir the GBuffer
		GLint CurrentTexHeight;//the current tex height for the GBuffer

		GLuint GBuffer_Depth;

		GLuint GBuffer_FBO_ID;
		GLuint Gather_FBO_ID;
		GLuint Temp_FBO_ID;
		GLuint Edge_FBO_ID;
		GLuint HDRScreen_FBO_ID;

		GLuint GBufferID[GBufferTextNums];//GBuffer for each camera
		GLuint GatherTexID;
		GLuint TempTexID;
		GLuint EdgeTexID;
		GLuint HDRScreenTexID;

		GLuint ScreenDepthRBO;
#endif
#ifdef DeferredSM
		GLuint DeferredShadowText[MaxShadowmapsNums];
		GLuint DeferredFilteredShadowText;
		GLuint DeferredShadow_RBO_ID;
		GLuint DeferredShadow_FBO_ID;
#endif
		void __declspec(dllexport) init();//initialize the camera
		void __declspec(dllexport) update_camera();//update camera
		void __declspec(dllexport) fps_camera(float pitch,float yaw,vector head_up);//pith and yaw the camera in fps mode
	};
	struct CameraData
	{
		_4x4matrix ViewMatrix;//modelview matrix
		_4x4matrix ProjectionMatrix;//projection
		float camera_viewport[4];//view port property

		float Left, Right, Buttom, Top, Near, Far;//projection property
		float x, y, z;
		float data[19];
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
		bool compressed;//is the data compressed

		unsigned short int number_of_components;// number of components

		unsigned char *rgba;//the rgba better exist for later change

		unsigned int width, height;//width and height

		unsigned long int total_size;
		unsigned long int compression_method;//the compresion method
		
		void __declspec(dllexport) build(unsigned char *irgba, unsigned int w, unsigned int h, unsigned short int NOC, bool icompressed, unsigned long int icompression_method, unsigned long int idata_size);//will copy the irgba to rgb free your own allocation
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

		float max_radius;//the maximum radius of light contribution

		float light_color[3];//light color

		float cut_off_cos;//cosine of the the the cut of angle 

		float Attenuation[3];//attenuation values

		float edge_cut_off_cos_delta;

		float direction[3];//the direction of light

		bool light_enabled;//is the light enabled
		bool update_shadow_maps;//does the shadow map need update
		bool directional;//light is diretional
		bool shadow_map;//does it use shadow maps?

		

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

		light lights[100];						//no more than 32 ligts at a scene will be rendered and 8 will be shadow mapped

		unsigned long int current_camera_nums;		//number of current used camera
		unsigned long int cameras_nums;				//number of cameras
		unsigned long int mesh_nums;				//number of meshs
		unsigned long int vert_nums;				//total number of verts
		unsigned long int total_size;				//total size of the verts data
		unsigned long int renderer_ID;				//will be initialized in init
		unsigned long int texture_nums;				//number of textures
		unsigned long int last_light_ID;			//last light id

		unsigned long int *current_cameras;			//current used cameras

		bool light_data_changed;					//lights need to be updated

		GLuint ScreenQuadVAO;					//screen mesh
		GLuint ScreenQuadVBO;					//screen mesh


		GLuint text_offset_UBO_ID;				//the id of texture sampling offset 
		GLuint light_data_UBO_ID;				//the id of light data 
		GLuint mat_data_UBO_ID;					//the id of material data
		GLuint camera_data_UBO_ID;				//the id of the camera data
		GLuint mesh_data_UBO_ID;				//the id of the mesh data
		GLuint Gbuffer_textIDS;                 //id of Gbuffer textures
		GLuint Gbuffer_FBO_ID;					//id of the Gbuffer textures
		GLuint Shadowmap_FBO_ID;				//id of shadowmap fbo
		GLuint Shadowmap_RBO_ID;				//id of shadowmap rbo


		GLuint Shaders[50];											//max 100 shaders 20 is rendered in the scene
		GLuint Text_Offset_Block_Index[50];							//the index of light block in shader
		GLuint Light_Block_Index[50];								//the index of texture sampling offset block in shader
		GLuint Camera_Data_Block_Index[50];							//the index of camera data block in shader
		GLuint Mesh_Data_Block_Index[50];							//the index of mesh data block in shader
		GLuint Model_Matrix_Location[50];							//the loaction of model matrix in shader
		GLuint Normal_Matrix_Location[50];							//the loaction of normal matrix in shader
		GLuint Projection_Matrix_Location[50];						//the loaction of projection matrix in shader
		GLuint View_Matrix_Location[50];							//the loaction of view matrix in shader
		GLuint Camera_Position_Location[50];						//the loaction of camera position in shader
		GLuint Lights_Nums_Location[50];							//the loaction of Lights Nums in shader
		GLuint Lights_Proj_View_Matrix_Location[50];				//the loaction of Lights view matrix in shader
		GLuint Inv_Lights_Proj_View_Matrix_Location[50];			//the loaction of Lights view matrix in shader
		GLuint Shadowmap_Sampler_Location[50];						//the loaction of Lights view matrix in shader
		GLuint CSM_Data_Location[50];								//the location of Ext's
		GLuint GBuffer_Normal_Map_Sampler_Location[50];				//the location of the material ID in shader
		GLuint GBuffer_Sampler_loaction[50];						//the location of the GBuffer sampler in shader
		GLuint GBuffer_Mat_ID_Sampler_loaction[50];					//the location of the GBuffer sampler in shader
		GLuint GBuffer_Depth_Buffer_Sampler_loaction[50];			//the location of the GBuffer sampler in shader
		GLuint Current_Light_Location[50];							//the location of the index of the light that the process is being done on
		GLuint Diffuse_Texture_Sampler_Location[50];				//the location of the GBuffer sampler in shader
		GLuint Texture_Sampler_Location[50];						//the location of the texture sampler in shader
		GLuint Edge_Texture_Sampler_Location[50];					//the location of the edge texture sampler in shader
		GLuint Aliased_Image_Texture_Sampler_Location[50];			//the location of the aliased image texture sampler in shader
		GLuint Blur_Pass_Location[50];								//the location of the blur pass in shader
#ifdef Deferred
		Couple<unsigned long int, long double> *mesh_draw_order;	//the order in which meshs are drawn
#endif
		GLuint Shadow_Maps_Tex_ID[MaxCascadessNums];		//the shadow map textures an array of cascades



		void __declspec(dllexport) add_current_camera(unsigned long int camera_ID);	//add a current camera
		void __declspec(dllexport) delete_current_camera(unsigned long int camera_ID);	//delete a current camera
		
		unsigned long int __declspec(dllexport) add_camera();	//add a camera
		unsigned long int __declspec(dllexport) add_texture(unsigned char *irgba, unsigned int w, unsigned int h, unsigned short int NOC, bool icompressed, unsigned long int icompression_method, unsigned long int idata_size);	//add a texture
		unsigned long int __declspec(dllexport) add_mesh(char* path);	//add a mesh 

		void __declspec(dllexport) init();		//initialize function
		void __declspec(dllexport) render();	//render the scene using current cameras



		/******************************************************************/
		/**************************test functions**************************/
		/******************************************************************/
		/******************************************************************/
		void __declspec(dllexport) test_render();
		void __declspec(dllexport) test_init();
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