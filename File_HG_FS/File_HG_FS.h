#ifndef _file_HG_fs_h
#define _file_HG_fs_h
#include <math.h>
#include "..\Shared Headers\hstring.h"
#include <stdio.h>
namespace File_HG_FS
{
	class File
	{
		FILE *mainfile;
	public:
		File()
		{
			compressed = 0;//not yet compressed
			data = (char*)malloc(0);//allocate 0 so it can be freeable
		}
		~File()
		{
			free(data);//free the memmory
		}
		bool compressed;//is file compressed?
		string path;//path
		UINT64 size;
		char *data;
		void __declspec(dllexport) MND(UINT64 x, UINT64 l);//mske new data at x with length of l
		void __declspec(dllexport) DSD(UINT64 x, UINT64 l);//delete some data at x with length of l
		void __declspec(dllexport) new_file(const char *ipath, UINT64 isize = 0);//make new flw
		void __declspec(dllexport) open(const char *ipath);//open a file from path
		void __declspec(dllexport) save();//save the file on the current path
		void __declspec(dllexport) save_as(const char *ipath);//save as the file on new path
		void __declspec(dllexport) code(UINT64 code);//encrypt the file
		void __declspec(dllexport) decode(UINT64 code);//decrypt the file
		void __declspec(dllexport) *RLFX(UINT64 x, UINT64 l);//read l data from x
		void __declspec(dllexport) CLFX(UINT64 x, UINT64 l, void *idata);//copy l data to file from x
		void __declspec(dllexport) compress();//compress the file
		void __declspec(dllexport) decompress();//decompress the file
	};
}
#endif
