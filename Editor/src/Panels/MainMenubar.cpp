#include "ECS/Component/CameraComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SEditor/Core/AssetLoader.h"
#include "SEditor/Util/NfdDialog.h"
#include "SceneSys/SceneManager.h"
#include "imgui/imgui.h"
#include <SEditor/Panels/MainMenubar.h>
namespace SEditor::Panels{

MainMenubar::MainMenubar():
assetloader_(SANASERVICE(Core::AssetLoader)),
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{

}

void MainMenubar::DrawImpl(float deltat){
    
    int loadtileopen=0;
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("SanaEngine")){
            if (ImGui::MenuItem("Load tile")){
                loadtileopen=1;
            }
            
            if (ImGui::BeginMenu("Add Actor")){
                if (ImGui::MenuItem("Add Camera")){
                    auto& tmpa=scenemanager_.GetScene()->CreateActor("Camera");
                    tmpa.AddComponent<ECS::Components::CameraComponent>();
                    tmpa.AddComponent<ECS::Components::TransformComponent>();
                    tmpa.GetTransformComponent()->trans_.world_pos_=scenemanager_.cursor_pos_;
                    scenemanager_.SetActiveCamera(tmpa.GetID());
                    scenemanager_.SetSelectedActor(tmpa.GetID());
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")){
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (loadtileopen)
        ImGui::OpenPopup("Loadtile?");

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));



    if (ImGui::BeginPopupModal("Loadtile?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("LOAD TILE!\n\n");
        ImGui::Separator();

        //static int unused_i = 0;
        //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

        static int st=0;
        static int cnt=60;
        ImGui::InputInt("st", &st);
        ImGui::InputInt("cnt", &cnt);
                    
        // static bool dont_ask_me_next_time = false;
        // ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        // ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
        // ImGui::PopStyleVar();

        static std::string tilefolder="";
        if(ImGui::Button("Folder...")){
            tilefolder = Util::NfdDialog::OpenFolderDlg();
        }
        ImGui::SameLine();
        ImGui::Text(("pth: "+tilefolder).c_str());
        
        const char* internalformats[] = { "Default","DXT5"};
        const SRender::Resources::TexInternalFormat formatvalues[]={SRender::Resources::TexInternalFormat::Default,SRender::Resources::TexInternalFormat::DXT5};
        static int formatid=0;
        static SRender::Resources::ModelLoadSetting loadsetting;

        ImGui::Combo("TextureCompress", &formatid, internalformats, IM_ARRAYSIZE(internalformats));
        loadsetting.texinfo.internal=formatvalues[formatid];
        if (ImGui::Button("OK", ImVec2(120, 0))) { 
            
            if (tilefolder!=""){
                assetloader_.LoadTiles(tilefolder,st,cnt,loadsetting);
            }
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

}



}