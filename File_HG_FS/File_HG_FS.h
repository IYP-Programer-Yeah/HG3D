#ifndef _file_HG_fs_h
#define _file_HG_fs_h
#include <math.h>
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
			path = (char*)malloc(0);//allocate 0 so it can be freeable
		}
		~File()
		{
			free(data);//free the memmory
			free(path);//free the memmory
		}
		bool compressed;//is file compressed?
		char* path;//path
		unsigned long int size;
		char *data;
		void __declspec(dllexport) MND(unsigned long int x, unsigned long int l);//mske new data at x with length of l
		void __declspec(dllexport) DSD(unsigned long int x, unsigned long int l);//delete some data at x with length of l
		void __declspec(dllexport) new_file(const char *ipath, unsigned long int isize = 0);//make new flw
		void __declspec(dllexport) open(const char *ipath);//open a file from path
		void __declspec(dllexport) save();//save the file on the current path
		void __declspec(dllexport) save_as(const char *ipath);//save as the file on new path
		void __declspec(dllexport) code(unsigned long int code);//encrypt the file
		void __declspec(dllexport) decode(unsigned long int code);//decrypt the file
		void __declspec(dllexport) *RLFX(unsigned long int x, unsigned long int l);//read l data from x
		void __declspec(dllexport) CLFX(unsigned long int x, unsigned long int l, void *idata);//copy l data to file from x
		void __declspec(dllexport) compress();//compress the file
		void __declspec(dllexport) decompress();//decompress the file
	};
}
#endif
