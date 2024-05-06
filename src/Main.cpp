// Main.cpp : Defines the entry point for the application.
//

#include "Main.h"

//SPoint_Double
//-----------------------------------------------------------------------------
//SPoint_Double::SPoint_Double()
//	: X(0), Y(0)
//{
//}
////-----------------------------------------------------------------------------
//SPoint_Double::SPoint_Double(double x, double y)
//	: X(x), Y(y)
//{
//}
//-----------------------------------------------------------------------------

quaternion::quaternion(double W, double X, double Y, double Z)
	: w(W), x(X), y(Y), z(Z)
{
}

double quat_len(quaternion q)
{

	return max(0.0000000000000000001,sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z));

}

quaternion Mul(quaternion q1, quaternion q2)
{
	quaternion q = {0,0,0,0};

	//q.w = q1.w * q2.w;
	/*if (q2.w == 0)
	{
		q.w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
		q.x = q1.w * q2.x + q1.y * q2.z - q1.z * q2.y;
		q.y = q1.w * q2.y - q1.x * q2.z + q1.z * q2.x;
		q.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x;
	}
	else {*/
	q.w = (q1.w * q2.w) + (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z);
	q.x = (q1.w * q2.x) + (q1.x * q2.w) - (q1.y * q2.z) + (q1.z * q2.y);
	q.y = (q1.w * q2.y) + (q1.y * q2.w) - (q1.z * q2.x) + (q1.x * q2.z);
	q.z = (q1.w * q2.z) + (q1.z * q2.w) - (q1.x * q2.y) + (q1.y * q2.x);
	
	//q.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
	//q.x = (q1.w * q2.x) - (q1.y * q2.z);
	//}

	return q;
}

double Dot(quaternion q1, quaternion q2)
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

quaternion Norm(quaternion q1)
{
	quaternion q = { 0,0,0,0 };

	double len = quat_len(q1);
	len = 1.0 / len;

	q.x = q1.x * len;
	q.y = q1.y * len;
	q.z = q1.z * len;

	return q;
}

quaternion operator+(quaternion q1, quaternion q2)
{
	quaternion q(0.0, 0.0, 0.0, 0.0);

	q.w = q1.w + q2.w;
	q.x = q1.x + q2.x;
	q.y = q1.y + q2.y;
	q.z = q1.z + q2.z;

	return q;
}

quaternion operator-(quaternion q1, quaternion q2)
{
	quaternion q(0.0, 0.0, 0.0, 0.0);

	q.w = q1.w - q2.w;
	q.x = q1.x - q2.x;
	q.y = q1.y - q2.y;
	q.z = q1.z - q2.z;

	return q;
}

quaternion operator*(quaternion q1, double q2)
{
	quaternion q(0.0, 0.0, 0.0, 0.0);

	q.w = q1.w * q2;
	q.x = q1.x * q2;
	q.y = q1.y * q2;
	q.z = q1.z * q2;

	return q;
}

quaternion abs(quaternion q1)
{
	quaternion q(0.0, 0.0, 0.0, 0.0);

	q.w = std::abs(q1.w);
	q.x = std::abs(q1.x);
	q.y = std::abs(q1.y);
	q.z = std::abs(q1.z);

	return q;
}

double deg2rad(int deg)
{
	return (double)deg * 0.017453292519943295;
}

//SPacked_XY_4
//-----------------------------------------------------------------------------
SPacked_XY_4::SPacked_XY_4()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		Four_Fours[i] = 4.0;
	}
}
//SPoint
//-----------------------------------------------------------------------------
SPoint::SPoint()
	: X(0), Y(0)
{
}
//-----------------------------------------------------------------------------
SPoint::SPoint(unsigned short x, unsigned short y)
	: X(x), Y(y)
{
}
//-----------------------------------------------------------------------------


