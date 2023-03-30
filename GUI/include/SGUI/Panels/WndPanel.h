#pragma once
#include <SRender/Buffers/GLFrameBuffer.h>
#include <string>
#include <SGUI/Panels/Panel.h>
#include <imgui/imgui.h>

namespace SGUI::Panels{

class WndPanel:public SGUI::Panels::Panel {
public:
    WndPanel();

    void DrawImpl() override;
    void UpdateWndStates();

    virtual void DrawContent() = 0; //Draw UI on canvas


    std::pair<int,int> GetImguiSize();
    std::pair<int,int> GetImguiPos();
    std::pair<int,int> GetCanvasSize();
    std::pair<int,int> GetCanvasPos();
    //SRender::Buffers::GLFrameBuffer fbo_; 
    std::string name_;
    bool resizable_=true;
    bool closable_=true;
    bool movable_=true;
    bool dockable_=true;
    bool scrollable_=true;
    bool hori_scrollable_=false;
    std::pair<int,int> size_={65535,65535};
    std::pair<int,int> pos_;
    std::pair<int,int> canvas_size_={65535,65535};
    std::pair<int,int> canvas_pos_;
    ImVec2 window_padding_={15,15};


    bool opened_=true;
    bool hovered_=true;
    bool focused_=true;

    
};

}