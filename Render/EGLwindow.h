#pragma once
#include "../stdafx.h"
#include "process.h"

#define EGL_WAIT_FOR(cond) while (!cond) Sleep (0);

namespace EGL_WND {
	bool Initialize();
	void Cleanup();

	void    Lock();
	bool TryLock();
	void  Unlock();

	HWND             egl_wnd		 = NULL;
	CRITICAL_SECTION egl_locker		 = {};
	UINT             egl_wndThreadID = 0;
	BITMAPINFO       egl_bmpInfo	 = {};
	HDC              egl_dcBuf		 = NULL;
	BYTE*            egl_colorBuf	 = NULL;
	SIZE             egl_colorBufSz  = {};
	float*			 egl_depthBuf	 = NULL;
	SIZE			 egl_depthBufSz  = {};
	bool             egl_initialized = Initialize();

    const SIZE& GetWndSize() { return egl_depthBufSz; }

	bool Initialize()
	{
		InitializeCriticalSection (&egl_locker);

		atexit (Cleanup);

		return true;
	}

	void Cleanup()
	{
		egl_initialized = false;

		delete[] egl_colorBuf;
		 		 egl_colorBuf   = NULL;
				 egl_colorBufSz = {};

		delete[] egl_depthBuf;
				 egl_depthBuf   = NULL;
				 egl_depthBufSz = {};
	
		DeleteCriticalSection (&egl_locker);
	}

	void Lock()
	{
		EnterCriticalSection (&egl_locker);
	}

	bool TryLock()
	{
		return !!TryEnterCriticalSection (&egl_locker);
	}

	void Unlock()
	{
		LeaveCriticalSection (&egl_locker);
	}
}

HWND eglCreateWindow (short x, short y);

LRESULT CALLBACK eglWnd_WndProc    (HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
UINT    WINAPI   eglWnd_ThreadProc (PVOID pSize);
HWND             eglWnd_CreateWnd  (PSIZE pSize);

LRESULT eglWnd_MsgCreate  (HWND wnd, WPARAM wparam, LPARAM lparam);
LRESULT eglWnd_MsgDestroy (HWND wnd, WPARAM wparam, LPARAM lparam);
LRESULT eglWnd_MsgClose   (HWND wnd, WPARAM wparam, LPARAM lparam);
LRESULT eglWnd_MsgPaint   (HWND wnd, WPARAM wparam, LPARAM lparam);

int  eglFlush();
int  eglFlushBack();
void eglClear(short depth);

HWND eglCreateWindow(short x, short y)
{
	BITMAPINFOHEADER bmiHeader = { sizeof(BITMAPINFOHEADER), x, y, 1, 24, BI_RGB };

	EGL_WND::egl_bmpInfo = { bmiHeader,{} };

	EGL_WND::egl_colorBufSz = { x*3 + x%4 , y };
	EGL_WND::egl_depthBufSz = { x  		  , y };

	EGL_WND::egl_colorBuf = new BYTE[EGL_WND::egl_colorBufSz.cx *
									 EGL_WND::egl_colorBufSz.cy]; assert(EGL_WND::egl_colorBuf);

	EGL_WND::egl_depthBuf = new float[EGL_WND::egl_depthBufSz.cx *
									  EGL_WND::egl_depthBufSz.cy]; assert(EGL_WND::egl_depthBuf);

	memset(EGL_WND::egl_colorBuf, 0, sizeof(BYTE)  * EGL_WND::egl_colorBufSz.cx*EGL_WND::egl_colorBufSz.cy);
	memset(EGL_WND::egl_depthBuf, 0, sizeof(float) * EGL_WND::egl_depthBufSz.cx*EGL_WND::egl_depthBufSz.cy);

	_beginthreadex (NULL, 0, &eglWnd_ThreadProc, (PVOID)&EGL_WND::egl_depthBufSz, 0, &EGL_WND::egl_wndThreadID);
	assert (EGL_WND::egl_wndThreadID);

	EGL_WAIT_FOR (EGL_WND::egl_wnd);

	return EGL_WND::egl_wnd;
}

LRESULT CALLBACK eglWnd_WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:  return eglWnd_MsgCreate  (wnd, wparam, lparam);
	case WM_DESTROY: return eglWnd_MsgDestroy (wnd, wparam, lparam);
	case WM_CLOSE:   return eglWnd_MsgClose   (wnd, wparam, lparam);
	case WM_PAINT:   return eglWnd_MsgPaint   (wnd, wparam, lparam);
	default:		 return DefWindowProc     (wnd, msg, wparam, lparam);
	}

	return 0;
}

