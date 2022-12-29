// Dear ImGui: standalone example application for GLFW + OpenGL 3

#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GL_SILENCE_DEPRECATION

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a
// merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort)) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

// Basic Dockspace
void ShowExampleAppDockSpace_base() {
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

// Advanced Dockspace (with DockSpaceOverViewport + DockBuilder)
void ShowExampleAppDockSpace() {
  // If you strip some features of, this demo is pretty much equivalent to
  // calling DockSpaceOverViewport()! In most cases you should be able to just
  // call DockSpaceOverViewport() and ignore all the code below! In this
  // specific demo, we are not using DockSpaceOverViewport() because:
  // - we allow the host window to be floating/moveable instead of filling the
  // viewport (when opt_fullscreen == false)
  // - we allow the host window to have padding (when opt_padding == true)
  // - we have a local menu bar in the host window (vs. you could use
  // BeginMainMenuBar() + DockSpaceOverViewport() in your code!) TL;DR; this
  // demo is more complicated than what you would normally use. If we removed
  // all the options we are showcasing, this demo would become:
  //     void ShowExampleAppDockSpace()
  //     {
  //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  //     }

  // static bool opt_fullscreen = true;
  // static bool opt_padding = false;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
  // not dockable into, because it would be confusing to have two docking
  // targets within each others.
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  // if (opt_fullscreen) {
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  // } else {
  //   dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
  // }

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render
  // our background and handle the pass-thru hole, so we ask Begin() to not
  // render a background.
  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  // Important: note that we proceed even if Begin() returns false (aka window
  // is collapsed). This is because we want to keep our DockSpace() active. If a
  // DockSpace() is inactive, all active windows docked into it will lose their
  // parent and become undocked. We cannot preserve the docking relationship
  // between an active window and an inactive docking, otherwise any change of
  // dockspace/settings would lead to windows being stuck in limbo and never
  // being visible.
  // if (!opt_padding)
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace Demo", NULL, window_flags);
  // if (!opt_padding)
  ImGui::PopStyleVar();

  // if (opt_fullscreen)
  ImGui::PopStyleVar(2);

  // Submit the DockSpace
  ImGuiIO& io = ImGui::GetIO();
  // if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  // } else {
  //   ShowDockingDisabledMessage();
  // }

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Options")) {
      // Disabling fullscreen would allow the window to be moved to the front of
      // other windows, which we can't undo at the moment without finer window
      // depth/z control.
      // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
      // ImGui::MenuItem("Padding", NULL, &opt_padding);
      // ImGui::Separator();

      if (ImGui::MenuItem(
              "Flag: NoSplit", "",
              (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
      }
      if (ImGui::MenuItem(
              "Flag: NoResize", "",
              (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
      }
      if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "",
                          (dockspace_flags &
                           ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
      }
      if (ImGui::MenuItem(
              "Flag: AutoHideTabBar", "",
              (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
      }
      if (ImGui::MenuItem("Flag: PassthruCentralNode", "",
                          (dockspace_flags &
                           ImGuiDockNodeFlags_PassthruCentralNode) != 0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
      }
      ImGui::Separator();
      // if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
      // *p_open = false;
      ImGui::EndMenu();
    }
    HelpMarker(
        "When docking is enabled, you can ALWAYS dock MOST window into "
        "another! Try it now!\n"
        "- Drag from window title bar or their tab to dock/undock.\n"
        "- Drag from window menu button (upper-left button) to undock an "
        "entire node (all windows).\n"
        "- Hold SHIFT to disable docking (if io.ConfigDockingWithShift == "
        "false, default)\n"
        "- Hold SHIFT to enable docking (if io.ConfigDockingWithShift == "
        "true)\n"
        "This demo app has nothing to do with enabling docking!\n\n"
        "This demo app only demonstrate the use of ImGui::DockSpace() which "
        "allows you to manually create a docking node _within_ another "
        "window.\n\n"
        "Read comments in ShowExampleAppDockSpace() for more details.");

    ImGui::EndMenuBar();
  }
  ImGui::End();
}

// Main code
int main(int, char**) {
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return 1;

// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
#pragma comment "GL ES 2.0 + GLSL 100"
  const char* glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
#pragma comment "GL 3.2 + GLSL 150"
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
#pragma comment "GL 3.0 + GLSL 130"
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

  // Create window with graphics context
  GLFWwindow* window = glfwCreateWindow(
      1280, 720, "Dear ImGui GLFW + OpenGL3 example", NULL, NULL);

  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window\n");
    return 1;
  }

  // set window borderless
  // glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  // Set Config Flags
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Keyboard Controls
  io.ConfigDockingWithShift = true;

  // Multi-Viewport Platform Windows
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // Setup Dear ImGui style (dark)
  ImGui::StyleColorsDark();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Load Fonts
  ImFont* font =
      io.Fonts->AddFontFromFileTTF("../misc/fonts/FiraCode-Retina.ttf", 18.0f);
  if (font == NULL) {
    fprintf(stderr, "Could not load font file: FiraCode-Retina\n");
    io.Fonts->AddFontDefault();
  }

  // Our state
  bool show_demo_window = false;
  bool show_another_window = false;
  bool my_test_window = false;

  // background color
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application, or clear/overwrite your copy of the
    // keyboard data. Generally you may always pass all inputs to dear imgui,
    // and hide them from your application based on those two flags.
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 0. Dockspace
    // ShowExampleAppDockSpace();
    ShowExampleAppDockSpace_base();

    // 1. Show the big demo window
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves
    // We use a Begin/End pair to create a named window.
    {
      static float f = 0.0f;
      static int counter = 0;

      // Create a window called "Hello, world!" and append into it.
      ImGui::Begin("Hello, world!");

      // Display some text (you can use a format strings too)
      ImGui::Text("This is some useful text.");

      // Edit bools storing our window open/close state
      ImGui::Checkbox("Demo Window", &show_demo_window);
      ImGui::Checkbox("Another Window", &show_another_window);
      ImGui::Checkbox("My Test Window", &my_test_window);

      // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

      // Edit 3 floats representing a color
      ImGui::ColorEdit3("clear color", (float*)&clear_color);

      // Buttons return true when clicked
      // (most widgets return true when edited/activated)
      if (ImGui::Button("Button")) counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window) {
      // Pass a pointer to our bool variable
      // window will have a closing button that will clear bool when clicked
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me")) show_another_window = false;
      ImGui::End();
    }

    // 4. Show a test window
    if (my_test_window) {
      ImGui::Begin("Test Window", &my_test_window);
      ImGui::Text("Hello from test window!");
      if (ImGui::Button("Close Me")) my_test_window = false;
      // color picker (square)
      static float col[3] = {1.0f, 0.0f, 0.5f};
      ImGui::ColorPicker3("MyColor##3", col);
      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we
    // save/restore it to make it easier to paste this code elsewhere.
    // For this specific demo app we could also call
    // glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}