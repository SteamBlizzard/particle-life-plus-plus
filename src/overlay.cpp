#include "overlay.h"
#include "settings.h"
#include "configurations.h"

// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <format>
#include <iostream>
#include <sstream>
#include <string>

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

        std::vector<float> &forces = Configurations::GetForceValues(row);

        for (int column = 1; column < particleCount + 1; column++)
        {
          ImGui::TableSetColumnIndex(column);
          ImGui::PushID((row * MAXIMUM_PARTICLE_TYPES) + column);
          float wHeight = ImGui::GetFontSize() + style.FramePadding.y * 2;
          float yOffset = (CONFIG_MATRIX_CELL_HEIGHT - wHeight) * 0.5f;
          if (yOffset > 0)
          {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + yOffset);
          }
          ImGui::PushItemWidth(-FLT_MIN);
          ImGui::DragFloat("##force", &forces[column], 0.005f, -1.0f, 1.0f, "%.2f");
          ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(-std::min(0, (int)(255 * forces[column])), std::max(0, (int)(255 * forces[column])), 0, 255));
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
  }
  ImGui::End();
}