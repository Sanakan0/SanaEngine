#include "SEditor/Panels/SceneView.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Settings/GLSet.h"
#include <SRender/Resources/SModel.h>
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
    //rtcontext_.shape_drawer_->SetViewPrj(cam_.GetProjectionMat()*cam_.GetViewMat());
}

void SceneView::RenderTick(){   
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    fbo_.Bind();
    auto& renderer = *(rtcontext_.core_renderer_);
    auto& shape_drawer = *(rtcontext_.shape_drawer_);
    renderer.SetClearColor(0.2f, 0.2f, 0.2f);
    renderer.ClearBuffer();
    
    // shape_drawer.DrawLine({0,0,0}, {5,0,0}, {1,0,0});
    // shape_drawer.DrawLine({0,0,0}, {0,5,0}, {0,1,0});
    // shape_drawer.DrawLine({0,0,0}, {0,0,5}, {0,0,1});
   
    
    static SRender::Resources::SModel cube("..\\assets\\models\\GUN.fbx");
    static std::unique_ptr<SRender::Resources::GLShader> shaderp(SRender::Resources::GLShaderLoader::LoadFromFile( "..\\assets\\shaders\\test.glsl"));
    shaderp->Bind();
    auto tmp =cube.GetMeshes();
    for (auto i:cube.GetMeshes()){
        renderer.Draw(*i, SRender::Setting::SPrimitive::TRIANGLES);
    }
    shaderp->Unbind();

    shape_drawer.DrawGrid();
    fbo_.Unbind();

}


}
