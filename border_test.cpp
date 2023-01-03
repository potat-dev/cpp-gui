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

int main() {
  GLFWwindow* window;
  glfwSetErrorCallback(glfw_error_callback);

  // initialize the library
  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
  int count, windowWidth, windowHeight, monitorX, monitorY;

  // I am assuming that main monitor is in the 0 position
  GLFWmonitor** monitors = glfwGetMonitors(&count);
  const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);
  // width and height of the monitor
  windowWidth = static_cast<int>(videoMode->width);
  windowHeight = static_cast<int>(videoMode->height);

  glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

  // set the visibility window hint to false for subsequent window creation
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  // create a windowed mode window and its OpenGL context
  window =
      glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // make the window's context current
  glfwMakeContextCurrent(window);

  // reset the window hints to default
  glfwDefaultWindowHints();

  glfwSetWindowPos(window, monitorX, monitorY);

  // show the window
  glfwShowWindow(window);

  // uncomment following line to see the border of window
  glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  ImGui_ImplOpenGL3_Init("#version 130");

  // Our state
  bool show_demo_window = false;
  bool show_another_window = false;
  bool my_test_window = false;

  bool main_menu = true;

  // background color
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  while (!glfwWindowShouldClose(window) && main_menu) {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 0. Dockspace
    // ShowExampleAppDockSpace();
    // ShowExampleAppDockSpace_base();

    static int grid_size = 10;

    // 1. Show the big demo window
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves
    // We use a Begin/End pair to create a named window.
    if (main_menu) {
      static float f = 0.0f;
      static int counter = 0;

      // Create a window called "Hello, world!" and append into it.
      ImGui::Begin("Hello, world!", &main_menu);

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

      // slider for grid size
      ImGui::SliderInt("Grid Size", &grid_size, 1, 100);

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
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar |
                                      ImGuiWindowFlags_NoScrollWithMouse |
                                      ImGuiWindowFlags_NoResize;

      ImGui::Begin("Test Window", &my_test_window, window_flags);
      // set window size
      ImGui::SetWindowSize(ImVec2(500, 500 + 20));
      // disable window padding
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
      // set no scrollbars
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

      // ImGui::Text("Hello from test window!");
      // if (ImGui::Button("Close Me")) my_test_window = false;
      // color picker (square)
      // static float col[3] = {1.0f, 0.0f, 0.5f};
      // ImGui::ColorPicker3("MyColor##3", col);

      // square 2d table with height and width of grid_size squares
      static float grid[100][100] = {0.0f};

      // automaically calculate button size to fit grid
      // float button_size = 500.0f / grid_size;
      // for (int i = 0; i < grid_size; i++) {
      //   for (int j = 0; j < grid_size; j++) {
      //     ImGui::PushID(i * grid_size + j);
      //     ImGui::PushStyleColor(ImGuiCol_Button,
      //                           ImVec4(grid[i][j], 0.0f, 0.0f, 1.0f));
      //     ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
      //                           ImVec4(grid[i][j], 0.0f, 0.0f, 1.0f));
      //     ImGui::PushStyleColor(ImGuiCol_ButtonActive,
      //                           ImVec4(grid[i][j], 0.0f, 0.0f, 1.0f));
      //     if (ImGui::Button("##button", ImVec2(button_size, button_size))) {
      //       grid[i][j] += 0.1f;
      //     }
      //     ImGui::PopStyleColor(3);
      //     ImGui::PopID();
      //   }
      //   ImGui::NewLine();
      // }

      static ImGuiTableFlags flags = 0;

      // set cell padding to 0
      ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 0.0f));

      if (ImGui::BeginTable("table2", grid_size, flags, ImVec2(0.0f, 0.0f),
                            0.0f)) {
        // set column height to be same as width
        ImGui::TableSetupColumn("##column", ImGuiTableColumnFlags_WidthStretch,
                                0.0f, 0.0f);
        for (int cell = 0; cell < grid_size * grid_size; cell++) {
          ImGui::TableNextColumn();
          int column = ImGui::TableGetColumnIndex();
          int row = ImGui::TableGetRowIndex();

          ImGui::PushID(cell);
          char label[32];
          static char text_buf[32] = "";
          sprintf(label, "%d,%d", column, row);
          // ImGui::TextUnformatted(label);
          // get text size
          // ImVec2 text_size = ImGui::CalcTextSize(label);
          float w = ImGui::GetContentRegionAvail().x;
          // set height to be same as width
          ImGui::Button(label, ImVec2(w, w));

          // ImGui::Button(label, ImVec2(-FLT_MIN, 0.0f));
          // ImGui::Button(label, ImVec2(-FLT_MIN, -FLT_MIN));
          // set button height to be same as width

          ImGui::SameLine((float)column * ImGui::GetWindowContentRegionWidth() /
                              (float)grid_size,
                          0);

          ImGui::PopID();
        }
        ImGui::EndTable();
      }

      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    // // transparent background (test)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // // glClearColor(clear_color.x * clear_color.w, clear_color.y *
    // // clear_color.w,
    // //              clear_color.z * clear_color.w, clear_color.w);
    // // glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we
    // save/restore it to make it easier to paste this code elsewhere.
    // For this specific demo app we could also call
    // glfwMakeContextCurrent(window) directly)
    // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    //   GLFWwindow* backup_current_context = glfwGetCurrentContext();
    //   ImGui::UpdatePlatformWindows();
    //   ImGui::RenderPlatformWindowsDefault();
    //   glfwMakeContextCurrent(backup_current_context);
    // }

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