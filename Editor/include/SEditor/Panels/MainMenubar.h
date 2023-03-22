#pragma once
#include "SEditor/Core/AssetLoader.h"
#include "SGUI/Panels/Panel.h"
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>
#include<SEditor/Core/RuntimeContext.h>
namespace SEditor::Panels{

class MainMenubar:public SGUI::Panels::Panel {
public:
    MainMenubar();
    void DrawImpl() override;  

private:
    Core::AssetLoader& assetloader_;
};

}