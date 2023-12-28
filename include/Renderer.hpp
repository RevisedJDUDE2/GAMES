#include <windows.h>
#include "glad\glad.h"

class Renderer {
  private:
    HGLRC pOpenGLRC;
    HDC pOpenGLDC;
  public:
    void InitContext(HWND& Window);
    HGLRC GetGLRC(void) const;
    HDC GetGLDC(void) const;
};