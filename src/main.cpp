#include <windows.h>
#include <iostream>
#include "glad/glad.h"
#include "Renderer.hpp"

// Vertex Shader source
const char* vertexShaderSource = R"(
    attribute vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
)";

// Fragment Shader source
const char* fragmentShaderSource = R"(
    void main() {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
    }
)";

// Function to compile shader
GLuint compileShader(GLenum shaderType, const char* source) {
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  // Check for shader compilation errors
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
      GLchar infoLog[512];
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      std::cerr << "Shader compilation failed: " << infoLog << std::endl;
      return 0;
  }

  return shader;
}

// Function to create shader program
GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
  // Compile vertex shader
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  if (vertexShader == 0) {
      return 0;
  }

  // Compile fragment shader
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
  if (fragmentShader == 0) {
      return 0;
  }

  // Link shaders into a program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check for linking errors
  GLint success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
      GLchar infoLog[512];
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cerr << "Shader program linking failed: " << infoLog << std::endl;
      return 0;
  }

  // Clean up shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  LRESULT result = 0;
  switch (uMsg) {
      default:
        result = DefWindowProcA(hwnd, uMsg, wParam, lParam);
        break;
  }
  return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  // Register window class
  WNDCLASSA wc = {sizeof(WNDCLASSA)};
  wc.lpszClassName = "OpenGLWindow";
  wc.hInstance = hInstance;
  wc.lpfnWndProc = WindowProc;

  if(!RegisterClassA(&wc))
    MessageBoxA(nullptr, "Cannot Register Window", "", MB_OK);

  // Create window
  HWND hwnd = CreateWindowA(wc.lpszClassName, "OpenGL Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      100, 100, 800, 600, NULL, NULL, wc.hInstance, NULL);

  if(hwnd == INVALID_HANDLE_VALUE)
    MessageBoxA(nullptr, "Cannot create window", "", MB_OK);

  // Get device context
  HDC hdc = GetDC(hwnd);

  // Set pixel format
  PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR)};
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24; // 24-bit color buffer
  pfd.cDepthBits = 24; // 24-bit depth buffer
  pfd.cStencilBits = 8; // 8-bit stencil buffer
  int format = ChoosePixelFormat(hdc, &pfd);
  SetPixelFormat(hdc, format, &pfd);

  // Create OpenGL context
  HGLRC hglrc = wglCreateContext(hdc);
  wglMakeCurrent(hdc, hglrc);

  if(!gladLoadGL())
    MessageBoxA(nullptr, "GLAD LOAD ERROR", "", MB_OK);

  const GLubyte* version = glGetString(GL_VERSION);

  std::cout << "VERSION: " << version << "\n";
  // Create and compile shaders, link program
  GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

  // Vertices for a triangle
  GLfloat vertices[] = {
      -0.5f, -0.5f, 0.0f, // bottom left
      0.5f, -0.5f, 0.0f,  // bottom right
      0.0f, 0.5f, 0.0f    // top
  };

  // Create and bind VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Main loop
  MSG msg = {};
  while (true) {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
          if (msg.message == WM_QUIT)
              break;
      } else {
        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram);

        // Specify the layout of the vertex data
        GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers
        SwapBuffers(hdc);
    }
  }

  // Cleanup
  glDeleteProgram(shaderProgram);
  glDeleteBuffers(1, &VBO);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hglrc);
  ReleaseDC(hwnd, hdc);
  DestroyWindow(hwnd);
  UnregisterClassA("OpenGLWindow", hInstance);

  return 0;
}
