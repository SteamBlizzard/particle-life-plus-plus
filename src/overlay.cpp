#include "overlay.h"

// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <format>
#include <iostream>

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
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
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

void Overlay::ShowMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Windows")) {
            ImGui::MenuItem("Settings", NULL, &settingsMenuEnabled);
            ImGui::MenuItem("Configurations", NULL, &configurationMenuEnabled);
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
    if (ImGui::Begin("Settings")) {
        // TODO: Implement settings menu
        std::cout << "Showing settings menu!" << std::endl;
    }
    ImGui::End();
}