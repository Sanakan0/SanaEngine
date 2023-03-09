#pragma once
#include "SGUI/Panels/WndPanel.h"
#include "SceneSys/SceneManager.h"
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>
#include<SEditor/Core/RuntimeContext.h>
namespace SEditor::Panels{

class Hierarchy:public SGUI::Panels::WndPanel {
public:
    Hierarchy();
    void DrawContent() override;  
private:

    void RightClickMenu();
    SceneSys::SceneManager& scenemanager_;


};

}