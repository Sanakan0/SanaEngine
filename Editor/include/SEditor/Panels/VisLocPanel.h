#pragma once
#include "SGUI/Panels/WndPanel.h"
#include "SRender/Resources/STexture.h"
#include "VisualLoc/RenderBasedLocEngine.h"
#include <memory>
#include <opencv2/core/mat.hpp>
namespace SEditor::Panels{

class VisLocPanel:public SGUI::Panels::WndPanel {
public:
    VisLocPanel();
    void DrawContent() override;  
private:
    VisualLoc::RenderBasedLocEngine locengine;
    cv::Mat img1,img2;
    std::unique_ptr<SRender::Resources::STexture> imgp1,imgp2;
};

}