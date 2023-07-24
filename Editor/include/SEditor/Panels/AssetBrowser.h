#pragma once
#include "Eventing/Event.h"
#include "SEditor/Panels/SceneView.h"
#include<SEditor/Panels/SView.h>
#include <vector>

namespace SEditor::Panels{



class AssetBrowser:public SGUI::Panels::WndPanel {
public:
    AssetBrowser();
    void DrawContent() override;  
private:
    void PrepareWnd();
    void RightClickMenu();
    
    SceneSys::SceneManager& scenemanager_;
    SceneView& sceneview_;
    SCore::Event<> component_drawlist_;
    static const std::string componentlist[];
    
};

struct FileTreenode{
    std::string name;
    bool is_directory;
    std::vector<FileTreenode>nodes;
}


}