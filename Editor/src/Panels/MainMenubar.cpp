#include "ECS/Component/CameraComponent.h"
#include "ECS/Component/DatasetGenComponent.h"
#include "ECS/Component/RectifyComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SEditor/Core/AssetLoader.h"
#include "SEditor/Util/NfdDialog.h"
#include "SGUI/Core/UImanager.h"
#include "SEditor/Panels/CameraView.h"
#include "SEditor/Panels/DistortionRectiferPanel.h"
#include "SEditor/Panels/Hierarchy.h"
#include "SEditor/Panels/InfoPanel.h"
#include "SEditor/Panels/Inspector.h"
#include "SEditor/Panels/MainMenubar.h"
#include "SEditor/Panels/TestView.h"
#include "SEditor/Panels/VisLocPanel.h"
#include "SResourceManager/Util.h"
#include "SceneSys/SceneManager.h"
#include "imgui/imgui.h"
#include "spdlog/spdlog.h"
#include "tinyxml2.h"
#include <SEditor/Panels/MainMenubar.h>
#include <filesystem>
#include <fstream>
#include <vector>
namespace SEditor::Panels{

MainMenubar::MainMenubar():
assetloader_(SANASERVICE(Core::AssetLoader)),
scenemanager_(SANASERVICE(SceneSys::SceneManager)),
uimanager_(SANASERVICE(SGUI::Core::UImanager))
{

}

void MainMenubar::OpenScene(const std::string& pth){
    scenemanager_.LoadScene(pth);
}

void MainMenubar::SaveCurrentScene(const std::string& pth){
    tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* node = doc.NewElement("root");
    doc.InsertFirstChild(node);
    scenemanager_.GetScene()->Serialize(doc, node);
    doc.SaveFile(pth.c_str());
    std::ofstream out(std::filesystem::u8path(pth));
    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    out << std::string(printer.CStr());
}

void MainMenubar::GetPanelsOpenFlag(){
    show_scene_view_=  &uimanager_.GetPanel<SEditor::Panels::SceneView>("Scene View").opened_;
    show_test_view_=&uimanager_.GetPanel<SEditor::Panels::TestView>("Test View").opened_;
    show_camera_view_=&uimanager_.GetPanel<Panels::CameraView>("Camera View").opened_;
    show_hierarchy_=&uimanager_.GetPanel<Panels::Hierarchy>("Hierarchy").opened_;
    show_inspector_=&uimanager_.GetPanel<Panels::Inspector>("Inspector").opened_;
    show_vislocpanel_=&uimanager_.GetPanel<Panels::VisLocPanel>("VisLocPanel").opened_;
    show_distortionrecifierpanel_=&uimanager_.GetPanel<Panels::DistrotionRectifierPanel>("DistrotionRectifierPanel").opened_;
    show_infopanel_ = &uimanager_.GetPanel<Panels::InfoPanel>("InfoPanel").open_;
}

void MainMenubar::DrawImpl(float deltat){
    static bool first_init=true;
    if (first_init){
        GetPanelsOpenFlag();
        first_init=false;
    }

    int loadtileopen=0;
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("文件")){
            static std::vector<nfdfilteritem_t> filters{{"scene","sanascene"}};
            if (ImGui::MenuItem("open","O")){
                
                auto filepth = Util::NfdDialog::OpenFileDlg(filters,ResourceManager::PathManager::GetProjectPath());
                OpenScene(filepth);

            }
            if (ImGui::MenuItem("Save As","CTRL + SHIFT + S")){
                
                auto filepth = Util::NfdDialog::SaveDlg(filters,"New Scene",ResourceManager::PathManager::GetProjectPath());
                SaveCurrentScene(filepth);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("编辑")){
            if (ImGui::MenuItem("Load tile")){
                loadtileopen=1;
            }
            
            if (ImGui::BeginMenu("Add Actor")){
                if (ImGui::MenuItem("Add Camera")){
                    auto& tmpa=scenemanager_.GetScene()->CreateActor("Camera");
                    tmpa.AddComponent<ECS::Components::CameraComponent>();
                    tmpa.AddComponent<ECS::Components::TransformComponent>();
                    tmpa.GetTransformComponent()->trans_.SetPosW(scenemanager_.cursor_pos_);
                    scenemanager_.SetActiveCamera(tmpa.GetID());
                    scenemanager_.SetSelectedActor(tmpa.GetID());
                }
                if (ImGui::MenuItem("Add Rectifier")){
                    auto& tmpa=scenemanager_.GetScene()->CreateActor("Distortion Rectifier");
                    tmpa.AddComponent<ECS::Components::RectifyComponent>();
                    tmpa.AddComponent<ECS::Components::CameraComponent>();
                    tmpa.AddComponent<ECS::Components::TransformComponent>();
                    //tmpa.GetTransformComponent()->trans_.SetPosW(scenemanager_.cursor_pos_);
                    scenemanager_.SetSelectedActor(tmpa.GetID());
                }
                if (ImGui::MenuItem("Light")){
                    auto& tmpa=scenemanager_.GetScene()->CreateActor("Light");
                    tmpa.AddComponent<ECS::Components::LightComponent>();
                    tmpa.AddComponent<ECS::Components::TransformComponent>();
                    //tmpa.GetTransformComponent()->trans_.SetPosW(scenemanager_.cursor_pos_);
                    scenemanager_.SetSelectedActor(tmpa.GetID());
                }
                if (ImGui::MenuItem("DatasetGen")){
                    auto& tmpa=scenemanager_.GetScene()->CreateActor("DatasetGen");
                    tmpa.AddComponent<ECS::Components::DatasetGenComponent>();
                    tmpa.AddComponent<ECS::Components::TransformComponent>();
                    //tmpa.GetTransformComponent()->trans_.SetPosW(scenemanager_.cursor_pos_);
                    scenemanager_.SetSelectedActor(tmpa.GetID());
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("窗口")){
            ImGui::MenuItem("Scene View", NULL, show_scene_view_);
            ImGui::MenuItem("Test View", NULL, show_test_view_);
            ImGui::MenuItem("Camera View", NULL, show_camera_view_);
            ImGui::MenuItem("Hierarchy", NULL, show_hierarchy_);
            ImGui::MenuItem("Inspector", NULL, show_inspector_);
            ImGui::MenuItem("VisLocPanel", NULL, show_vislocpanel_);
            ImGui::MenuItem("DistrotionRectifierPanel", NULL, show_distortionrecifierpanel_);
            ImGui::MenuItem("InfoPanel", NULL, show_infopanel_);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    static int firstfrm=1;
    if (firstfrm){
        firstfrm=0;
    }else{
        if (ResourceManager::PathManager::GetProjectPath()==""){
            ImGui::OpenPopup("ProjectSelection");
            show_project_wnd_=true;
        }else{
            show_project_wnd_=false;
        }
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

    if (ImGui::BeginPopupModal("ProjectSelection", &show_project_wnd_, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Project Selection\n\n");
        
        if (ImGui::Button("Open Project")){
            static std::vector<nfdfilteritem_t> filters{{"Project","sanaprj"}};
            auto pth = Util::NfdDialog::OpenFileDlg(filters);
            if (pth!=""){
                auto path = std::filesystem::path(pth);
                auto papth = path.parent_path();
                ResourceManager::PathManager::SetProjectPath(papth.generic_string()+"/");
                std::ifstream in(std::filesystem::u8path(pth));
                std::string tmp;
                in>>tmp;
                std::cout << tmp << std::endl;
            }
            
        }
        if (ImGui::Button("New Project")){
            static std::vector<nfdfilteritem_t> filters{{"Project folder",""}};
            auto pth = Util::NfdDialog::SaveDlg(filters,"");
            std::cout << pth << std::endl;
            if (pth=="" ||std::filesystem::exists(pth)){
                spdlog::error("[Filesystem] failed");
            }else{
                auto succ = std::filesystem::create_directory(std::filesystem::u8path(pth));
                if(succ){
                    // auto path = std::filesystem::u8path(pth);
                    // auto prjname = path.filename().generic_u8string();

                    auto path = std::filesystem::path(pth);
                    auto prjname = path.filename().generic_string();
                    //std::ofstream fo(pth+"\\"+prjname+".sanaprj");
                    std::ofstream fo(std::filesystem::u8path(pth+"\\"+prjname+".sanaprj"));
                    fo << "hello";
                }
            }
            ResourceManager::PathManager::SetProjectPath(pth+"\\");
            
        }
        
        ImGui::EndPopup();
    }

}



}