UINT WINAPI eglWnd_ThreadProc(PVOID pSize)
{
	WNDCLASSEX wndClass = {};

	WCHAR className[256] = L"";
	swprintf_s(className, 256, L"THREAD ID: %x", (int)GetCurrentThreadId());

	wndClass.cbSize		   = sizeof(wndClass);
	wndClass.hInstance     = NULL;
	wndClass.lpfnWndProc   = &eglWnd_WndProc;
	wndClass.style		   = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpszClassName = className;
	wndClass.lpszMenuName  = className;
	wndClass.hbrBackground = (HBRUSH)GetStockObject (HOLLOW_BRUSH);
	wndClass.hCursor	   = LoadCursor (NULL, IDC_ARROW);
	wndClass.hIcon		   = NULL;

	RegisterClassEx (&wndClass);

	EGL_WND::egl_wnd = eglWnd_CreateWnd((PSIZE)pSize); assert (EGL_WND::egl_wnd);

	UpdateWindow (EGL_WND::egl_wnd);
	  ShowWindow (EGL_WND::egl_wnd, SW_SHOW);

	MSG message = {};
	while (GetMessage (&message, EGL_WND::egl_wnd, 0, 0))
	{
		TranslateMessage (&message);
		 DispatchMessage (&message);

		Sleep(0);
	}

	EGL_WND::egl_wndThreadID = 0;

	exit (0);

	return message.wParam;
}

HWND eglWnd_CreateWnd(PSIZE pSize)
{
	DWORD exStyle = WS_EX_APPWINDOW | WS_EX_CLIENTEDGE,
			style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_BORDER);

	WCHAR name[256] = L"";
	swprintf_s(name, 256, L"THREAD ID: %x", (int)GetCurrentThreadId());

	EGL_WND::egl_wnd = CreateWindowEx(exStyle, name, name, style,
									 (GetSystemMetrics(SM_CXSCREEN) - pSize->cx) / 2,
									 (GetSystemMetrics(SM_CYSCREEN) - pSize->cy) / 2,
									  GetSystemMetrics(SM_CYMENU)   + pSize->cx,
									  GetSystemMetrics(SM_CXFRAME)  + pSize->cy,
									  NULL, NULL, NULL, NULL);	assert(EGL_WND::egl_wnd);

	return EGL_WND::egl_wnd;
}

LRESULT eglWnd_MsgCreate(HWND wnd, WPARAM wparam, LPARAM lparam)
{
	RECT paintArea   = {};
	SIZE paintAreaSz = {};

	GetClientRect(wnd, &paintArea);

	paintAreaSz = {paintArea.right  - paintArea.left,
				   paintArea.bottom - paintArea.top};

	EGL_WND::Lock();
	{
		HDC     wnd_dc = GetDC (wnd);													  assert(wnd_dc);
		HBITMAP bmp    = CreateCompatibleBitmap (wnd_dc, paintAreaSz.cx, paintAreaSz.cy); assert(bmp);
		HPEN    pen    = CreatePen (PS_SOLID, 1, RGB (255, 255, 255));					  assert(pen);
		HBRUSH  brush = (HBRUSH)GetStockObject (BLACK_BRUSH);							  assert(brush);

		EGL_WND::egl_dcBuf = CreateCompatibleDC (wnd_dc); assert(EGL_WND::egl_dcBuf);

		SelectObject (EGL_WND::egl_dcBuf, bmp);
		SelectObject (EGL_WND::egl_dcBuf, pen);
		SelectObject (EGL_WND::egl_dcBuf, brush);
	}
	EGL_WND::Unlock();

	return 0;
}

