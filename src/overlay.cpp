#include "overlay.h"
#include "settings.h"
#include "configurations.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

Overlay::Overlay()
{
}

Overlay::~Overlay()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Overlay::Init(GLFWwindow *window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  this->window = window;
}

void Overlay::Render()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow();

  if (mainMenuBarEnabled)
    ShowMainMenuBar();
  if (configurationMenuEnabled)
    ShowConfigurationMenu();
  if (settingsMenuEnabled)
    ShowSettingsMenu();
  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::HandleInput()
{
  if (ImGui::IsKeyPressed(ImGuiKey_Q) && ImGui::IsKeyDown(ImGuiMod_Ctrl))
  {
    settingsMenuEnabled = !settingsMenuEnabled;
  }
  if (ImGui::IsKeyPressed(ImGuiKey_W) && ImGui::IsKeyDown(ImGuiMod_Ctrl))
  {
    configurationMenuEnabled = !configurationMenuEnabled;
  }
}

void Overlay::ShowMainMenuBar()
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("Windows"))
    {
      ImGui::MenuItem("Settings", "CTRL+Q", &settingsMenuEnabled);
      ImGui::MenuItem("Configurations", "CTRL+W", &configurationMenuEnabled);
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void Overlay::ShowConfigurationMenu()
{
  // Generate an adjacency matrix of particles
  if (ImGui::Begin("Configuration Menu"))
  {
    ImGuiStyle &style = ImGui::GetStyle();
    ImVec2 originalCellPadding = style.CellPadding;
    ImVec2 originalFramePadding = style.FramePadding;
    style.CellPadding = ImVec2(0, 0);
    style.FramePadding = ImVec2(0, 0);
    ImGui::Text("Particle Life++ Configuration");
    ImGui::Separator();

    // Example configuration options
    static int particleCount = 1;
    ImGui::SliderInt("Particle Types", &particleCount, 1, 100);

    static ImGuiTableFlags flags = !ImGuiTableFlags_BordersOuter | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX;

    if (ImGui::BeginTable("Force Matrix", particleCount + 1, flags))
    {
      for (int i = 0; i < particleCount + 1; i++)
      {
        ImGui::TableSetupColumn(NULL, ImGuiTableColumnFlags_WidthFixed, CONFIG_MATRIX_CELL_WIDTH);
      }
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
      ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
      ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
      ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));
      ImGui::TableNextRow();
      for (int i = 0; i < particleCount; i++)
      {
        ImGui::TableSetColumnIndex(i + 1);
        ImGui::TextUnformatted(std::to_string(i).c_str());
      }
      for (int row = 0; row < particleCount; row++)
      {
        ImGui::TableNextRow(ImGuiTableRowFlags_None, CONFIG_MATRIX_CELL_HEIGHT);
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted(std::to_string(row).c_str());
        
        if (ImGui::Button(std::format("Customize##{}", row).c_str()))
          ImGui::OpenPopup(std::format("ColorPicker##{}", row).c_str());

        if (ImGui::BeginPopupModal(std::format("ColorPicker##{}", row).c_str())) {
            ImGui::ColorPicker4(
                std::format("Select color for particle type {}", row).c_str(),
                glm::value_ptr(Configurations::particleColors[row]),
                ImGuiWindowFlags_AlwaysAutoResize
            );
            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        // Doesn't work for some reason. TODO: Fix to make header cells colored.
        // glm::vec4 color = Configurations::particleColors[row];
        // ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(color.r, color.g, color.b, color.a));

        for (int column = 0; column < particleCount; column++)
        {
          ImGui::TableSetColumnIndex(column + 1);
          ImGui::PushID((row * MAXIMUM_PARTICLE_TYPES) + column);
          float wHeight = ImGui::GetFontSize() + style.FramePadding.y * 2;
          float yOffset = (CONFIG_MATRIX_CELL_HEIGHT - wHeight) * 0.5f;
          if (yOffset > 0)
          {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + yOffset);
          }
          ImGui::PushItemWidth(-FLT_MIN);
          float &force = Configurations::GetForceValue(row, column);
          ImGui::DragFloat("##force", &force, 0.005f, -1.0f, 1.0f, "%.2f");
          ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(-std::min(0, (int)(255 * force)), std::max(0, (int)(255 * force)), 0, 255));
          ImGui::PopItemWidth();
          ImGui::PopID();
        }
      }
      ImGui::PopStyleColor(3);
      ImGui::PopStyleVar();
      ImGui::EndTable();
    }
    style.CellPadding = originalCellPadding;
    style.FramePadding = originalFramePadding;

    ImGui::Separator();
    ImGui::Text("Friction Coefficient");
    ImGui::Text("0.0 (Static)");
    ImGui::SameLine();
    ImGui::DragFloat("1.0 (No Friction)", &Configurations::friction, 0.005f, 0.0f, 1.0f);

    ImGui::DragFloat("Particle Size", &Configurations::particleRadius, 1.0f, 1.0f, 200.0f);

    ImGui::DragFloat("Force Multiplier", &Configurations::forceMultiplier, 0.1f, 0.0f, 100.0f);
  }
  ImGui::End();
}

void Overlay::ShowSettingsMenu()
{
  static int currentResolution = 0;
  if (ImGui::Begin("Settings"))
  {
    std::pair<int, int> currentRes = Settings::RESOLUTIONS[currentResolution];
    if (ImGui::BeginCombo("Resolution", std::format("{}x{}", currentRes.first, currentRes.second).c_str()))
    {
      for (int i = 0; i < Settings::RESOLUTIONS.size(); i++)
      {
        bool selected = currentResolution == i;
        std::pair res = Settings::RESOLUTIONS[i];
        if (ImGui::Selectable(std::format("{}x{}", res.first, res.second).c_str(), selected))
        {
          currentResolution = i;
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("Apply Resolution") && Settings::GetResolution(window) != Settings::RESOLUTIONS[currentResolution])
    {
      Settings::SetResolution(window, Settings::RESOLUTIONS[currentResolution]);
    }

    static int currentDisplayMode = 0;
    std::string currentDisplay = Settings::DISPLAY_MODES[currentDisplayMode];
    if (ImGui::BeginCombo("Window Type", currentDisplay.c_str()))
    {
      for (int i = 0; i < Settings::DISPLAY_MODES.size(); i++)
      {
        bool selected = currentDisplayMode == i;
        std::string displayMode = Settings::DISPLAY_MODES[i];
        if (ImGui::Selectable(displayMode.c_str(), selected))
        {
          currentDisplayMode = i;
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("Apply Display Mode") && Settings::GetDisplayMode(window) != Settings::DISPLAY_MODES[currentDisplayMode])
    {
      Settings::setDisplayMode(window, Settings::DISPLAY_MODES[currentDisplayMode]);
    }

    ImGui::SliderInt("Threads", &Settings::threadCount, 1, std::thread::hardware_concurrency());

    static bool vsync;
    ImGui::Checkbox("VSync Enabled", &vsync);
    if (vsync && !Settings::vsync)
    {
      glfwSwapInterval(1);
      Settings::vsync = true;
    }
    else if (!vsync && Settings::vsync)
    {
      glfwSwapInterval(0);
      Settings::vsync = false;
    }
  }
  ImGui::End();
}