#pragma once
#include "ECS/Actor.h"
#include "Eventing/Event.h"
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
    ECS::Actor* selected_actor_=nullptr;
    SCore::Event<> component_drawlist_;
    static const std::string componentlist[];
    
};

}