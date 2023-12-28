#include <windows.h>
#include "glad/glad.h"
#include "Renderer.hpp"

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

  Renderer WindowRenderer;

  WindowRenderer.InitContext(handle);
  WindowRenderer.InitShaders();

  if(handle == INVALID_HANDLE_VALUE)
    MessageBoxA(nullptr, "Window creation failed", "GAME", MB_OK);

  if(!gladLoadGL())
    MessageBoxA(nullptr, "glad error", "GLAD", MB_OK);

  WindowRenderer.InitPipeLine();

  while(true) {
    if(PeekMessage(&msg, handle, 0, 0, PM_REMOVE) > 0) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      if(msg.message == WM_QUIT)
        break;
    } else {
      glUseProgram(WindowRenderer.pShaderProgram);
      glBindVertexArray(WindowRenderer.pVAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
      SwapBuffers(WindowRenderer.GetGLDC());
    }
  } 
  glDeleteVertexArrays(1, &WindowRenderer.pVAO);
  glDeleteBuffers(1, &WindowRenderer.pVBO);
  glDeleteProgram(WindowRenderer.pShaderProgram);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(WindowRenderer.GetGLRC());
  ReleaseDC(handle, WindowRenderer.GetGLDC());
  DestroyWindow(handle);
  return 0;
}

LRESULT CALLBACK winprocedure(HWND handle, UINT reason, WPARAM wp, LPARAM lp) {
  LRESULT Result = 0;
  switch(reason) {
    default:
      Result = DefWindowProcA(handle, reason, wp, lp);
      break;
  }
  return Result;
}