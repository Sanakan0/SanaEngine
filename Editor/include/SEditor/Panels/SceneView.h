#pragma once
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>

namespace SEditor::Panels{

class SceneView:public SView{
public:
    SceneView();
    ~SceneView();
    virtual void LogicTick(float deltat) override;
    virtual void RenderImpl() override;
    
    
};

}