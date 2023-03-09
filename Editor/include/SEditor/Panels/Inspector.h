#pragma once
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>
#include<SEditor/Core/RuntimeContext.h>
namespace SEditor::Panels{

class Inspector:public SGUI::Panels::WndPanel {
public:
    Inspector();
    void DrawContent() override;  
private:
    void PrepareWnd();
    void RightClickMenu();
    SceneSys::SceneManager& scenemanager_;


};

}