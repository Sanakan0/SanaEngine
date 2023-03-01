#include "SEditor/Panels/SView.h"
#include "SEditor/Core/RuntimeContext.h"
#include <iostream>
namespace SEditor::Panels{


SView::SView():camctrl_(*this,SANASERVICE(SWnd::Context),cam_){}



void SView::FillUBO(){
    auto& editor_ubo = SANASERVICE(SRender::Buffers::GLUniformBuffer);
    editor_ubo.BufferSubData(cam_.GetViewMat(),0);
    editor_ubo.BufferSubData(cam_.GetProjectionMat(),sizeof(glm::mat4)*1);
    editor_ubo.BufferSubData(cam_.GetProjectionMat()*cam_.GetViewMat(),sizeof(glm::mat4)*2); //viewprj
    editor_ubo.BufferSubData(camctrl_.GetPos(),sizeof(glm::mat4)*3);
}

void SView::UpdateViewCam(float deltat){
    camctrl_.HandleInputs(deltat);
    auto[w,h] = canvas_size_;
    cam_.CacheViewMat(camctrl_.GetPos(),camctrl_.GetOrien());
    cam_.CacheProjectionMat(w, h);
    fbo_.Resize(w,h);
    FillUBO();
}




}