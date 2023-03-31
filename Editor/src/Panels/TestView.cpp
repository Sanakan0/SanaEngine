#include "SEditor/Panels/TestView.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SAnimation.h"
#include "SRender/Settings/GLSet.h"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"
#include <SRender/Resources/SModel.h>
#include <SRender/Resources/SModelLoader.h>
#include <SEditor/Core/SimpleJoint.h>
#include <iostream>
#include <memory>

namespace SEditor::Panels{

TestView::TestView(Core::RuntimeContext& rtcontext):rtcontext_(rtcontext){
    name_="Test View";
}



void TestView::LogicTick(float deltat){
    

    //rtcontext_.shape_drawer_->SetViewPrj(cam_.GetProjectionMat()*cam_.GetViewMat());
}

void TestView::RenderTick(float deltat){   
    UpdateViewCam(deltat);
    rtcontext_.core_renderer_->SetViewPort(0, 0,canvas_size_.first ,canvas_size_.second );

    fbo_.Bind();
    
    auto& renderer = *(rtcontext_.core_renderer_);
    auto& shape_drawer = *(rtcontext_.shape_drawer_);
    renderer.SetClearColor(0.2f, 0.2f, 0.2f);
    renderer.ApplyGLstate(SRender::Core::Default_GLstate);
    renderer.ClearBuffer();
    
    // shape_drawer.DrawLine({0,0,0}, {5,0,0}, {1,0,0});
    // shape_drawer.DrawLine({0,0,0}, {0,5,0}, {0,1,0});
    // shape_drawer.DrawLine({0,0,0}, {0,0,5}, {0,0,1});
    

    static SimpleJoint sgun;
    static SRender::Resources::SModel model,model2;
    static std::vector<SRender::Resources::SAnimation> animas;
    static int initflag=1;
    if (initflag) {
        SRender::Resources::SModelLoader::LoadModelWithAnima("..\\assets\\models\\GUN.fbx", model, animas),--initflag;
        SRender::Resources::SModelLoader::LoadSimpleModel(R"(../assets/models/adamHead/adamHead.gltf)", model2);
    }
    static std::unique_ptr<SRender::Resources::GLShader> shaderp(SRender::Resources::GLShaderLoader::LoadFromFile( "..\\assets\\shaders\\animation.glsl"));
    
    //sgun.TickStatus(deltat);
    animas[0].Play();
    animas[0].Tick(deltat);
    model.CalcDerivedJoint();
    model.CalcPalette();
    rtcontext_.anima_ssbo_->SendBlocks<glm::mat4>(model.palette_.data(), model.palette_.size()*sizeof(glm::mat4));
    
   
    shaderp->Bind();
    //auto& tmp =model.GetMeshes();
    shaderp->SetUniMat4("ModelMat", model.modelmat_);
    for (auto i:model.GetMeshes()){
        renderer.Draw(*i, SRender::Setting::SPrimitive::TRIANGLES);
    }
    shaderp->Unbind();

    renderer.DrawSkeleton(model);

    shape_drawer.DrawGrid();
    fbo_.Unbind();


}


}
