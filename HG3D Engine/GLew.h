#include <GL/glew.h>
//              glew func deffs

//      glew definitions
PFNGLGENFRAMEBUFFERSPROC                     glGenFramebuffers;
PFNGLDELETEFRAMEBUFFERSPROC                  glDeleteFramebuffers;
PFNGLBINDFRAMEBUFFERPROC                     glBindFramebuffer;
PFNGLCHECKFRAMEBUFFERSTATUSPROC              glCheckFramebufferStatus;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
PFNGLGENERATEMIPMAPPROC                      glGenerateMipmap;
PFNGLFRAMEBUFFERTEXTURE2DPROC                glFramebufferTexture2D;
PFNGLFRAMEBUFFERRENDERBUFFERPROC             glFramebufferRenderbuffer;
PFNGLGENRENDERBUFFERSPROC                    glGenRenderbuffers;
PFNGLDELETERENDERBUFFERSPROC                 glDeleteRenderbuffers;
PFNGLBINDRENDERBUFFERPROC                    glBindRenderbuffer;
PFNGLRENDERBUFFERSTORAGEPROC                 glRenderbufferStorage;
PFNGLGETRENDERBUFFERPARAMETERIVPROC          glGetRenderbufferParameteriv;
PFNGLISRENDERBUFFERPROC                      glIsRenderbuffer;
PFNGLCREATESHADERPROC                        glCreateShader;
PFNGLSHADERSOURCEPROC                        glShaderSource;
PFNGLCOMPILESHADERPROC                       glCompileShader;
PFNGLGETSHADERIVPROC                         glGetShaderiv;
PFNGLATTACHSHADERPROC                        glAttachShader;
PFNGLLINKPROGRAMPROC                         glLinkProgram;
PFNGLGETPROGRAMIVPROC                        glGetProgramiv;
PFNGLDELETESHADERPROC                        glDeleteShader;
PFNGLCREATEPROGRAMPROC                       glCreateProgram;
PFNGLUSEPROGRAMPROC                          glUseProgram;
PFNGLGETUNIFORMLOCATIONPROC                  glGetUniformLocation;
PFNGLUNIFORM1IPROC                           glUniform1i;
PFNGLUNIFORM1UIPROC                          glUniform1ui;
PFNGLUNIFORM1IVPROC                          glUniform1iv;
PFNGLUNIFORM1FPROC                           glUniform1f;
PFNGLUNIFORM2FPROC                           glUniform2f;
PFNGLUNIFORM1FVPROC                          glUniform1fv;
PFNGLUNIFORM3FVPROC                          glUniform3fv;
PFNGLUNIFORM3FPROC                           glUniform3f;
PFNGLUNIFORMMATRIX4FVPROC                    glUniformMatrix4fv;
PFNGLACTIVETEXTUREPROC                       glActiveTexture;
PFNGLBINDSAMPLERPROC                         glBindSampler;
PFNGLTEXIMAGE3DPROC                          glTexImage3D;
PFNGLTEXSTORAGE3DPROC                        glTexStorage3D;
PFNGLFRAMEBUFFERTEXTURE3DPROC                glFramebufferTexture3D;
PFNGLFRAMEBUFFERTEXTURELAYERPROC             glFramebufferTextureLayer;
PFNGLFRAMEBUFFERTEXTUREPROC					 glFramebufferTexture;
PFNGLDELETEBUFFERSPROC                       glDeleteBuffers;
PFNGLGENBUFFERSPROC                          glGenBuffers;
PFNGLBINDBUFFERPROC                          glBindBuffer;
PFNGLBUFFERDATAPROC                          glBufferData;
PFNGLUNMAPBUFFERPROC                         glUnmapBuffer;
PFNGLMAPBUFFERPROC                           glMapBuffer;
PFNGLTEXIMAGE2DMULTISAMPLEPROC               glTexImage2DMultisample;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC      glRenderbufferStorageMultisample;
PFNGLBLITFRAMEBUFFERPROC                     glBlitFramebuffer;
PFNGLDRAWBUFFERSPROC                         glDrawBuffers;
PFNGLBINDVERTEXARRAYPROC                     glBindVertexArray;
PFNGLGENVERTEXARRAYSPROC                     glGenVertexArrays;
PFNGLDELETEVERTEXARRAYSPROC                  glDeleteVertexArrays;
PFNGLVERTEXATTRIBPOINTERPROC                 glVertexAttribPointer;
PFNGLVERTEXATTRIBIPOINTERPROC                glVertexAttribIPointer;
PFNGLBINDATTRIBLOCATIONPROC                  glBindAttribLocation;
PFNGLENABLEVERTEXATTRIBARRAYPROC             glEnableVertexAttribArray;
PFNGLBINDFRAGDATALOCATIONPROC                glBindFragDataLocation;
PFNGLGETSHADERINFOLOGPROC                    glGetShaderInfoLog;
PFNGLGETPROGRAMINFOLOGPROC                   glGetProgramInfoLog;
PFNGLCOMPRESSEDTEXIMAGE2DPROC				 glCompressedTexImage2D;
PFNGLGETUNIFORMBLOCKINDEXPROC				 glGetUniformBlockIndex;
PFNGLBINDBUFFERBASEPROC						 glBindBufferBase;
PFNGLUNIFORMBLOCKBINDINGPROC				 glUniformBlockBinding;
PFNGLGENERATETEXTUREMIPMAPPROC				 glGenerateTextureMipmap;
PFNGLBINDIMAGETEXTUREPROC					 glBindImageTexture;
//      glew definitions
//
void Init_glew()
{
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
	glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
	glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameteriv");
	glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbuffer");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform1ui = (PFNGLUNIFORM1UIPROC)wglGetProcAddress("glUniform1ui");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	glBindSampler = (PFNGLBINDSAMPLERPROC)wglGetProcAddress("glBindSampler");
	glTexImage3D = (PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");
	glTexStorage3D = (PFNGLTEXSTORAGE3DPROC)wglGetProcAddress("glTexStorage3D");
	glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)wglGetProcAddress("glFramebufferTexture3D");
	glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)wglGetProcAddress("glFramebufferTextureLayer");
	glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)wglGetProcAddress("glFramebufferTexture");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
	glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
	glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)wglGetProcAddress("glTexImage2DMultisample");
	glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)wglGetProcAddress("glRenderbufferStorageMultisample");
	glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)wglGetProcAddress("glBlitFramebuffer");
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)wglGetProcAddress("glVertexAttribIPointer");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)wglGetProcAddress("glBindFragDataLocation");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)wglGetProcAddress("glCompressedTexImage2D");
	glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)wglGetProcAddress("glGetUniformBlockIndex");
	glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)wglGetProcAddress("glBindBufferBase");
	glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)wglGetProcAddress("glUniformBlockBinding");
	glGenerateTextureMipmap = (PFNGLGENERATETEXTUREMIPMAPPROC)wglGetProcAddress("glGenerateTextureMipmap");
	glBindImageTexture = (PFNGLBINDIMAGETEXTUREPROC)wglGetProcAddress("glBindImageTexture");
}
// shader loader