#pragma once
#include <SRender/Buffers/GLFrameBuffer.h>
#include <string>
#include <SGUI/Panels/Panel.h>
#include <imgui/imgui.h>

namespace SGUI::Panels{

class WndPanel:public SGUI::Panels::Panel {
public:
    WndPanel();
    ~WndPanel();

    void DrawImpl() override;
    void UpdateWndStates();
    std::pair<int,int> GetImguiSize();
    std::pair<int,int> GetImguiPos();
    std::pair<int,int> GetCanvasSize();
    std::pair<int,int> GetCanvasPos();
    SRender::Buffers::GLFrameBuffer fbo_; 
    std::string name_;
    bool resizable_=true;
    bool closable_=true;
    bool movable_=true;
    bool dockable_=true;
    bool scrollable_=true;
    std::pair<int,int> size_={65535,65535};
    std::pair<int,int> pos_;
    std::pair<int,int> canvas_size_={65535,65535};
    std::pair<int,int> canvas_pos_;



    bool opened_=true;
    bool hovered_=true;
    bool focused_=true;

    
};

}