//SSize
//-----------------------------------------------------------------------------
SSize::SSize()
	: Width(0),Height(0)
{
}
//-----------------------------------------------------------------------------
SSize::SSize(unsigned short width, unsigned short height)
	: Width(width), Height(height)
{
}
//-----------------------------------------------------------------------------
//AsFrame_DC
//-----------------------------------------------------------------------------
AsFrame_DC::~AsFrame_DC()
{
	if (Palette_pens[0] != 0 && Palette_brush[0] != 0)
	{
		int i;
		for (i = 0; i < colors_cnt; i++)
		{
			DeleteObject(Palette_pens[i]);
			DeleteObject(Palette_brush[i]);
			Palette_pens[i] = 0;
			Palette_brush[i] = 0;
		}
	}
	if (Bitmap != 0)
		DeleteObject(Bitmap);
	if (dc != 0)
		DeleteObject(dc);
}
//-----------------------------------------------------------------------------
AsFrame_DC::AsFrame_DC()
: dc(0), Bitmap(0), BG_Brush(0), Bitmap_buff(0)
{
	BG_Brush = CreateSolidBrush(RGB(0,0,0));
	White_pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

	//Create_Double_pal(0, SRGB(71,60,49), SRGB(75, 147, 254));
	//Create_Double_pal(0, SRGB(0, 0, 0), SRGB(255, 255, 225));
	//Create_Double_pal(colors_cnt / 2, SRGB(255, 255, 225), SRGB(83, 33, 78));
	//Create_Double_pal(colors_cnt / 3, SRGB(255, 255, 225), SRGB(0, 0, 0));
	//Create_Web_pal();
	Create_Color_pal();
}
//-----------------------------------------------------------------------------
HDC AsFrame_DC::Get_DC(HWND hwnd, HDC hdc)
{
	int dc_width, dc_height;
	RECT rect;
	BITMAPINFO bmp_info{};
	
	GetClientRect(hwnd, &rect);
	dc_width = rect.right - rect.left;
	dc_height = rect.bottom - rect.top;

	if (dc_width != Buf_Size.Width && dc_height != Buf_Size.Height)
	{
		if (Bitmap != 0)
			DeleteObject(Bitmap);
		if (dc != 0)
			DeleteObject(dc);

		Buf_Size.Width = dc_width;
		Buf_Size.Height = dc_height;

		dc = CreateCompatibleDC(hdc);

		bmp_info.bmiHeader.biSize = sizeof(BITMAPINFO);
		bmp_info.bmiHeader.biWidth = Buf_Size.Width;
		bmp_info.bmiHeader.biHeight = Buf_Size.Height;
		bmp_info.bmiHeader.biPlanes = 1;
		bmp_info.bmiHeader.biBitCount = 32;
		bmp_info.bmiHeader.biCompression = BI_RGB;

		Bitmap = CreateDIBSection(hdc, &bmp_info, DIB_RGB_COLORS, (void**)&Bitmap_buff, 0, 0);
		
		if (Bitmap != 0)
			SelectObject(dc, Bitmap);

		++rect.right;
		++rect.bottom;

		SelectObject(dc, BG_Brush);
		Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);

	}
	return dc;
}
//-----------------------------------------------------------------------------
char* AsFrame_DC::Get_Buff()
{
	return Bitmap_buff;

}
//-----------------------------------------------------------------------------
void AsFrame_DC::Draw_Grayscale_pal(HDC frame_dc)
{
	int i;
	HPEN pen;
	HBRUSH brush;
	double x_pos = 0.0;
	double bar_width = (double)Buf_Size.Width / (double)colors_cnt;

	for (i = 0; i < colors_cnt; i++)
	{
		pen = CreatePen(PS_SOLID, 0, RGB(i, i, i));
		brush = CreateSolidBrush(RGB(i, i, i));

		SelectObject(frame_dc, pen);
		SelectObject(frame_dc, brush);

		Rectangle(frame_dc, (int)x_pos, 0, (int)(x_pos + bar_width), Buf_Size.Height / 2);

		x_pos += bar_width;

		DeleteObject(pen);
		DeleteObject(brush);
	}
}
//-----------------------------------------------------------------------------
void AsFrame_DC::Create_Color_pal()
{
	int i;
	int rgb_color;
	int color_ang = 200;

	for (i = 0; i < colors_cnt; i++)
	{
		//color_ang = (int)((double)i / (double)colors_cnt * 360.0);
		color_ang += 1;
		if (color_ang >= 220)
			color_ang = 200;
		rgb_color = Color_to_RGB(color_ang);
		
		Palette_RGB[i] = rgb_color;


		Palette_pens[i] = CreatePen(PS_SOLID, 0, rgb_color);
		Palette_brush[i] = CreateSolidBrush(rgb_color);

	}

}
//-----------------------------------------------------------------------------
void AsFrame_DC::Create_Web_pal()
{
	int i, j;
	int pos = 0;
	unsigned char r, g, b;
	unsigned int base_color = 0;

	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (j & 1)
				r = 21;
			else
				r = 0;

			if (j & 2)
				g = 21;
			else
				g = 0;

			if (j & 4)
				b = 21;
			else
				b = 0;

			Palette_Web[pos++] = base_color + RGB(r, g, b);
		}

		base_color += 0x00151515;

		/*r = (i % 6) * 0x33;
		g = ( (i / 6) % 6) * 0x33;
		b = ( (i / 36) % 6) * 0x33;*/
		
	}

}
//-----------------------------------------------------------------------------
void AsFrame_DC::Create_Double_pal(int start_index, const SRGB &color_1, const SRGB &color_2)
{
	int i;
	int len = colors_cnt / 2;
	double cur_r, cur_g, cur_b;
	double delta_r, delta_g, delta_b;
	SRGB cur_color(0, 0, 0);

	delta_r = (double)(color_2.R - color_1.R) / (double)len;
	delta_g = (double)(color_2.G - color_1.G) / (double)len;
	delta_b = (double)(color_2.B - color_1.B) / (double)len;

	cur_r = (double)color_1.R;
	cur_g = (double)color_1.G;
	cur_b = (double)color_1.B;

	for (i = 0; i < len; i++)
	{
		cur_color.R = (unsigned char)cur_r;
		cur_color.G = (unsigned char)cur_g;
		cur_color.B = (unsigned char)cur_b;

		cur_r += delta_r;
		cur_g += delta_g;
		cur_b += delta_b;

		Palette_RGB[start_index + i] = RGB(cur_color.R, cur_color.G, cur_color.B);

	}
}
//-----------------------------------------------------------------------------
void AsFrame_DC::Draw_Color_pal(HDC frame_dc)
{
	int i;
	double x_pos = 0.0;
	double bar_width = (double)Buf_Size.Width / (double)colors_cnt;

	for (i = 0; i < colors_cnt; i++)
	{

		SelectObject(frame_dc, Palette_pens[i]);
		SelectObject(frame_dc, Palette_brush[i]);

		Rectangle(frame_dc, (int)x_pos, Buf_Size.Height / 2, (int)(x_pos + bar_width), Buf_Size.Height);

		x_pos += bar_width;

		DeleteObject(Palette_pens[i]);
		DeleteObject(Palette_brush[i]);
		Palette_pens[i] = 0;
		Palette_brush[i] = 0;
	}
}
//-----------------------------------------------------------------------------
void AsFrame_DC::Draw_Web_pal(HDC frame_dc)
{
	int i;
	int len = sizeof(Palette_Web) / sizeof(Palette_Web[0]);
	HPEN pen;
	HBRUSH brush;
	double x_pos = 0.0;
	double bar_width = (double)Buf_Size.Width / (double)len;

	for (i = 0; i < len; i++)
	{
		pen = CreatePen(PS_SOLID, 0, Palette_Web[i]);
		brush = CreateSolidBrush(Palette_Web[i]);

		SelectObject(frame_dc, pen);
		SelectObject(frame_dc, brush);

		Rectangle(frame_dc, (int)x_pos, Buf_Size.Height / 2, (int)(x_pos + bar_width), Buf_Size.Height);

		x_pos += bar_width;

		DeleteObject(pen);
		DeleteObject(brush);
	}
}
//-----------------------------------------------------------------------------
void AsFrame_DC::Draw_Multicolor_pal(HDC frame_dc)
{
	int i;
	//int len = sizeof(Palette_RGB) / sizeof(Palette_RGB[0]);
	HPEN pen;
	HBRUSH brush;
	double x_pos = 0.0;
	double bar_width = (double)Buf_Size.Width / (double)colors_cnt;

	for (i = 0; i < colors_cnt; i++)
	{
		pen = CreatePen(PS_SOLID, 0, Palette_RGB[i]);
		brush = CreateSolidBrush(Palette_RGB[i]);

		SelectObject(frame_dc, pen);
		SelectObject(frame_dc, brush);

		Rectangle(frame_dc, (int)x_pos, Buf_Size.Height / 2, (int)(x_pos + bar_width), Buf_Size.Height);

		x_pos += bar_width;

		DeleteObject(pen);
		DeleteObject(brush);
	}
}
//-----------------------------------------------------------------------------
int AsFrame_DC::Color_to_RGB(int color)
{// color [0;360) = H для HSV, S = 1.0, V = 1.0

	unsigned char r, g, b;
	unsigned char v_inc, v_dec;
	int h;
	double a;

	h = color / 60;	// h [0;5]

	a = (double)(color % 60) / 60.0;		// a [0;1)

	v_inc = (unsigned char)(a * 255.0);
	v_dec = (unsigned char)((1.0 - a) * 255.0);

	switch (h)
	{
	case 0:
		r = 255;
		g = v_inc;
		b = 0;
		break;
	case 1:
		r = v_dec;
		g = 255;
		b = 0;
		break;
	case 2:
		r = 0;
		g = 255;
		b = v_inc;
		break;
	case 3:
		r = 0;
		g = v_dec;
		b = 255;
		break;
	case 4:
		r = v_inc;
		g = 0;
		b = 255;
		break;
	case 5:
		r = 255;
		g = 0;
		b = v_dec;
		break;
	default:
		throw 13;
	}

	return RGB(r, g, b);
}
//-----------------------------------------------------------------------------


