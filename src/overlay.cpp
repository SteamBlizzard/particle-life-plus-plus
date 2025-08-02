#include "overlay.h"
#include "settings.h"

// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <format>
#include <iostream>
#include <sstream>

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
    // ImGui::ShowDemoWindow();

    if (mainMenuBarEnabled)         ShowMainMenuBar();
    if (configurationMenuEnabled)   ShowConfigurationMenu();
    if (settingsMenuEnabled)        ShowSettingsMenu();
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::HandleInput() {
    if (ImGui::IsKeyPressed(ImGuiKey_Q) && ImGui::IsKeyDown(ImGuiMod_Ctrl)) {
        settingsMenuEnabled = !settingsMenuEnabled;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_W) && ImGui::IsKeyDown(ImGuiMod_Ctrl)) {
        configurationMenuEnabled = !configurationMenuEnabled;
    }
}

void Overlay::ShowMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Windows")) {
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
        ImGui::Text("Particle Life++ Configuration");
        ImGui::Separator();

        // Example configuration options
        static int particleCount = 1;
        ImGui::SliderInt("Particle Types", &particleCount, 1, 100);

        ImGui::Separator();
        ImGui::BeginTable("Configuration Options", particleCount);
        ImGui::EndTable();
        
    }
    ImGui::End();
}

void Overlay::ShowSettingsMenu() {
    static int currentResolution = 0;
    if (ImGui::Begin("Settings")) {
        std::pair<int,int> currentRes = Settings::RESOLUTIONS[currentResolution];
        if (ImGui::BeginCombo("Resolution", std::format("{}x{}", currentRes.first, currentRes.second).c_str())) {
            for (int i = 0; i < Settings::RESOLUTIONS.size(); i++) {
                bool selected = currentResolution == i;
                std::pair res = Settings::RESOLUTIONS[i];
                if (ImGui::Selectable(std::format("{}x{}", res.first, res.second).c_str(), selected)) {
                    currentResolution = i;
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Apply Resolution") && Settings::GetResolution(window) != Settings::RESOLUTIONS[currentResolution]) {
            Settings::SetResolution(window, Settings::RESOLUTIONS[currentResolution]);
        }

        static int currentDisplayMode = 0;
        std::string currentDisplay = Settings::DISPLAY_MODES[currentDisplayMode];
        if (ImGui::BeginCombo("Window Type", currentDisplay.c_str())) {
            for (int i = 0; i < Settings::DISPLAY_MODES.size(); i++) {
                bool selected = currentDisplayMode == i;
                std::string displayMode = Settings::DISPLAY_MODES[i];
                if (ImGui::Selectable(displayMode.c_str(), selected)) {
                    currentDisplayMode = i;
                }
            }
            ImGui::EndCombo();
        }
        
        if (ImGui::Button("Apply Display Mode") && Settings::GetDisplayMode(window) != Settings::DISPLAY_MODES[currentDisplayMode]) {
            Settings::setDisplayMode(window, Settings::DISPLAY_MODES[currentDisplayMode]);
        }
    }
    ImGui::End();
}