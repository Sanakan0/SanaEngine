#include "SEditor/Panels/SceneView.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include <iostream>
#include <memory>

namespace SEditor::Panels{

SceneView::SceneView(Core::RuntimeContext& rtcontext):rtcontext_(rtcontext){
    name_="Scene View";
}

SceneView::~SceneView(){

}

void SceneView::LogicTick(float deltat){
    UpdateViewCam(deltat);
    rtcontext_.core_renderer_->SetViewPort(0, 0,canvas_size_.first ,canvas_size_.second );
    rtcontext_.shape_drawer_->SetViewPrj(cam_.GetProjectionMat()*cam_.GetViewMat());
}

void SceneView::RenderTick(){
    fbo_.Bind();
    auto& renderer = *(rtcontext_.core_renderer_);
    auto& shape_drawer = *(rtcontext_.shape_drawer_);
    renderer.SetClearColor(0.2f, 0.3f, 0.3f);
    renderer.ClearBuffer();
    
    shape_drawer.DrawLine({0,0,0}, {1,0,0}, {1,0,0});
    shape_drawer.DrawLine({0,0,0}, {0,1,0}, {0,1,0});
    shape_drawer.DrawLine({0,0,0}, {0,0,1}, {0,0,1});
    

    fbo_.Unbind();

}


}
