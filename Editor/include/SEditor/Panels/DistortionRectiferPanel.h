#pragma once
#include "SGUI/Panels/WndPanel.h"
#include "SRender/Resources/STexture.h"
#include "SceneSys/SceneManager.h"
#include "VisualLoc/RenderBasedLocEngine.h"
#include "VisualLoc/UndistortPipeline.h"
#include "VisualLoc/DistortionRectifier.h"
#include <memory>
#include <opencv2/core/mat.hpp>
namespace SEditor::Panels{

class DistrotionRectifierPanel:public SGUI::Panels::WndPanel {
public:
    DistrotionRectifierPanel();
    void DrawContent() override;  
private:
    VisualLoc::DistortionRectifier rectifier_;
    VisualLoc::UndistortPipeline undistpipeline;

    std::shared_ptr<SRender::Resources::STexture> uimgp;

};

}