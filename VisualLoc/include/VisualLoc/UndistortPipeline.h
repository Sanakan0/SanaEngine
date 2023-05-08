#pragma once
#include "SRender/Buffers/GLFrameBuffer.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SceneSys/SceneManager.h"
#include <algorithm>
#include <memory>

namespace VisualLoc{

class UndistortPipeline{
public:
    UndistortPipeline();
    ~UndistortPipeline() = default;
    void Run();



private:
    std::unique_ptr<SRender::Resources::GLShader> shaderp_;
    SRender::Core::EntityRenderer& renderer_;
    std::shared_ptr<SRender::Resources::STexture> dimg;
    SRender::Buffers::GLFrameBuffer fbo_;
    float aspec_ratio_;
    float norm_fh_; // f/h


};


}