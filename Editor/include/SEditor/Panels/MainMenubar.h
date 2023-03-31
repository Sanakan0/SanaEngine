#pragma once
#include "SEditor/Core/AssetLoader.h"
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
    Core::AssetLoader& assetloader_;
    SceneSys::SceneManager& scenemanager_;
};

}