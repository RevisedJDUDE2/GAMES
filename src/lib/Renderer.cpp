#include "Renderer.hpp"

void Renderer::InitContext(HWND& Window) {
  HDC OpenGLDC = GetDC(Window);
  PIXELFORMATDESCRIPTOR PixelFormat = {sizeof(PIXELFORMATDESCRIPTOR)};

  PixelFormat.nVersion = 1;
  PixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
  PixelFormat.iPixelType = PFD_TYPE_RGBA;
  PixelFormat.cColorBits = 16;

  int Format = ChoosePixelFormat(OpenGLDC, &PixelFormat);
  SetPixelFormat(OpenGLDC, Format, &PixelFormat);

  HGLRC OpenGLRC = wglCreateContext(OpenGLDC);
  wglMakeCurrent(OpenGLDC, OpenGLRC);

  this->pOpenGLDC = OpenGLDC;
  this->pOpenGLRC = OpenGLRC;
}

HDC Renderer::GetGLDC(void) const {
  return this->pOpenGLDC;
}

HGLRC Renderer::GetGLRC(void) const {
  return this->pOpenGLRC;
};