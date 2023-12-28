#include <windows.h>
#include "glad/glad.h"

LRESULT CALLBACK winprocedure(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hThis, HINSTANCE hPrev, LPSTR Args, int Ncmd) {
  HWND handle;
  MSG msg;
  WNDCLASSEXA wndclassexa = {sizeof(WNDCLASSEXA)};
  wndclassexa.lpszClassName = "game";
  wndclassexa.lpfnWndProc = winprocedure;
  wndclassexa.hInstance = hThis;

  if(!RegisterClassExA(&wndclassexa)) {
    MessageBoxA(nullptr, "Cannot register window", "GAME", MB_OK);
  }

  handle = CreateWindowExA(0, wndclassexa.lpszClassName, "Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                          nullptr, nullptr, wndclassexa.hInstance, nullptr);

  HDC opengldc = GetDC(handle);
  PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR)};

  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 16;

  int format = ChoosePixelFormat(opengldc, &pfd);
  SetPixelFormat(opengldc, format, &pfd);

  HGLRC openglrc = wglCreateContext(opengldc);
  wglMakeCurrent(opengldc, openglrc);

  if(handle == INVALID_HANDLE_VALUE)
    MessageBoxA(nullptr, "Window creation failed", "GAME", MB_OK);

  if(!gladLoadGL())
    MessageBoxA(nullptr, "glad error", "GLAD", MB_OK);

  while(true) {
    if(PeekMessage(&msg, handle, 0, 0, PM_REMOVE) > 0) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
      SwapBuffers(opengldc);
    }
  }
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(openglrc);
  ReleaseDC(handle, opengldc);
  DestroyWindow(handle);
  return 0;
}

LRESULT CALLBACK winprocedure(HWND handle, UINT reason, WPARAM wp, LPARAM lp) {
  switch(reason) {
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProcA(handle, reason, wp, lp);
  }
}