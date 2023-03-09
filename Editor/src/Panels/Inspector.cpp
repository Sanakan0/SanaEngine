#include "SCore/Global/ServiceLocator.h"
#include "SceneSys/SceneManager.h"
#include "imgui/imgui.h"
#include <SEditor/Panels/Inspector.h>
#include <string>
namespace SEditor::Panels{

Inspector::Inspector():
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{
    name_="Inspector";
}

void Inspector::RightClickMenu(){
    if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
    {
        ImGui::Text("This a popup!");
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void Inspector::DrawContent(){
    
    
}





}