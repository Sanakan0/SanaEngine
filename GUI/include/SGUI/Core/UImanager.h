#pragma once
#include <string>
#include <vector>
#include <memory>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "SGUI/Panels/Panel.h"
#include "SWnd/context/context.h"
//#include <SEditor/Panels/SceneView.h>
#include <unordered_map>
namespace SGUI::Core{

struct LevelFonts{
    ImFont* small;
    ImFont* medium;
    ImFont* large;
};

class UImanager{
public:
    UImanager(const SWnd::Context& context,const std::string& version="#version 150");
    ~UImanager();
    
    template<typename T,typename... Args>
    void CreatePanel(const std::string& id,Args&&... args){
        panels_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        //if constexpr (std::is_base_of<SEditor::Panels::SView,T>::value){
        panelmap_.emplace(id,panels_.size()-1);
        //}
        
    }
    template<typename T>
    T& GetPanel(const std::string& id){
        return *static_cast<T*>(panels_[panelmap_[id]].get());
    }
    void SetupIconFont();
    void SetupFont();
    void ApplyStyle();
    void StartAFrame();
    void RenderTick();
    void EnableDocking(bool flg);
    bool dock_state_;
    bool dockspace_on_=true;
    float baseFontSize_=15.0f;
    std::vector <std::unique_ptr<Panels::Panel>> panels_;
    std::unordered_map <std::string,int> panelmap_;
    LevelFonts textfont;
    LevelFonts iconfont;
};

}