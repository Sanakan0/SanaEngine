#include "SEditor/Panels/SView.h"
#include "SEditor/Core/RuntimeContext.h"
#include <iostream>
namespace SEditor::Panels{


SView::SView():camctrl_(*this,SANASERVICE(SWnd::Context),cam_){}

SView::~SView(){}

void SView::FillUBO(){
    auto& editor_ubo = SANASERVICE(SRender::Buffers::GLUniformBuffer);
    editor_ubo.BufferSubData(cam_.GetViewMat(),sizeof(glm::mat4)); //LEAVE space for Model mat
    editor_ubo.BufferSubData(cam_.GetProjectionMat(),sizeof(glm::mat4)*2);
    editor_ubo.BufferSubData(cam_.campos,sizeof(glm::mat4)*3);
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