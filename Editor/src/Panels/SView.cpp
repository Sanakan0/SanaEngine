#include "SEditor/Panels/SView.h"
#include "SEditor/Core/RuntimeContext.h"
#include <iostream>
namespace SEditor::Panels{


SView::SView():camctrl_(*this,SANASERVICE(SWnd::Context),cam_){}

SView::~SView(){}

void SView::FillUBO(){
    auto& editor_ubo = SANASERVICE(SRender::Buffers::GLUniformBuffer);
    editor_ubo.BufferSubData(cam_.GetProjectionMat()*cam_.GetViewMat(),0); //viewprj
    editor_ubo.BufferSubData(cam_.campos,sizeof(glm::mat4)*1);
}

void SView::UpdateViewCam(float deltat){
    camctrl_.HandleInputs(deltat);
    auto[w,h] = canvas_size_;
    cam_.CacheViewMat();
    cam_.CacheProjectionMat(w, h);
    fbo_.Resize(w,h);
    FillUBO();
}




}