#pragma once
#include <string>
#include <vector>
#include <memory>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "SGUI/Panels/Panel.h"
#include <SEditor/Panels/SceneView.h>
#include <unordered_map>
namespace SGUI::Core{

class UImanager{
public:
    UImanager(GLFWwindow* glfw_wndp,const std::string& version="#version 150");
    ~UImanager();
    
    template<typename T,typename... Args>
    void CreatePanel(const std::string& id,Args&&... args){
        panels_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        if constexpr (std::is_base_of<SEditor::Panels::SView,T>::value){
            panelmap_.emplace(id,panels_.size()-1);
        }
        
    }
    template<typename T>
    T& GetPanel(const std::string& id){
        return *static_cast<T*>(panels_[panelmap_[id]].get());
    }
    void ApplyStyle();
    void RenderTick();
    void EnableDocking(bool flg);
    bool dock_state_;
    bool dockspace_on_=true;
    std::vector <std::unique_ptr<Panels::Panel>> panels_;
    std::unordered_map <std::string,int> panelmap_;
};

}