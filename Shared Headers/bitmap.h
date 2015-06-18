#ifndef _bitmap_h
#define _bitmap_h
#include <stdio.h>
#include <Windows.h>
#define bmpnums 100
bool nobetb(int f, int s, int no)
{
	if (f>s)
	{
		if (no>s&&no<f)
			return 1;
		return 0;
	}
	else if (s>f)
	{
		if (no>f&&no<s)
			return 1;
		return 0;
	}
	else if (no == f || no == s)
		return 1;
	return 0;
}
class bitmaprgb
{
public:
	BYTE  b;
	BYTE  g;
	BYTE  r;
};
class hbitmap
{
public:
	long int  FileType[1];
	long int  FileSize[1];
	long int  Reserved1[1];
	long int  Reserved2[1];
	long int  BitmapOffset[1];
	long int  Size[1];
	long int  Width[1];
	long int  Height[1];
	long int  Planes[1];
	long int  BitsPerPixel[1];
	long int  Compression[1];
	long int  SizeOfBitmap[1];
	long int  HorzResolution[1];
	long int  VertResolution[1];
	long int  ColorsUsed[1];
	long int  ColorsImportant[1];
	long int  RedMask[1];
	long int  GreenMask[1];
	long int  BlueMask[1];
	long int  AlphaMask[1];
	long int  CSType[1];
	long int  RedX[1];
	long int  RedY[1];
	long int  RedZ[1];
	long int  GreenX[1];
	long int  GreenY[1];
	long int  GreenZ[1];
	long int  BlueX[1];
	long int  BlueY[1];
	long int  BlueZ[1];
	long int  GammaRed[1];
	long int  GammaGreen[1];
	long int  GammaBlue[1];
	bitmaprgb *rgb;
	string    bitmap;
	FILE*     BITMAP;
	void getimage(char *path)
	{
		bitmap = path;
		fopen_s(&BITMAP, bitmap.string1, "rb");
		fread(FileType, 2, 1, BITMAP);
		fread(FileSize, 4, 1, BITMAP);
		fread(Reserved1, 2, 1, BITMAP);
		fread(Reserved2, 2, 1, BITMAP);
		fread(BitmapOffset, 4, 1, BITMAP);
		fread(Size, 4, 1, BITMAP);
		fread(Width, 4, 1, BITMAP);
		fread(Height, 4, 1, BITMAP);
		fread(Planes, 2, 1, BITMAP);
		fread(BitsPerPixel, 2, 1, BITMAP);
		fread(Compression, 4, 1, BITMAP);
		fread(SizeOfBitmap, 4, 1, BITMAP);
		fread(HorzResolution, 4, 1, BITMAP);
		fread(VertResolution, 4, 1, BITMAP);
		fread(ColorsUsed, 4, 1, BITMAP);
		fread(ColorsImportant, 4, 1, BITMAP);
		if (Size[0] == 108)
		{
			fread(RedMask, 4, 1, BITMAP);
			fread(GreenMask, 4, 1, BITMAP);
			fread(BlueMask, 4, 1, BITMAP);
			fread(AlphaMask, 4, 1, BITMAP);
			fread(CSType, 4, 1, BITMAP);
			fread(RedX, 8, 1, BITMAP);
			fread(RedY, 8, 1, BITMAP);
			fread(RedZ, 8, 1, BITMAP);
			fread(GreenX, 8, 1, BITMAP);
			fread(GreenY, 8, 1, BITMAP);
			fread(GreenZ, 8, 1, BITMAP);
			fread(BlueX, 8, 1, BITMAP);
			fread(BlueY, 8, 1, BITMAP);
			fread(BlueZ, 8, 1, BITMAP);
			fread(GammaRed, 4, 1, BITMAP);
			fread(GammaGreen, 4, 1, BITMAP);
			fread(GammaBlue, 4, 1, BITMAP);
		}
		if (BitsPerPixel[0]>15)
			ColorsUsed[0] = 1 << BitsPerPixel[0];
		fseek(BITMAP, BitmapOffset[0], SEEK_SET);
		rgb = (bitmaprgb*)malloc(Height[0] * Width[0] * 3);
		bitmaprgb temp1[1];
		for (register int i = 0; i<Height[0] * Width[0]; i++)
		{
			fread(temp1, 3, 1, BITMAP);
			if ((i + 1) % Width[0] == 0)
				fseek(BITMAP, (4 - (Width[0] * 3) % 4) % 4, SEEK_CUR);
			rgb[i] = temp1[0];
		}
		fclose(BITMAP);
	}
	void savebmp(char *path)
	{
		bitmap = path;
		fopen_s(&BITMAP, bitmap.string1, "wb");
		fwrite(FileType, 2, 1, BITMAP);
		fwrite(FileSize, 4, 1, BITMAP);
		fwrite(Reserved1, 2, 1, BITMAP);
		fwrite(Reserved2, 2, 1, BITMAP);
		fwrite(BitmapOffset, 4, 1, BITMAP);
		fwrite(Size, 4, 1, BITMAP);
		fwrite(Width, 4, 1, BITMAP);
		fwrite(Height, 4, 1, BITMAP);
		fwrite(Planes, 2, 1, BITMAP);
		fwrite(BitsPerPixel, 2, 1, BITMAP);
		fwrite(Compression, 4, 1, BITMAP);
		fwrite(SizeOfBitmap, 4, 1, BITMAP);
		fwrite(HorzResolution, 4, 1, BITMAP);
		fwrite(VertResolution, 4, 1, BITMAP);
		fwrite(ColorsUsed, 4, 1, BITMAP);
		fwrite(ColorsImportant, 4, 1, BITMAP);
		if (Size[0] == 108)
		{
			fwrite(RedMask, 4, 1, BITMAP);
			fwrite(GreenMask, 4, 1, BITMAP);
			fwrite(BlueMask, 4, 1, BITMAP);
			fwrite(AlphaMask, 4, 1, BITMAP);
			fwrite(CSType, 4, 1, BITMAP);
			fwrite(RedX, 8, 1, BITMAP);
			fwrite(RedY, 8, 1, BITMAP);
			fwrite(RedZ, 8, 1, BITMAP);
			fwrite(GreenX, 8, 1, BITMAP);
			fwrite(GreenY, 8, 1, BITMAP);
			fwrite(GreenZ, 8, 1, BITMAP);
			fwrite(BlueX, 8, 1, BITMAP);
			fwrite(BlueY, 8, 1, BITMAP);
			fwrite(BlueZ, 8, 1, BITMAP);
			fwrite(GammaRed, 4, 1, BITMAP);
			fwrite(GammaGreen, 4, 1, BITMAP);
			fwrite(GammaBlue, 4, 1, BITMAP);
		}
		fseek(BITMAP, BitmapOffset[0], SEEK_SET);
		bitmaprgb temp1[1];
		for (register int i = 0; i<Height[0] * Width[0]; i++)
		{
			temp1[0] = rgb[i];
			fwrite(temp1, 3, 1, BITMAP);
			if ((i + 1) % Width[0] == 0)
				fseek(BITMAP, (4 - (Width[0] * 3) % 4) % 4, SEEK_CUR);
		}
		fclose(BITMAP);
	}
	void filter(BYTE r, BYTE g, BYTE b, BYTE r1, BYTE r2, BYTE g1, BYTE g2, BYTE b1, BYTE b2, bool in)
	{
		for (register int i = Height[0] * Width[0] - 1; i >= 0; --i)
		{
			if (!(nobetb(r1, r2, rgb[i].r) && nobetb(g1, g2, rgb[i].g) && nobetb(b1, b2, rgb[i].b)) && !in)
			{
				rgb[i].r = r;
				rgb[i].g = g;
				rgb[i].b = b;
			}
			if (nobetb(r1, r2, rgb[i].r) && nobetb(g1, g2, rgb[i].g) && nobetb(b1, b2, rgb[i].b) && in)
			{
				rgb[i].r = r;
				rgb[i].g = g;
				rgb[i].b = b;
			}
		}
	}
};
class _bitmap
{
public:
	int id;
	HINSTANCE hInstance;
	HBITMAP load_bitmap()
	{
		HBITMAP bmp;
		bmp = LoadBitmap(hInstance, MAKEINTRESOURCE(id));
		return bmp;
	}
};
void showbitmap(HDC hdc, HINSTANCE hInstance, int id, int x, int y, int w, int h, int rop = SRCCOPY)
{
	static int Filled_One;
	static _bitmap bmp[bmpnums];
	static HDC hDC[bmpnums];
	static bool dowork;
	static HDC lhdc;
	lhdc = hdc;
	if (hdc != lhdc)
	{
		dowork = 0;
		for (register int i = 0; i<bmpnums; i++)
			bmp[i].id = -1;
	}
	if (!dowork)
	{
		dowork = 1;
		for (register int i = 0; i<bmpnums; i++)
			hDC[i] = CreateCompatibleDC(hdc);
	}
	for (register int i = 0; i<Filled_One + 1; i++)
		if (bmp[i].id == id&&bmp[i].hInstance == hInstance)
		{
			BitBlt(hdc, x, y, w, h, hDC[i], 0, 0, rop);
			return;
		}
	HBITMAP bitmap;
	Filled_One++;
	Filled_One = Filled_One%bmpnums;
	bmp[Filled_One].id = id;
	bmp[Filled_One].hInstance = hInstance;
	bitmap = bmp[Filled_One].load_bitmap();
	SelectObject(hDC[Filled_One], bitmap);
	BitBlt(hdc, x, y, w, h, hDC[Filled_One], 0, 0, rop);
}
#endif