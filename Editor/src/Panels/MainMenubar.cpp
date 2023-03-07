#include "imgui/imgui.h"
#include <SEditor/Panels/MainMenubar.h>
namespace SEditor::Panels{

void MainMenubar::DrawImpl(){
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("SanaEngine")){
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")){
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

}



}