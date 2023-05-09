#pragma once
#include "SRender/Buffers/GLFrameBuffer.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SceneSys/SceneManager.h"
#include <algorithm>
#include <memory>
#include <stdint.h>

namespace VisualLoc{

class UndistortPipeline{
public:
    UndistortPipeline();
    ~UndistortPipeline() = default;
    void Run(const SRender::Resources::STexture& distimg,float norm_fh,const SRender::LowRenderer::RadialDistortion& distortioninfo);
    uint32_t GetUndistortTexID(){return fbo_.tex_buf_id_;}


private:
    std::unique_ptr<SRender::Resources::SMesh> panelmeshp_=nullptr;
    std::unique_ptr<SRender::Resources::GLShader> shaderp_;
    SRender::Core::EntityRenderer& renderer_;
    std::shared_ptr<SRender::Resources::STexture> dimg;
    SRender::Buffers::GLFrameBuffer fbo_;
    
    float norm_fh_=1; // f/h


};


}