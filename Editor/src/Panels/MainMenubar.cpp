#include "imgui/imgui.h"
#include "nfd.h"
#include <SEditor/Panels/MainMenubar.h>
namespace SEditor::Panels{

void MainMenubar::DrawImpl(){
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("SanaEngine")){
            if (ImGui::MenuItem("Load tile")){
                nfdchar_t* output = NULL;
                NFD_OpenDialog(NULL, NULL, &output);
                free(output);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")){
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

}



}