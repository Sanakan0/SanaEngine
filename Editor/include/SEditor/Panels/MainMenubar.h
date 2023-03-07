#pragma once
#include "SGUI/Panels/Panel.h"
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>
#include<SEditor/Core/RuntimeContext.h>
namespace SEditor::Panels{

class MainMenubar:public SGUI::Panels::Panel {
public:
    void DrawImpl() override;  
};

}