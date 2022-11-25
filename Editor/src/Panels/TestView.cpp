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
    Dshaderp = std::unique_ptr<SRender::Resources::GLShader>((SRender::Resources::GLShaderLoader::LoadFromFile( "..\\assets\\shaders\\distortion.glsl")));
    std::vector<SRender::Resources::Vertex> tmpv;
    float sqrt3=sqrt(3);
    tmpv.push_back({
        {0,0,0},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {1,sqrt3,0},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {-1,sqrt3,0},
        {0,0},
        {0,0,0}
    });
    std::vector<uint32_t> idx{0,1,2};
    trimeshp = std::make_unique<SRender::Resources::SMesh>(tmpv,idx);
}

TestView::~TestView(){

}




void TestView::LogicTick(float deltat){
   
    
    //rtcontext_.shape_drawer_->SetViewPrj(cam_.GetProjectionMat()*cam_.GetViewMat());
}
void TestView::RenderTick(float deltat){   
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
    Dshaderp->Bind();
    Dshaderp->SetUniFloat("k", 0.8);
    Dshaderp->SetUniMat4("ModelMat", glm::mat4(1));
    renderer.Draw(*trimeshp,SRender::Setting::SPrimitive::TRIANGLES);


    shape_drawer.DrawGrid();
    fbo_.Unbind();
}

}