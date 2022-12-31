/*
SetupGraphics
SetupWindow
SetupImGui

while (isRunning) {
  BeginRender
  Render
  EndRender
}

DestroyImGui
DestroyWindow
DestroyGraphics
*/

#include <GLFW/glfw3.h>
#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

class GUI {
 private:
  //  TODO: make these configurable (settings class)

  constexpr static int WIDTH = 1280;
  constexpr static int HEIGHT = 720;
  const char* TITLE = "Dear ImGui GLFW + OpenGL3 example";

  GLFWwindow* window;
  ImGuiStyle style;
  ImGuiIO io;

  const char* glsl_version = NULL;
  const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // render variables
  bool show_demo_window = false;
  bool show_another_window = false;
  bool my_test_window = false;

  // setup functions
  void SetupGraphics();
  void SetupWindow();
  void SetupImGui();

  void BeginRender();
  void Render();
  void EndRender();

  void DestroyImGui();
  void DestroyWindow();
  void DestroyGraphics();

  // helper functions
  void DecideGLSLVersion();

 public:
  GUI();
  ~GUI();

  // main loop
  bool isRunning();
  void Update();
};
