#pragma once
#include "SEditor/Core/AssetLoader.h"
#include "SGUI/Core/UImanager.h"
#include "SGUI/Panels/Panel.h"
#include "SceneSys/SceneManager.h"
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>
#include<SEditor/Core/RuntimeContext.h>
namespace SEditor::Panels{

class MainMenubar:public SGUI::Panels::Panel {
public:
    MainMenubar();
    void DrawImpl(float deltat) override;  

private:
    void GetPanelsOpenFlag();
    void OpenScene(const std::string& pth);
    void SaveCurrentScene(const std::string& pth);
    Core::AssetLoader& assetloader_;
    SceneSys::SceneManager& scenemanager_;
    SGUI::Core::UImanager& uimanager_;

    bool* show_scene_view_;
    bool* show_test_view_;
    bool* show_camera_view_;
    bool* show_hierarchy_;
    bool* show_inspector_;
    bool* show_vislocpanel_;
    bool* show_distortionrecifierpanel_;
};

}