LRESULT eglWnd_MsgDestroy(HWND wnd, WPARAM wparam, LPARAM lparam)
{
	EGL_WND::Lock();
	{
		DeleteObject (GetCurrentObject (EGL_WND::egl_dcBuf, OBJ_BRUSH));
		DeleteObject (GetCurrentObject (EGL_WND::egl_dcBuf, OBJ_PEN));
		DeleteObject (GetCurrentObject (EGL_WND::egl_dcBuf, OBJ_BITMAP));
		DeleteDC					   (EGL_WND::egl_dcBuf);
	}
	EGL_WND::Unlock();

	PostQuitMessage (0);

	return 0;
}

LRESULT eglWnd_MsgClose(HWND wnd, WPARAM wparam, LPARAM lparam)
{
	SendNotifyMessage (wnd, WM_DESTROY, 0, 0);

	return 1;
}

LRESULT eglWnd_MsgPaint(HWND wnd, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT paintStruct = {};
	RECT		paintArea   = {};
	SIZE        paintAreaSz = {};

	HDC draw_dc = BeginPaint (wnd, &paintStruct); assert (draw_dc);
	GetClientRect (wnd, &paintArea);

	paintAreaSz = { paintArea.right - paintArea.left, paintArea.bottom - paintArea.top };

	EGL_WAIT_FOR (EGL_WND::TryLock());
	{
		BitBlt(draw_dc,		       0, 0, paintAreaSz.cx, paintAreaSz.cy,
			   EGL_WND::egl_dcBuf, 0, 0, SRCCOPY);
	}
	EGL_WND::Unlock();

	return !EndPaint (wnd, &paintStruct);
}

int eglFlush()
{
	int res = 0;

	EGL_WND::Lock();
	{
		res = SetDIBits (EGL_WND::egl_dcBuf, (HBITMAP)GetCurrentObject(EGL_WND::egl_dcBuf, OBJ_BITMAP), 0,
					 	 EGL_WND::egl_bmpInfo.bmiHeader.biHeight, EGL_WND::egl_colorBuf, &EGL_WND::egl_bmpInfo,
						 DIB_RGB_COLORS); assert(!EGL_WND::egl_bmpInfo.bmiHeader.biHeight || res);
	}
	EGL_WND::Unlock();

	RedrawWindow (EGL_WND::egl_wnd, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_UPDATENOW);

	return res;
}

int eglFlushBack()
{
	int res = 0;

	EGL_WND::Lock();
	{
		res = GetDIBits (EGL_WND::egl_dcBuf, (HBITMAP)GetCurrentObject(EGL_WND::egl_dcBuf, OBJ_BITMAP), 0,
						 EGL_WND::egl_bmpInfo.bmiHeader.biHeight, EGL_WND::egl_colorBuf, &EGL_WND::egl_bmpInfo,
						 DIB_RGB_COLORS); assert(!EGL_WND::egl_bmpInfo.bmiHeader.biHeight || res);
	}
	EGL_WND::Unlock();

	return res;
}

void eglClear(short depth)
{
	for (int i = 0; i < EGL_WND::egl_depthBufSz.cx *
						EGL_WND::egl_depthBufSz.cy; i++) EGL_WND::egl_depthBuf[i] = depth;

	memset(EGL_WND::egl_colorBuf, 0, sizeof(BYTE)*EGL_WND::egl_colorBufSz.cx *
	  											  EGL_WND::egl_colorBufSz.cy);
}
