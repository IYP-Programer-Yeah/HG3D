#include <allocators>
#include <iostream>

#include "File_HG_Fs.h"
namespace File_HG_FS
{
	struct S_O_S
	{
		char symble;
		unsigned long int S_occurrence, S_start;
	};
	void File::MND(unsigned long int x, unsigned long int l)//mske new data at x with length of l
	{
		if (x >= size&&!size == 0)//is input logical?
			x = size - 1;//reset the input to an acceptable input
		size = size + l;//calculate new size
		char *temp;
		temp = (char*)malloc(size);//allocate new data
		for (register unsigned long int i = 0; i<size; i++)
			if (i >= x)
				temp[i] = (i >= l + x) ? data[i - l] : 0;//new data set to 0
			else
				temp[i] = data[i];//copy the data
		free(data);//free last data replace with new
		data = temp;
	}
	void File::DSD(unsigned long int x, unsigned long int l)//delete some data at x with length of l
	{
		if (size == 0)//check if there is sometihng to delete
			return;
		if (x >= size)//is input logical
			x = size - 1;//reset the input to an acceptable intup
		if (l>x + 1)//is input logical
			l = x + 1;//reset the input to an acceptable intup
		size = size - l;
		char *temp;
		temp = (char*)malloc(size);//allocate the new data
		for (register unsigned long int i = 0; i<size; i++)
			if (i >= x + 1 - l)
				temp[i] = data[i + l];//replace the data
			else
				temp[i] = data[i];//the data stays the same
		free(data);//free the last data
		data = temp;//replace with new one
	}
	void File::new_file(const char *ipath, unsigned long int isize)//open a file from path
	{
		path = ipath;
		size = isize;
		data = (char*)malloc(size);//allocate memory
	}
	void File::open(const char *ipath)//save the file on the current path
	{
		size = 0;
		path = ipath;
		fopen_s(&mainfile, ipath, "rb");//open the file for read
		if (!mainfile)//if the file exists continue
			return;
		fseek(mainfile, 0, SEEK_END);
		size = ftell(mainfile);
		fclose(mainfile);//cloase the file
		data = (char*)malloc(size + 1);//allocate the memory
		data[size] = 0;//the end of file indicator
		fopen_s(&mainfile, ipath, "rb");//reopen the file for read
		fread(data, 1, size, mainfile);//read the whole file
		fclose(mainfile);//close the file
	}
	void File::save()//save the file
	{
		fopen_s(&mainfile, path.string1, "wb");//open the file for write
		fwrite(data, 1, size, mainfile);
		fclose(mainfile);//cloase the file
	}
	void File::save_as(const char *ipath)//save as the file on new path
	{
		path = ipath;//current path will be replaced
		fopen_s(&mainfile, path.string1, "wb");//open the file for write
		fwrite(data, 1, size, mainfile);//write data
		fclose(mainfile);//cloase the file
	}
	void File::code(unsigned long int code)//this is coding no comments
	{
		unsigned long int caltest;
		bool doloop = 0;
		unsigned int *fileparts1;
		//
		unsigned long int *dfileparts1;
		///
		unsigned short int *fileparts2;
		//
		unsigned int *dfileparts2;
		///
		unsigned char remainings[4];
		if (code>255)
		{
			////////////////////////////////////////////////////////////////////////////////
			if (code<65536)
			{
				short int sousi = sizeof(unsigned short int);
				short int msousi = size%sousi;
				for (register short int i = 0; i<msousi; i++)
				{
					remainings[i] = data[size - 1];
					DSD(size - 1, 1);
				}
				caltest = size / sousi;
				dfileparts2 = (unsigned int*)malloc(caltest*sizeof(unsigned int));
				fileparts2 = (unsigned short int *)data;
				for (register unsigned long int i = 0; i<caltest; i++)
					dfileparts2[i] = code - fileparts2[i] - (((((((i % 8 + 1) % 7 + 2) % 6 + 3) % 5 + 4) % 4 + 5) % 3 + 6) % 2);
				new_file(path.string1, caltest*sizeof(unsigned int));
				free(data);
				data = (char*)dfileparts2;
				for (register short int i = 0; i<msousi; i++)
				{
					MND(size, 1);
					data[size - 1] = remainings[i];
				}
				free(fileparts2);
				return;
			}
			else
			{
				short int sousi = sizeof(unsigned int);
				short int msousi = size%sousi;
				for (register short int i = 0; i<msousi; i++)
				{
					remainings[i] = data[size - 1];
					DSD(size - 1, 1);
				}
				caltest = size / sousi;
				dfileparts1 = (unsigned long int*)malloc(caltest*sizeof(unsigned long int));
				fileparts1 = (unsigned int *)data;
				for (register unsigned long int i = 0; i<caltest; i++)
					dfileparts1[i] = code - fileparts1[i] - (((((((((i % 10 + 1) % 9 + 2) % 8 + 3) % 7 + 4) % 6 + 5) % 5 + 6) % 4 + 7) % 3 + 8) % 2);
				new_file(path.string1, caltest*sizeof(unsigned long int));
				free(data);
				data = (char*)dfileparts1;
				for (register short int i = 0; i<msousi; i++)
				{
					MND(size, 1);
					data[size - 1] = remainings[i];
				}
				free(fileparts1);
				return;
			}
		}
		return;
	}
	void File::decode(unsigned long int code)//encoding still no comments
	{
		unsigned long int caltest;
		bool doloop = 0;
		unsigned long int *fileparts1;
		//
		unsigned int *dfileparts1;
		///
		unsigned int *fileparts2;
		//
		unsigned short int *dfileparts2;
		///
		unsigned char remainings[4];
		if (code>255)
		{
			////////////////////////////////////////////////////////////////////////////////
			if (code<65536)
			{
				short int sousi = sizeof(unsigned int);
				short int msousi = size%sousi;
				for (register short int i = 0; i<msousi; i++)
				{
					remainings[i] = data[size - 1];
					DSD(size - 1, 1);
				}
				caltest = size / sousi;
				dfileparts2 = (unsigned short int*)malloc(caltest*sizeof(unsigned short int));
				fileparts2 = (unsigned int *)data;
				for (register unsigned long int i = 0; i<caltest; i++)
					dfileparts2[i] = (unsigned short)(code - fileparts2[i] - (((((((i % 8 + 1) % 7 + 2) % 6 + 3) % 5 + 4) % 4 + 5) % 3 + 6) % 2));
				new_file(path.string1, caltest*sizeof(unsigned short int));
				free(data);
				data = (char*)dfileparts2;
				for (register short int i = 0; i<msousi; i++)
				{
					MND(size, 1);
					data[size - 1] = remainings[i];
				}
				free(fileparts2);
				return;
			}
			else
			{
				short int sousi = sizeof(unsigned long int);
				short int msousi = size%sousi;
				for (register short int i = 0; i<msousi; i++)
				{
					remainings[i] = data[size - 1];
					DSD(size - 1, 1);
				}
				caltest = size / sousi;
				dfileparts1 = (unsigned int*)malloc(caltest*sizeof(unsigned int));
				fileparts1 = (unsigned long int *)data;
				for (register unsigned long int i = 0; i<caltest; i++)
					dfileparts1[i] = code - fileparts1[i] - (((((((((i % 10 + 1) % 9 + 2) % 8 + 3) % 7 + 4) % 6 + 5) % 5 + 6) % 4 + 7) % 3 + 8) % 2);
				new_file(path.string1, caltest*sizeof(unsigned int));
				free(data);
				data = (char*)dfileparts1;
				for (register short int i = 0; i<msousi; i++)
				{
					MND(size, 1);
					data[size - 1] = remainings[i];
				}
				free(fileparts1);
				return;
			}
		}
		return;
	}
	void *File::RLFX(unsigned long int x, unsigned long int l)//read l data from x
	{
		if (x + l>size)//check input see if logical
			l = size - x;//make it logical
		char *temp;
		temp = (char*)malloc(l);//allocate output
		for (register unsigned long int i = x; i<x + l; i++)
			temp[i - x] = data[i];//copy data to output
		return (void*)temp;
	}
	void File::CLFX(unsigned long int x, unsigned long int l, void *idata)//copy l data to file from x
	{
		if (x + l>size)//check input see if logical
			l = size - x;//make it logical
		char *temp;
		temp = (char*)idata;
		for (register unsigned long int i = x; i<x + l; i++)
			data[i] = temp[i - x];//replace data with input
	}
	void File::compress()//compresing no comments
	{
		S_O_S *sos;
		sos = (S_O_S*)malloc(9 * size);
		unsigned long int  sos_T_N = 0, I_temp = 0;
		File backup;
		backup.new_file("", size);
		for (register unsigned long int i = 0; i<size; i++)
			backup.data[i] = data[i];
		unsigned long int C_times = 0;
		bool changed = 1;
		while (changed)
		{
			for (register unsigned long int i = 0; i<size; i++)
			{
				sos[sos_T_N].symble = data[i];
				sos[sos_T_N].S_start = i;
				for (register unsigned long int j = i; j<size; j++)
				{
					if (sos[sos_T_N].symble == data[j])
					{
						sos[sos_T_N].S_occurrence = j - i + 1;
						if (j == size - 1)
						{
							i = j;
							sos_T_N++;
						}
					}
					else
					{
						sos_T_N++;
						i = j - 1;
						break;
					}
				}
			}
			for (register unsigned long int i = 0; i<sos_T_N; i++)
				if (sos[I_temp].S_occurrence<sos[i].S_occurrence)
					I_temp = i;
			if (sos[I_temp].S_occurrence>9)
			{
				DSD(sos[I_temp].S_start + sos[I_temp].S_occurrence - 1, sos[I_temp].S_occurrence);
				MND(0, 1);
				CLFX(0, 1, &sos[I_temp].symble);
				MND(0, 4);
				CLFX(0, 4, &sos[I_temp].S_occurrence);
				MND(0, 4);
				CLFX(0, 4, &sos[I_temp].S_start);
				C_times++;
				sos_T_N = 0;
				I_temp = 0;
			}
			else
			{
				if (C_times)
				{
					compressed = 1;
					MND(0, 4);
					CLFX(0, 4, &C_times);
				}
				changed = 0;
				break;
			}
		}
		free(sos);
	}
	void File::decompress()//decomprssing still no comments
	{
		S_O_S sos;
		unsigned long int C_times;
		C_times = (unsigned char)(data[3]) * 16777216 + (unsigned char)(data[2]) * 65536 + (unsigned char)(data[1]) * 256 + (unsigned char)(data[0]);
		DSD(3, 4);
		for (register unsigned long int i = 0; i<C_times; i++)
		{
			sos.S_start = (unsigned char)(data[3]) * 16777216 + (unsigned char)(data[2]) * 65536 + (unsigned char)(data[1]) * 256 + (unsigned char)(data[0]);
			DSD(3, 4);
			sos.S_occurrence = (unsigned char)(data[3]) * 16777216 + (unsigned char)(data[2]) * 65536 + (unsigned char)(data[1]) * 256 + (unsigned char)(data[0]);
			DSD(3, 4);
			sos.symble = data[0];
			DSD(0, 1);
			char *C_temp;
			C_temp = (char*)malloc(sos.S_occurrence);
			for (register unsigned long int j = 0; j<sos.S_occurrence; j++)
				C_temp[j] = sos.symble;
			MND(sos.S_start, sos.S_occurrence);
			CLFX(sos.S_start, sos.S_occurrence, C_temp);
			free(C_temp);
		}
	}
}