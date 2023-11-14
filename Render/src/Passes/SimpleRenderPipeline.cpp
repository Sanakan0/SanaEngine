#include "SRender/Passes/SimpleRenderPipeline.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Core/GLRenderer.h"
#include "SRender/Passes/SceneRenderPass.h"
#include <memory>

namespace SRender{

SimpleRenderPipeline::SimpleRenderPipeline():
renderer_(SANASERVICE(Core::EntityRenderer)){
    scenerenderpass_ = std::make_unique<Passes::SceneRenderPass>();
}

void SimpleRenderPipeline::Init(SEditor::Core::ACamera* acam, Buffers::GLFrameBuffer *fbo,int w,int h,bool enable_distortion){
    acam_=acam;
    fbo_=fbo;
    w_=w;
    h_=h;
    if (enable_distortion){
        scenerenderpass_->BindDistortionInfo(&acam_->cam_->distortion_);
        scenerenderpass_->EnableDistortion();
    }else{
        scenerenderpass_->BindDistortionInfo(nullptr);
        scenerenderpass_->DisableDistortion();
    }
}

void SimpleRenderPipeline::RenderTick(){
    if(acam_==nullptr||fbo_==nullptr){
        return;
    }
    fbo_->Resize(w_, h_);
    fbo_->Bind();
    auto& editor_ubo = SANASERVICE(SRender::Buffers::GLUniformBuffer);
    acam_->CalcAndFillUBO(editor_ubo);
    renderer_.SetViewPort(0, 0, w_,  h_);
    renderer_.SetClearColor(0,0,0);
    renderer_.ApplyGLstate(SRender::Core::Default_GLstate);
    glStencilMask(0xFF); // stencil mask also influence glclear()
    renderer_.ClearBuffer();
    glStencilMask(0x00);

    scenerenderpass_->Draw();
    fbo_->Unbind();
}

}