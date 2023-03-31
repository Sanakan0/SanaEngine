#pragma once
#include "SGUI/Panels/Panel.h"
namespace SEditor::Panels{


class InfoPanel:public SGUI::Panels::Panel {
public:
    void DrawImpl(float deltat) override;
private:
    int corner_ = 0;
    bool open_ = 1;
};



}