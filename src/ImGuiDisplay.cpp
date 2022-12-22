#include "ImGuiDisplay.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "globals.h"

static const char* planet_focus[]{"Sun","Mercury", "Venus", "Earth", "Moon", "Mars"};


void buildImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Diorama");
    ImGui::SetWindowFocus();
    {
        ImGui::Text("Controls");
        ImGui::Checkbox("Debug Mode", &debug_mode);

        ImGui::Combo("Planetary Focus", &selected_planet, planet_focus, IM_ARRAYSIZE(planet_focus));
        if(pause)
        {
            if(ImGui::Button("Play"))
                pause = !pause;
        }
        else
        {
            if(ImGui::Button("Pause"))
                pause = !pause;
        }
        ImGui::SameLine();
        if(ImGui::Button("Restart")){
            glfwtime = 0.f;
        }

        ImGui::SliderFloat("Animation Speed",&animationSpeed,0.1f,10.f, "%.1f", 0);

        if(debug_mode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE /*GL_LINE*/);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL /*GL_LINE*/);
        }
    }
    ImGui::End();
}