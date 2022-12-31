#include "gui.hpp"

void GUI::Render() {
  // Render here

  // ShowExampleAppDockSpace_base();
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

  static int grid_size = 10;

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
    // // disable window padding
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    // // // set no scrollbars
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

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
    // ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 0.0f));

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
    // ImGui::PopStyleVar(3);
  }
}
