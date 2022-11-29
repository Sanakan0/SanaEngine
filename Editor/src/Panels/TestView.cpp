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
    name_="Distortion View";
    Dshaderp = std::unique_ptr<SRender::Resources::GLShader>((SRender::Resources::GLShaderLoader::LoadFromFile( "..\\assets\\shaders\\distortion.glsl")));
    std::vector<SRender::Resources::Vertex> tmpv;
    float sqrt3=sqrt(3);
   
    for (int i=0;i<5;++i){
        for (int j=0;j<=i;++j){
            tmpv.push_back({
            {0.0f-i+j*2,sqrt3*i,0},
            {0,0},
            {0,0,1}
            });
        }
    }
    std::vector<uint32_t> idx;
    int index=0;
    for (int i=0;i<4;++i){
        for (int j=0;j<=i;++j){
            idx.push_back(index);
            idx.push_back(index+i+1);
            idx.push_back(index+i+2);
            ++index;
        }
    }
    
    trimeshp = std::make_unique<SRender::Resources::SMesh>(tmpv,idx);

    SRender::Resources::SModelLoader::LoadSimpleModel("..\\assets\\models\\GUN.fbx", model);
}

TestView::~TestView(){

}




void TestView::LogicTick(float deltat){
   
    
    //rtcontext_.shape_drawer_->SetViewPrj(cam_.GetProjectionMat()*cam_.GetViewMat());
}
void TestView::RenderTick(float deltat){   
    UpdateViewCam(deltat);
    rtcontext_.core_renderer_->SetViewPort(0, 0,canvas_size_.first ,canvas_size_.second );
    static bool rasflag=0;
    if(ImGui::Button("switch ras")){
        rasflag^=1;
    }
    if (rasflag){
        rtcontext_.core_renderer_->SetRasterizationMode(SRender::Setting::SRasterization::LINE);
        rtcontext_.core_renderer_->SetRasterizationLineWdith(5);
    }else{
        rtcontext_.core_renderer_->SetRasterizationMode(SRender::Setting::SRasterization::FILL);
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    fbo_.Bind();
    auto& renderer = *(rtcontext_.core_renderer_);
    auto& shape_drawer = *(rtcontext_.shape_drawer_);
    renderer.SetClearColor(0.2f, 0.2f, 0.2f);
    renderer.ClearBuffer();
    Dshaderp->Bind();
    static float k=0.8;
    //ImGui::InputFloat("k:", &k);
    ImGui::SliderFloat("k:",&k,-3,3);
    Dshaderp->SetUniFloat("k", k);
    Dshaderp->SetUniMat4("ModelMat", glm::mat4(1));
    //renderer.Draw(*trimeshp,SRender::Setting::SPrimitive::TRIANGLES);
    for (auto i:model.GetMeshes()){
        renderer.Draw(*i, SRender::Setting::SPrimitive::TRIANGLES);
    }
    rtcontext_.core_renderer_->SetRasterizationMode(SRender::Setting::SRasterization::FILL);
    //shape_drawer.DrawGrid();
    fbo_.Unbind();
}

}