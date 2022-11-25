#include "SEditor/Panels/SceneView.h"
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

SceneView::SceneView(Core::RuntimeContext& rtcontext):rtcontext_(rtcontext){
    name_="Scene View";
}

SceneView::~SceneView(){

}

void SceneView::LogicTick(float deltat){
    

    //rtcontext_.shape_drawer_->SetViewPrj(cam_.GetProjectionMat()*cam_.GetViewMat());
}

void SceneView::RenderTick(float deltat){   
    UpdateViewCam(deltat);
    rtcontext_.core_renderer_->SetViewPort(0, 0,canvas_size_.first ,canvas_size_.second );

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
    

    static SimpleJoint sgun;
    static SRender::Resources::SModel model;
    static std::vector<SRender::Resources::SAnimation> animas;
    static int initflag=1;
    if (initflag) SRender::Resources::SModelLoader::LoadModelWithAnima("..\\assets\\models\\Capoeira.fbx", model, animas),--initflag;
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
    glDisable(GL_DEPTH_TEST);
    renderer.DrawSkeleton(model);
    glEnable(GL_DEPTH_TEST);
    shape_drawer.DrawGrid();
    fbo_.Unbind();
    float tmp = 1.0f/deltat;
    ImGui::InputFloat("fps:",&tmp);
    //imgui debug
    // ImGui::Text("hello");
    // char out[20];
    // sprintf(out, "t %f", deltat);
    // ImGui::Text(out);
    // ImGui::Button("shoot")?sgun.Shoot():0;
    //

}


}
