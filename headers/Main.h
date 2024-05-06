#pragma once

#define _SILENCE_AMP_DEPRECATION_WARNINGS
#include "resource.h"
#include "framework.h"
#include <math.h>
#include <string>
#include <thread>
#include <mutex>
#include <amp.h>
#include <amp_math.h>
#include <complex>
#include <vector>


//const double PI = 3.14159265358979323846;


struct quaternion
{
	quaternion(double W, double X, double Y, double Z);
	double w, x, y, z;
};


//-----------------------------------------------------------------------------
struct SRGB
{
	SRGB(unsigned char r, unsigned char g, unsigned char b)
		: R(r), G(g), B(b)
	{
	}

	unsigned char R, G, B;
};

//-----------------------------------------------------------------------------
struct SPacked_XY
{
	double Y0;
	double X0_1, X0_2;	// SIMD-пакет из 2-х значений
};
//-----------------------------------------------------------------------------
struct SPacked_XY_4
{
	SPacked_XY_4();

	// SIMD-пакеты из 4-х значений
	long double Four_Fours[4];
	long double X0[4];
	long double Y0[4];
};
//-----------------------------------------------------------------------------
struct SPoint_Double
{
	double X, Y;
};
//-----------------------------------------------------------------------------
struct SPoint
{
	SPoint();
	SPoint(unsigned short x, unsigned short y);

	unsigned short X, Y;
};
//-----------------------------------------------------------------------------
struct SSize
{
	SSize();
	SSize(unsigned short width, unsigned short height);

	unsigned short Width, Height;
};
//-----------------------------------------------------------------------------
struct SBuf_Color
{
	SSize Buf_Size;
	unsigned int color;
};
//-----------------------------------------------------------------------------

class AsFrame_DC
{
public:
	~AsFrame_DC();
	AsFrame_DC();

	HDC Get_DC(HWND hwnd, HDC hdc);
	char* Get_Buff();
	void Create_Color_pal();
	void Create_Web_pal();
	void Create_Double_pal(int start_index, const SRGB& color_1, const SRGB& color_2);

	void Draw_Color_pal(HDC frame_dc);	
	void Draw_Web_pal(HDC frame_dc);
	void Draw_Multicolor_pal(HDC frame_dc);
	void Draw_Grayscale_pal(HDC frame_dc);

	//int Width, Height;
	SSize Buf_Size;
	HBRUSH BG_Brush;
	HPEN White_pen;	

	static const int colors_cnt = 400;

	int Palette_RGB[colors_cnt];
	int Palette_Web[colors_cnt];

private:
	int Color_to_RGB(int color);

	HDC dc;
	HBITMAP Bitmap;
	char* Bitmap_buff;

	HPEN Palette_pens[colors_cnt];
	HBRUSH Palette_brush[colors_cnt];

};
//-----------------------------------------------------------------------------
extern "C" void Asm_Draw(char *video_buff, SSize size);
extern "C" void Asm_Draw_Line(char* video_buff, SPoint start_point, SPoint end_point, SBuf_Color buf_color);
extern "C" void Asm_Draw_Hor_Line(char* video_buff, SPoint start_point, int len, SBuf_Color buf_color);
extern "C" void Asm_Set_Pixel(char* video_buff, SPoint point, SBuf_Color buf_color);
extern "C" int Asm_GetMandelbrotIndex(char* video_buff, long double x_0, long double y_0, int colors_cnt);
extern "C" int Asm_Set_Mandelbrot_Point(char* video_buff, SPoint_Double *xy_0, int * palette_rgb, int colors_cnt);
extern "C" int Asm_Set_Mandelbrot_2_Points(char* video_buff, SPacked_XY* xy_0, int* palette_rgb, int colors_cnt);
extern "C" int Asm_Set_Mandelbrot_4_Points(char* video_buff, SPacked_XY_4* packed_xy, int* palette_rgb, int colors_cnt);