#define MAX_LOADSTRING 100

// Global Variables:
long double Main_scale = 1.7500000000000000;

long double Center_X = -1.69100052764625;			// > x - вправо, < x - влево
long double Center_Y = 0.00000000000000001;				// > y - вниз, < y - вверх


AsFrame_DC Frame_DC; 
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

//-----------------------------------------------------------------------------
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//-----------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MANDELBROT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MANDELBROT));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
//-----------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MANDELBROT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = Frame_DC.BG_Brush;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MANDELBROT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//-----------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hwnd;
	RECT window_rc;

	window_rc.left = 0;
	window_rc.top = 0;
	window_rc.right = 1200;
	window_rc.bottom = 800;

	hInst = hInstance; // Store instance handle in our global variable

	AdjustWindowRect(&window_rc, WS_OVERLAPPEDWINDOW - WS_THICKFRAME, TRUE);

	hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW - WS_THICKFRAME,
		0, 0, window_rc.right - window_rc.left, window_rc.bottom - window_rc.top, nullptr, nullptr, hInstance, nullptr);

	if (hwnd == 0)
	{
		return FALSE;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	return TRUE;
}

//-----------------------------------------------------------------------------
void Draw_Half(HDC frame_dc, int start, int end)
{
	int i, x, y, z;
	long double x_0, y_0, z_0;
	long double x_n, y_n, z_n;
	long double x_n1, y_n1, z_n1;
	long double scale_x = (long double)Frame_DC.Buf_Size.Width / (long double)Frame_DC.Buf_Size.Height * Main_scale;
	long double distance;
	int color;

	char* video_buf;
	char* curr_video_buf;
	SPoint point;
	SBuf_Color buf_color;

	video_buf = Frame_DC.Get_Buff();
	curr_video_buf = video_buf;
	buf_color.Buf_Size = Frame_DC.Buf_Size;


	for (y = start; y < end; y++)
	{
		y_0 = (long double)y / (long double)Frame_DC.Buf_Size.Height - 0.5;
		y_0 = y_0 * Main_scale + Center_Y;

		for (x = 0; x < Frame_DC.Buf_Size.Width; x++)
		{
			x_0 = (long double)x / (long double)Frame_DC.Buf_Size.Width - 0.5; // Получаем x_0 в диапозоне [-0.5;0.5)
			x_0 = x_0 * scale_x + Center_X;

			x_n = 0.0;
			y_n = 0.0;

			for (i = 0; i < Frame_DC.colors_cnt; i++)
			{
				x_n1 = x_n * x_n - y_n * y_n + x_0;
				y_n1 = 2.0 * x_n * y_n + y_0;

				distance = x_n1 * x_n1 + y_n1 * y_n1;
				if (distance > 4.0)
					break;

				x_n = x_n1;
				y_n = y_n1;
			}

			if (i == Frame_DC.colors_cnt)
				color = 0;
			else
				color = Frame_DC.Palette_RGB[i];

			buf_color.color = color;
			point.X = (unsigned short)x;
			point.Y = (unsigned short)y;

			Asm_Set_Pixel(curr_video_buf, point, buf_color);

		}
	}
}
//-----------------------------------------------------------------------------
void Draw_Half_Fast(HDC frame_dc, int start, int end)
{
	int i, x, y;
	long double scale_x = (long double)Frame_DC.Buf_Size.Width / (long double)Frame_DC.Buf_Size.Height * Main_scale;
	long double distance;
	SPacked_XY_4 packed_xy;
	SPoint point;
	char* curr_video_buf;
	int color;

	curr_video_buf = Frame_DC.Get_Buff();
	curr_video_buf += start * Frame_DC.Buf_Size.Width * 4;
	

	for (y = start; y < end; y++)
	{
		packed_xy.Y0[0] = (long double)y / (long double)Frame_DC.Buf_Size.Height - 0.5;
		packed_xy.Y0[0] = packed_xy.Y0[0] * Main_scale + Center_Y;

		for (i = 1; i < 4; i++)
		{
			packed_xy.Y0[i] = packed_xy.Y0[0];
		}

		for (x = 0; x < Frame_DC.Buf_Size.Width; x += 4)
		{
			for (i = 0; i < 4; i++)
			{
				packed_xy.X0[i] = (long double)(x + i) / (long double)Frame_DC.Buf_Size.Width - 0.5; // Получаем x_0 в диапозоне [-0.5;0.5)
				packed_xy.X0[i] = packed_xy.X0[i] * scale_x + Center_X;

			}

			Asm_Set_Mandelbrot_4_Points(curr_video_buf, &packed_xy, Frame_DC.Palette_RGB, Frame_DC.colors_cnt);

			curr_video_buf += 4 * 4;
		}
	}
}
//-----------------------------------------------------------------------------
void Draw_Mandelbrot(HDC frame_dc)
{
	int i, x, y;
	long double x_0, y_0;
	long double x_n, y_n;
	long double x_n1, y_n1;

	long double r = 0.0;

	//long double center_x = -1.69100052764625;			// > x - вправо, < x - влево
	//long double center_y = 0.00000000000000001;				// > y - вниз, < y - вверх
	long double scale_x = (long double)Frame_DC.Buf_Size.Width / (long double)Frame_DC.Buf_Size.Height * Main_scale;
	long double distance;
	int color = 0;
	unsigned long long startTick, endTick, deltaTick;

	SBuf_Color buf_color;
	char* video_buf;
	char* curr_video_buf;
	SPoint point;

	video_buf = Frame_DC.Get_Buff();
	curr_video_buf = video_buf;
	buf_color.Buf_Size = Frame_DC.Buf_Size;


	startTick = __rdtsc();
	

	std::thread t1(Draw_Half, frame_dc, Frame_DC.Buf_Size.Height / 2, Frame_DC.Buf_Size.Height);
	for (y = 0; y < Frame_DC.Buf_Size.Height / 2; y++)
	{
		y_0 = (long double)y / (long double)Frame_DC.Buf_Size.Height - 0.5;
		y_0 = y_0 * Main_scale + Center_Y;

		for (x = 0; x < Frame_DC.Buf_Size.Width; x++)
		{
			x_0 = (long double)x / (long double)Frame_DC.Buf_Size.Width - 0.5; // Получаем x_0 в диапозоне [-0.5;0.5)
			x_0 = x_0 * scale_x + Center_X;
			
			x_n = 0.0;
			y_n = 0.0;

			for (i = 0; i < Frame_DC.colors_cnt; i++)
			{
				x_n1 = x_n * x_n - y_n * y_n + x_0;
				y_n1 = 2.0 * x_n * y_n + y_0;

				distance = x_n1 * x_n1 + y_n1 * y_n1;
				if (distance > 4.0)
					break;

				x_n = x_n1;
				y_n = y_n1;
			}

			if (i == Frame_DC.colors_cnt)
				color = 0;
			else
				color = Frame_DC.Palette_RGB[i];

			buf_color.color = color;
			point.X = (unsigned short)x;
			point.Y = (unsigned short)y;

			Asm_Set_Pixel(curr_video_buf, point, buf_color);
		
		}
	} 
	t1.join();

	endTick = __rdtsc();

	deltaTick = endTick - startTick;			// 3 520 631 695, 3 425 230 382, 3 545 881 938, 3 576 698 990 ; onethread with SetPixel
												// 1 360 071 381, 1 388 739 607, 1 367 063 070, 1 526 369 533 ; multithread with SetPixel
												// 
												//	 652 894 113,   646 880 890,   691 717 180,   652 996 080 ; onethread with Asm_Set_Pixel
												//   338 391 634,   368 539 376,   317 029 000,   346 455 464 ; multithread with Asm_Set_Pixel 

	SetPixel(frame_dc, Frame_DC.Buf_Size.Width / 2, Frame_DC.Buf_Size.Height / 2, RGB(255, 255, 255));
}
//-----------------------------------------------------------------------------
int GetMandelbrotIndex(long double x_0, long double y_0, int colors_cnt)
{
	int i;
	long double x_n, y_n;
	long double x_n1, y_n1;
	long double distance;

	x_n = 0.0;
	y_n = 0.0;

	for (i = 0; i < colors_cnt; i++)
	{
		x_n1 = x_n * x_n - y_n * y_n + x_0;
		y_n1 = 2.0 * x_n * y_n + y_0;

		distance = x_n1 * x_n1 + y_n1 * y_n1;
		if (distance > 4.0)
			break;

		x_n = x_n1;
		y_n = y_n1;
	}
	return i;
}
//-----------------------------------------------------------------------------
void Draw_Mandelbrot_Fast(HDC frame_dc)
{
	int i;
	int x, y;
	SPacked_XY_4 packed_xy;
	long double scale_x = (long double)Frame_DC.Buf_Size.Width / (long double)Frame_DC.Buf_Size.Height * Main_scale;
	unsigned long long startTick, endTick, deltaTick;

	SBuf_Color buf_color;
	char* video_buf;
	char* curr_video_buf;
	SPoint point;

	video_buf = Frame_DC.Get_Buff();
	curr_video_buf = video_buf;
	buf_color.Buf_Size = Frame_DC.Buf_Size;

	startTick = __rdtsc();

	//std::thread t1(Draw_Half_Fast, frame_dc, Frame_DC.Buf_Size.Height / 2, Frame_DC.Buf_Size.Height);

	for (y = 0; y < Frame_DC.Buf_Size.Height; y++)
	{
		packed_xy.Y0[0] = (long double)y / (long double)Frame_DC.Buf_Size.Height - 0.5;
		packed_xy.Y0[0] = packed_xy.Y0[0] * Main_scale + Center_Y;

		for (i = 1; i < 4; i++)
		{
			packed_xy.Y0[i] = packed_xy.Y0[0];
		}

		for (x = 0; x < Frame_DC.Buf_Size.Width; x += 4)
		{
			for (i = 0; i < 4; i++)
			{
				packed_xy.X0[i] = (long double)(x + i) / (long double)Frame_DC.Buf_Size.Width - 0.5; // Получаем x_0 в диапозоне [-0.5;0.5)
				packed_xy.X0[i] = packed_xy.X0[i] * scale_x + Center_X;
				
			}

			Asm_Set_Mandelbrot_4_Points(curr_video_buf, &packed_xy, Frame_DC.Palette_RGB, Frame_DC.colors_cnt);

			curr_video_buf += 4 * 4;
		}
	} 

	//t1.join();

	endTick = __rdtsc();

	deltaTick = endTick - startTick;			// 1 579 063 260, 1 625 955 684, 1 671 708 404
												//   835 597 996,   836 947 944,   702 606 920
												// 2 885 792 046, 3 345 537 241, 3 009 871 080, 2 959 353 253
												// 3 140 626 244, 2 813 625 354, 2 814 421 957, 2 903 160 470
												// 1 679 193 605, 1 455 139 654, 1 619 393 421, 1 448 904 280	; xmm
												//   897 312 846,   698 440 894,   845 750 038,   723 967 038	; ymm
												//	 440 966 032,   445 495 927,   436 808 898,   504 957 344	; ymm + multithread

	SetPixel(frame_dc, Frame_DC.Buf_Size.Width / 2, Frame_DC.Buf_Size.Height / 2, RGB(255, 255, 255));

}
//-----------------------------------------------------------------------------
void On_Paint(HWND hwnd)
{
	
	HDC hdc, frame_dc;
	PAINTSTRUCT ps;
	

	hdc = BeginPaint(hwnd, &ps);
	frame_dc = Frame_DC.Get_DC(hwnd, hdc);

	
	GdiFlush();

		//doubleDouble a("123154654.123454");

		//Clear_screen(frame_dc);
	//Main_scale /= 1.0;

	Draw_Mandelbrot_Fast(frame_dc);

		//Frame_DC.Draw_Grayscale_pal(frame_dc);
		//Frame_DC.Draw_Color_pal(frame_dc);
		//Frame_DC.Draw_Web_pal(frame_dc);
		//Frame_DC.Draw_Multicolor_pal(frame_dc);

	InvalidateRect(hwnd, &ps.rcPaint, FALSE);

	BitBlt(hdc, 0, 0, Frame_DC.Buf_Size.Width, Frame_DC.Buf_Size.Height, frame_dc, 0, 0, SRCCOPY);


	//SelectObject(frame_dc, oldPen);

	EndPaint(hwnd, &ps);

}
//-----------------------------------------------------------------------------
void invalRect(HWND hwnd)
{
	RECT winRect;
	winRect.top = 0;
	winRect.left = 0;
	winRect.right = Frame_DC.Buf_Size.Width;
	winRect.bottom = Frame_DC.Buf_Size.Height;

	InvalidateRect(hwnd, &winRect, FALSE);

}
//-----------------------------------------------------------------------------
void onMouseWheel(HWND hwnd, unsigned int wParam)
{
	short wheel_dt = wParam >> 16;

	if (wheel_dt > 0)
		Main_scale /= 2.0;
	else
		Main_scale *= 2.0;

	invalRect(hwnd);

}
//-----------------------------------------------------------------------------
void onMouseDown(HWND hwnd, unsigned int lParam)
{
	int x, y;
	int window_center_x = Frame_DC.Buf_Size.Width / 2;
	int window_center_y = Frame_DC.Buf_Size.Height / 2;
	double centerXoffset;
	double centerYoffset;

	x = lParam & 0xFFFF;
	y = (lParam >> 16) & 0xFFFF;

	centerXoffset = (double)(x - window_center_x) / (double)Frame_DC.Buf_Size.Height;	// centerXoffset = (double)(x - window_center_x) / (double)Frame_DC.Buf_Size.Width * (double)Frame_DC.Buf_Size.Width / (double)Frame_DC.Buf_Size.Height
	centerYoffset = (double)(y - window_center_y) / (double)Frame_DC.Buf_Size.Height;
	centerYoffset = -centerYoffset;

	Center_X += centerXoffset * Main_scale;
	Center_Y += centerYoffset * Main_scale;

	invalRect(hwnd);
}
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;


	case WM_PAINT:
		On_Paint(hWnd);
		break;

	case WM_LBUTTONDOWN:
		if (wParam == MK_LBUTTON)
			onMouseDown(hWnd, (unsigned int)lParam);
		break;

	case WM_MOUSEWHEEL:
		onMouseWheel(hWnd, (unsigned int)wParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//-----------------------------------------------------------------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
//-----------------------------------------------------------------------------