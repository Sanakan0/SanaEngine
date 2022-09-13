#include "SEditor/Panels/SView.h"
#include <iostream>
namespace SEditor::Panels{


SView::SView():camctrl_(*this,SANASERVICE(SWnd::Context),cam_){}

SView::~SView(){}

void SView::FillUBO(){
    auto& editor_ubo = SANASERVICE(SRender::Buffers::GLUniformBuffer);
    auto&[w,h] = canvas_size_;
    
    editor_ubo.BufferSubData(cam_.CalcViewMat(),sizeof(glm::mat4)); //LEAVE space for Model mat
    editor_ubo.BufferSubData(cam_.CalcProjectionMat(w,h),sizeof(glm::mat4)*2);
    editor_ubo.BufferSubData(cam_.campos,sizeof(glm::mat4)*3);
}

void SView::LogicTick(float deltat){
    camctrl_.HandleInputs(deltat);
    auto&[w,h] = canvas_size_;
    fbo_.Resize(w,h);
}

void SView::RenderTick(){
    FillUBO();
    //TODO: setviewport
    glViewport(0,0,canvas_size_.first,canvas_size_.second);
    RenderImpl();
}


}