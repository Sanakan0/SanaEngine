#pragma once
#include "SGUI/Panels/Panel.h"
namespace SEditor::Panels{


class InfoPanel:public SGUI::Panels::Panel {
public:
    void DrawImpl(float deltat) override;

    int corner_ = 1;
    bool open_ = 1;
};



}