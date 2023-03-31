#include "SEditor/Panels/InfoPanel.h"
#include "imgui/imgui.h"
namespace SEditor::Panels{

void InfoPanel::DrawImpl(float deltat){
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner_ != -1)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (corner_ & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (corner_ & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (corner_ & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (corner_ & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Info overlay", &open_, window_flags))
    {

        ImGui::Text("Info overlay\n" "in the corner_ of the screen.\n" "(right-click to change position)");
        ImGui::Separator();
        ImGui::Text("FPS: %.1f",1.0f/deltat);
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom",       NULL, corner_ == -1)) corner_ = -1;
            if (ImGui::MenuItem("Top-left",     NULL, corner_ == 0)) corner_ = 0;
            if (ImGui::MenuItem("Top-right",    NULL, corner_ == 1)) corner_ = 1;
            if (ImGui::MenuItem("Bottom-left",  NULL, corner_ == 2)) corner_ = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner_ == 3)) corner_ = 3;
            if (ImGui::MenuItem("Close")) open_ = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

}