
#include "MyWindow.h"



HINSTANCE CMyWindow::hThisInst;
WIN_IDLE_FUNC CMyWindow::default_idle_func;
WIN_KEYBOARD_FUNC CMyWindow::default_keyboard_func;

/***************************/
/* Public Static Functions */
/***************************/

//================================================
// Initialize Function
// (Must be called before create windows)
//================================================
void CMyWindow::Initialize(HINSTANCE hThisInst)
{
	CMyWindow::hThisInst = hThisInst;
	default_idle_func = NULL;
	default_keyboard_func = NULL;
}

void CMyWindow::MainLoop()
{
	while (DoEvents());
}

//================================================
// Dispatch Events Function
// (Dispatch a message to the windows)
//================================================
BOOL CMyWindow::DoEvents()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!GetMessage(&msg, NULL, 0, 0)) return FALSE;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	} else {
		WaitMessage();
	}
	return TRUE;
}


/********************/
/* Public Functions */
/********************/

//================================================
// Constructor Function
// (Create new window and register)
//================================================
CMyWindow::CMyWindow(const char* title, int width, int height, int left, int top, WIN_MODE win_mode, HWND hWnd_parent, DWORD dwFlags)
{
	// Initialize Pointer of Callback Functions
	display_func = NULL;
	idle_func = NULL;
	keyboard_func = NULL;
	state_func = NULL;

	wc.lpszClassName = title;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = NULL;
	wc.lpfnWndProc = CMyWindow::WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hThisInst;
	//wc.hIcon = LoadIcon(hThisInst, MAKEINTRESOURCE(IDI_ICON1));
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	//wc.hIconSm = LoadIcon(hThisInst, MAKEINTRESOURCE(IDI_ICON1));
	if (win_mode == WIN_FULL)
		wc.hCursor = NULL;
	else
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wc);

	if (win_mode == WIN_FULL) {
		hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, title, title, WS_VISIBLE | WS_POPUP, left, top, width, height, hWnd_parent, NULL, hThisInst, NULL);
		window_width = client_width = width;
		window_height = client_height = height;
	} else {
		hWnd = CreateWindow(title, title, WS_OVERLAPPED | WS_SYSMENU, left, top, CW_USEDEFAULT, CW_USEDEFAULT, hWnd_parent, NULL, hThisInst, NULL);
		RECT wRect, cRect;
		GetWindowRect(hWnd, &wRect);
		GetClientRect(hWnd, &cRect);
		int ww = wRect.right - wRect.left - (cRect.right - cRect.left) + width;
		int wh = wRect.bottom - wRect.top - (cRect.bottom - cRect.top) + height;
		SetWindowPos(hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE);
		window_width = ww;
		window_height = wh;
		client_width = width;
		client_height = height;
	}
	SetProp(hWnd, PROP_WINPROC, this);

	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;

	img_screen = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 4);
	hBitmap_screen = CreateDIBSection(0, &bmi, DIB_RGB_COLORS, (void**)&(img_screen->imageData), NULL, 0);
	hDC_screen = CreateCompatibleDC(NULL);
	SelectObject(hDC_screen, hBitmap_screen);

	img_buffer = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 4);
	hBitmap_buffer = CreateDIBSection(0, &bmi, DIB_RGB_COLORS, (void**)&(img_screen->imageData), NULL, 0);
	hDC_buffer = CreateCompatibleDC(NULL);
	SelectObject(hDC_buffer, hBitmap_buffer);

	int pixelformat;
	PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | dwFlags, 
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0, 0,
		0, 0, 0, 0, 0,
		32,
		32,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	HDC hDC = GetDC(hWnd);
	pixelformat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelformat, &pfd);
	DescribePixelFormat(hDC, pixelformat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	hRC = wglCreateContext(hDC);
	ReleaseDC(hWnd, hDC);
}

//================================================
// Destructor Function
// (Close and Destroy window)
//================================================
CMyWindow::~CMyWindow()
{
	DeleteObject(hBitmap_screen);
	DeleteDC(hDC_screen);
	cvReleaseImage(&img_screen);

	DeleteObject(hBitmap_buffer);
	DeleteDC(hDC_buffer);
	cvReleaseImage(&img_buffer);

	wglDeleteContext(hRC);

	DestroyWindow(hWnd);
}

//================================================
// Show Window Function
// (Show window and activate)
//================================================
void CMyWindow::Show(int nCmdShow)
{
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}

//================================================
// Hide Window Function
// (Hide window and deactivate)
//================================================
void CMyWindow::Hide()
{
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
}

//================================================
// Post Redisplay Function
// (Post redisplay message 'WM_PAINT' to WndProc)
//================================================
void CMyWindow::PostRedisplay()
{
	InvalidateRgn(hWnd, 0, 0);
	//UpdateWindow(hWnd);
}

//================================================
// Call Redisplay Function
// (Call redisplay function)
//================================================
void CMyWindow::CallRedisplay()
{
	Redisplay();
}

//================================================
// Get Buffer Image Function
// (Get Bitmap Image from Window)
//================================================
void CMyWindow::GetBuffer(IplImage* image)
{
	if (image->width == img_buffer->width && image->height == img_buffer->height) {
		cvFlip(img_buffer, image, 0);
	} else {
		IplImage* img_buffer_resized = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 4);
		cvResize(img_buffer, img_buffer_resized);
		cvFlip(img_buffer_resized, image);
		cvReleaseImage(&img_buffer_resized);
	}
}


/****************************/
/* Private Static Functions */
/****************************/

//================================================
// WinProc Function
// (Called when window message was reached)
//================================================
LRESULT CALLBACK CMyWindow::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CMyWindow* This = (CMyWindow*)GetProp(hWnd, PROP_WINPROC);
	if (This == NULL) return DefWindowProc(hWnd, message, wParam, lParam);

	try {
		if (This != NULL && This->state_func != NULL && This->state_func(hWnd, message, wParam, lParam)) return 0;
	} catch ( ... ) {
		return 0;
	}

	switch (message) {
	case WM_KEYDOWN:
	case WM_KEYUP:
		if (default_keyboard_func != NULL) default_keyboard_func(message, (int)wParam);
		try {
			if (This->keyboard_func != NULL) This->keyboard_func(message, (int)wParam);
		} catch ( ... ) {
		}
		break;
	case WM_DESTROY:
		RemoveProp(hWnd, PROP_WINPROC);
		break;
		//case WM_PAINT:
		//	ValidateRgn(hWnd, 0);
		//	if (This->display_func != NULL) {
		//		This->Redisplay();
		//	}
		//	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


/*********************/
/* Private Functions */
/*********************/

//================================================
// Redraw Function
// (Redraw window for OpenGL)
//================================================
void CMyWindow::Redisplay()
{
	HDC hDC = GetDC(hWnd);
	wglMakeCurrent(hDC, hRC);

	try {
		display_func(hWnd, hDC);
	} catch ( ... ) {
	}

	if (set_buffer) SetBuffer(hDC);
	SwapBuffers(hDC);
	wglMakeCurrent(hDC, NULL);
	ReleaseDC(hWnd, hDC);
}

//================================================
// Set Buffer Image Function
// (Get Bitmap Image from Window)
//================================================
void CMyWindow::SetBuffer(HDC hDC)
{
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, img_buffer->width, img_buffer->height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, img_buffer->imageData);
}

