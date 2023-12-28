#include <windows.h>
#include "glad\glad.h"

class Renderer {
  private:
    HGLRC pOpenGLRC;
    HDC pOpenGLDC;
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
  public:
    void InitContext(HWND& Window);
    HGLRC GetGLRC(void) const;
    HDC GetGLDC(void) const;
    void InitPipeLine(void);
    void InitShaders(void);
    unsigned int pVAO;
    unsigned int pShaderProgram;
    unsigned int pVBO;
};