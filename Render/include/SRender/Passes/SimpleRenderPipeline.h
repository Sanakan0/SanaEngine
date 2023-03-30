#pragma once
#include "SEditor/Core/CameraCtrl.h"
#include "SRender/Buffers/GLFrameBuffer.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Passes/SceneRenderPass.h"
#include <algorithm>
#include <memory>


namespace SEditor::Core{class RuntimeContext;}

namespace SRender{

class SimpleRenderPipeline{
public:
    SimpleRenderPipeline();
    ~SimpleRenderPipeline()=default;
    void Init( SEditor::Core::ACamera* acam,Buffers::GLFrameBuffer* fbo,int w,int h);
    void RenderTick();
    float k=0;
private:
    SEditor::Core::ACamera* acam_;
    Buffers::GLFrameBuffer* fbo_;
    int w_,h_;
    Core::EntityRenderer& renderer_;
    std::unique_ptr<Passes::SceneRenderPass> scenerenderpass_;
};


}