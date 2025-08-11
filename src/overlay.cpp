#include "plpp/overlay.h"

// Project Includes
#include "plpp/settings.h"

// External Libraries
#include <glm/glm.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>

// C++ Standard Library
#include <format>
#include <iostream>
#include <string>

namespace PLPP
{
  Overlay::Overlay(GLFWwindow *window, PhysicsEngine &physicsEngine_)
      : window(window), physicsEngine_(physicsEngine_)
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    #ifndef NDEBUG
    io.ConfigDebugHighlightIdConflicts = false;
    #endif
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 440 core");
  }

  Overlay::~Overlay()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void Overlay::Render()
  {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    if (mainMenuBarEnabled)
      showMainMenuBar();
    if (settingsAndConfigsMenuEnabled)
      showSettingsAndConfigsMenu();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void Overlay::showMainMenuBar()
  {
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("Windows"))
      {
        ImGui::MenuItem("Settings & Configurations", "CTRL+Q", &settingsAndConfigsMenuEnabled);
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }

  void Overlay::showSettingsAndConfigsMenu()
  {
    if (ImGui::Begin("Settings & Configs"))
    {
      if (ImGui::BeginTabBar("Tabs"))
      {
        settingsMenu();
        configurationMenu();
        ImGui::EndTabBar();
      }
    }
    ImGui::End();
  }

  void Overlay::configurationMenu()
  {
    // Generate an adjacency matrix of particles
    if (ImGui::BeginTabItem("Configurations"))
    {
      ImGuiStyle &style = ImGui::GetStyle();
      ImVec2 originalCellPadding = style.CellPadding;
      ImVec2 originalFramePadding = style.FramePadding;
      style.CellPadding = ImVec2(0, 0);
      style.FramePadding = ImVec2(0, 0);
      ImGui::Text("Particle Life++ Configuration");
      ImGui::Separator();

      ImGui::SliderInt("Particle Types", &typeCount_, 1, 100);

      ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX;

      if (ImGui::BeginTable("Force Matrix", typeCount_ + 1, flags))
      {
        // Styling initialization
        for (int i = 0; i < typeCount_ + 1; i++)
        {
          ImGui::TableSetupColumn(NULL, ImGuiTableColumnFlags_WidthFixed, CONFIG_MATRIX_CELL_WIDTH);
        }
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));

        // Header Color Pickers
        ImGui::TableNextRow();
        for (int i = 0; i < typeCount_; i++)
        {
          ImGui::TableSetColumnIndex(i + 1);
          drawCircle(i);
          if (ImGui::BeginPopupModal(std::format("Particle Color Picker##{}", i).c_str(), nullptr, ImGuiPopupFlags_NoReopen))
          {
            ImGui::ColorPicker4(
                std::format("Select color for particle type {}", i).c_str(),
                glm::value_ptr(physicsEngine_.particleColors[i]),
                ImGuiWindowFlags_AlwaysAutoResize);
            if (ImGui::Button("Close"))
            {
              physicsEngine_.UpdateColors();
              ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
          }
        }

        for (int row = 0; row < typeCount_; row++)
        {
          // Column Color Pickers
          ImGui::TableNextRow(ImGuiTableRowFlags_None, CONFIG_MATRIX_CELL_HEIGHT);
          ImGui::TableSetColumnIndex(0);
          drawCircle(row);

          // Force values
          for (int column = 0; column < typeCount_; column++)
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
            float &force = physicsEngine_.GetForcesBuffer()[row * MAXIMUM_PARTICLE_TYPES + column];
            ImGui::DragFloat("##force", &force, 0.005f, -1.0f, 1.0f, "%.2f");
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(-std::min(0, static_cast<int>(255 * force)), std::max(0, static_cast<int>(255 * force)), 0, 255));
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
      ImGui::DragFloat("1.0 (No Friction)", &physicsEngine_.friction, 0.005f, 0.0f, 1.0f);

      ImGui::DragFloat("Particle Size", &physicsEngine_.particleRadius, 1.0f, 1.0f, 200.0f);
      ImGui::DragFloat("Particle Maximum Affected Radius", &physicsEngine_.gravityRadiusOuter, 5.0f, 1.0f, 500.0f);
      ImGui::DragFloat("Particle Minimum Neighbor Distance", &physicsEngine_.gravityRadiusInner, 5.0f, 1.0f, 500.0f);
      if (physicsEngine_.gravityRadiusInner > physicsEngine_.gravityRadiusOuter)
        physicsEngine_.gravityRadiusInner = physicsEngine_.gravityRadiusOuter;

      ImGui::DragFloat("Force Multiplier", &physicsEngine_.forceMultiplier, 0.1f, 0.0f, 100.0f);
      ImGui::EndTabItem();
    }
  }

  void Overlay::settingsMenu()
  {
    if (ImGui::BeginTabItem("Settings"))
    {
      ImGui::Text(std::format("Particle Count: {}", physicsEngine_.particleCount).c_str());
      std::pair<int, int> currentRes = Settings::RESOLUTIONS[currentResolution_];
      if (ImGui::BeginCombo("Resolution", std::format("{}x{}", currentRes.first, currentRes.second).c_str()))
      {
        for (int i = 0; i < Settings::RESOLUTIONS.size(); i++)
        {
          bool selected = currentResolution_ == i;
          std::pair res = Settings::RESOLUTIONS[i];
          if (ImGui::Selectable(std::format("{}x{}", res.first, res.second).c_str(), selected))
          {
            currentResolution_ = i;
          }
        }
        ImGui::EndCombo();
      }

      if (ImGui::Button("Apply Resolution") && Settings::GetResolution(window) != Settings::RESOLUTIONS[currentResolution_])
      {
        Settings::SetResolution(window, Settings::RESOLUTIONS[currentResolution_]);
      }

      std::string currentDisplay = Settings::DISPLAY_MODES[currentDisplayMode_];
      if (ImGui::BeginCombo("Window Type", currentDisplay.c_str()))
      {
        for (int i = 0; i < Settings::DISPLAY_MODES.size(); i++)
        {
          bool selected = currentDisplayMode_ == i;
          std::string displayMode = Settings::DISPLAY_MODES[i];
          if (ImGui::Selectable(displayMode.c_str(), selected))
          {
            currentDisplayMode_ = i;
          }
        }
        ImGui::EndCombo();
      }

      if (ImGui::Button("Apply Display Mode") && Settings::GetDisplayMode(window) != Settings::DISPLAY_MODES[currentDisplayMode_])
      {
        Settings::setDisplayMode(window, Settings::DISPLAY_MODES[currentDisplayMode_]);
      }

      ImGui::Checkbox("VSync Enabled", &vsync_);
      if (vsync_ && !Settings::vsync)
      {
        glfwSwapInterval(1);
        Settings::vsync = true;
      }
      else if (!vsync_ && Settings::vsync)
      {
        glfwSwapInterval(0);
        Settings::vsync = false;
      }
      ImGui::EndTabItem();
    }
  }

  void Overlay::drawCircle(int particleId)
  {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(
        pos.x + CONFIG_MATRIX_CELL_WIDTH * 0.5f,
        pos.y + CONFIG_MATRIX_CELL_HEIGHT * 0.5f
    );
    float radius = CONFIG_MATRIX_CELL_WIDTH * 0.5f;
    
    // Get the draw list and draw the circle
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    glm::vec4 color = physicsEngine_.particleColors[particleId];
    draw_list->AddCircleFilled(center, radius, IM_COL32(color.r * 255, color.g * 255, color.b * 255, color.a * 255));

    if (ImGui::InvisibleButton(std::format("Color##{}", particleId).c_str(), ImVec2(CONFIG_MATRIX_CELL_WIDTH, CONFIG_MATRIX_CELL_HEIGHT)))
      ImGui::OpenPopup(std::format("Particle Color Picker##{}", particleId).c_str());
  }
}