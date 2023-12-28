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

void Renderer::InitPipeLine(void) {
  float vertices[] = {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f,  0.5f, 0.0f
  };  
  unsigned int VAO, VBO;
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  this->pVBO = VBO;
}

void Renderer::InitShaders() {
  unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(VertexShader, 1, &this->vertexShaderSource, NULL);
  glCompileShader(VertexShader);

  unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(FragmentShader, 1, &this->fragmentShaderSource, NULL);
  glCompileShader(FragmentShader);

  unsigned int ShaderProgram = glCreateProgram();
  glAttachShader(ShaderProgram, VertexShader);
  glAttachShader(ShaderProgram, FragmentShader);
  glLinkProgram(ShaderProgram);

  glDeleteShader(VertexShader);
  glDeleteShader(FragmentShader);
  this->pShaderProgram = ShaderProgram;
}