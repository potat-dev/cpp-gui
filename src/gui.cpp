#include "gui.hpp"

// for exceptions
#include <stdexcept>

using namespace std;

GUI::GUI() {
  SetupGraphics();
  SetupWindow();
  SetupImGui();
}

GUI::~GUI() {
  DestroyImGui();
  DestroyWindow();
  DestroyGraphics();
}

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void GUI::DecideGLSLVersion() {
#if defined(IMGUI_IMPL_OPENGL_ES2)
#pragma comment "GL ES 2.0 + GLSL 100"
  glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
#pragma comment "GL 3.2 + GLSL 150"
  glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
#pragma comment "GL 3.0 + GLSL 130"
  glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
}

void GUI::SetupGraphics() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) throw runtime_error("Failed to initialize GLFW");
  DecideGLSLVersion();
}

void GUI::SetupWindow() {
  window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
  if (window == NULL) throw runtime_error("Failed to initialize GLFW window");
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enable vsync
}

void GUI::SetupImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = ImGui::GetIO();
  (void)io;

  // Setup Dear ImGui style (dark)
  ImGui::StyleColorsDark();

  // Set Config Flags
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigDockingWithShift = true;

  // Multi-Viewport Platform Windows
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Load Fonts
  ImFont* font =
      io.Fonts->AddFontFromFileTTF("../misc/fonts/FiraCode-Retina.ttf", 18.0f);
  if (font == NULL) {
    io.Fonts->AddFontDefault();
    throw runtime_error("Could not load font file: Fira Code Retina");
  }

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

bool GUI::isRunning() { return !glfwWindowShouldClose(window); }

void GUI::DestroyImGui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void GUI::DestroyWindow() { glfwDestroyWindow(window); }

void GUI::DestroyGraphics() { glfwTerminate(); }

void GUI::BeginRender() {
  glfwPollEvents();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void GUI::EndRender() {
  ImGui::Render();  // Rendering

  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
               clear_color.z * clear_color.w, clear_color.w);

  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }

  glfwSwapBuffers(window);
}

void GUI::Update() {
  BeginRender();
  Render();
  EndRender();
}