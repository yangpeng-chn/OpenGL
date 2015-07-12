#pragma once
#define PROP_WINPROC "PropClassWindowProc"
#define PROP_OBJECT "PropClassObject"

#include <windows.h>
#include <windowsx.h>
#include <process.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// OpenGL, GLU
#include <GL/gl.h>
#include <GL/glu.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// OpenCV Library (DLL File Required!!)
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
/*
//#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "cv.lib")
#pragma comment(lib, "highgui.lib")*/
#define WIN_TITLE "Interaction With Object"
#define WIN_WIDTH 800
#define WIN_HEIGHT 680
#define WIN_LEFT 100
#define WIN_TOP 100
enum WIN_MODE {
	WIN_NORMAL,
	WIN_FULL
};

// Callback Functions
typedef void (*WIN_DISPLAY_FUNC)(HWND hWnd, HDC hDC);
typedef void (*WIN_IDLE_FUNC)(void);
typedef void (*WIN_KEYBOARD_FUNC)(UINT message, int key);
typedef BOOL (*WIN_STATE_FUNC)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


/*********************/
/* Class Declaration */
/*********************/

class CMyWindow
{
public:
	// Constructor and Destroctor
	CMyWindow(const char* title, int width, int height, int left, int top, WIN_MODE win_mode=WIN_NORMAL, HWND hWnd_parent=NULL, DWORD dwFlags=0);
	~CMyWindow();
	// Public Static Functions
	static void Initialize(HINSTANCE hThisInst);
	static void MainLoop();
	static BOOL DoEvents();
	static void DefaultIdleFunc(WIN_IDLE_FUNC func) { default_idle_func = func; }
	static void DefaultKeyboardFunc(WIN_KEYBOARD_FUNC func) { default_keyboard_func = func; }
	static void* GetObject(HWND hWnd) { return GetProp(hWnd, PROP_OBJECT); }
	// Public Functions
	void SetDisplayFunc(WIN_DISPLAY_FUNC func, bool set_buffer=false) { display_func = func; this->set_buffer = set_buffer; }
	void SetIdleFunc(WIN_IDLE_FUNC func) { idle_func = func; }
	void SetKeyboardFunc(WIN_KEYBOARD_FUNC func) { keyboard_func = func; }
	void SetStateFunc(WIN_STATE_FUNC func) { state_func = func; }
	void SetObject(void* obj) { SetProp(hWnd, PROP_OBJECT, obj); }
	void Show(int nCmdShow=SW_SHOW);
	void Hide();
	void PostRedisplay();
	void CallRedisplay();
	// Public Const Functions
	HWND GetHWND() { return hWnd; }
	int GetWidth() { return client_width; }
	int GetHeight() { return client_height; }
	CvSize GetSize() { return cvSize(client_width, client_height); }
	int GetWindowWidth() { return window_width; }
	int GetWindowHeight() { return window_height; }
	CvSize GetWindowSize() { return cvSize(window_width, window_height); }
	void GetBuffer(IplImage* image);
private:
	// Private Static Functions
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// Private Functions
	void Redisplay();
	void SetBuffer(HDC hDC);

private:
	int window_width, window_height, client_width, client_height;
	static HINSTANCE hThisInst;
	WNDCLASSEX wc;
	HWND hWnd;
	HGLRC hRC;
	HBITMAP hBitmap_screen, hBitmap_buffer;
	HDC hDC_screen, hDC_buffer;
	IplImage *img_screen, *img_buffer;
	bool set_buffer;
	static WIN_IDLE_FUNC default_idle_func;
	static WIN_KEYBOARD_FUNC default_keyboard_func;
	WIN_DISPLAY_FUNC display_func;
	WIN_IDLE_FUNC idle_func;
	WIN_KEYBOARD_FUNC keyboard_func;
	WIN_STATE_FUNC state_func;
};
