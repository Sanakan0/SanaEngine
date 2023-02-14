#include "SEditor/Panels/TestView.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SAnimation.h"
#include "SRender/Settings/GLSet.h"
#include "SResourceManager/ModelManager.h"
#include "SResourceManager/ShaderManager.h"
#include "SResourceManager/TextureManager.h"
#include "SResourceManager/Util.h"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"
#include <spdlog/spdlog.h>
#include <SRender/Resources/SModel.h>
#include <SRender/Resources/SModelLoader.h>
#include <SEditor/Core/SimpleJoint.h>
#include <iostream>
#include <filesystem>
#include <memory>
#include <thread>
namespace SEditor::Panels{
TestView::TestView(Core::RuntimeContext& rtcontext):
rtcontext_(rtcontext),
shadermanager(SANASERVICE(ResourceManager::ShaderManager)),
texturemanager(SANASERVICE(ResourceManager::TextureManager)),
modelmanager(SANASERVICE(ResourceManager::ModelManager))
{
    name_="Distortion View";
    std::vector<SRender::Resources::Vertex> tmpv;
    float sqrt3=sqrtf(3);
   
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
    //model = modelmanager.CreateResources(R"(..\assets\models\Tile_+025_+034\Tile_+025_+034.obj)");
    //renderpass.render_resources_.push_back(model);
    namespace fs = std::filesystem;
    fs::path tile_pth(R"(F:\SUFFER\BUAA\beihang reconstruction data\dxobj)");
    //fs::path tile_pth(R"(E:\ExperimentsData\Models\GovFacility\Data)");
    int cnt=0;
    std::vector<std::thread>threadtest;
    if (std::filesystem::is_directory(tile_pth)){
        for (auto& entry:std::filesystem::directory_iterator(tile_pth)){
            cnt++;
            if (cnt<100) continue;
            if (entry.is_directory()){
                auto& tmppth = entry.path();
                auto objpth = tmppth/(tmppth.filename().string()+".obj");
                if (fs::exists(objpth)){
                    //spdlog::info(objpth.generic_string());
                    // renderpass.render_resources_.push_back(
                    //     modelmanager.CreateResources(objpth.generic_string()));
                    task1(objpth.generic_string());
                    //threadtest.emplace_back(&SEditor::Panels::TestView::task1,this,objpth.generic_string());
                }
            }
            if (cnt==200) break;
        }
        for (auto& t:threadtest){
            t.join();
        }
    }
}
void TestView::task1(std::string pth){
    renderpass.render_resources_.push_back(
                        modelmanager.CreateResources(pth));
}

TestView::~TestView(){
    //shadermanager.ClearAll();
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
        rtcontext_.core_renderer_->SetRasterizationLineWdith(1);
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
    //ImGui::InputFloat("k:", &k);
    ImGui::SliderFloat("k",&renderpass.k,-3,3);

    //renderer.Draw(*trimeshp,SRender::Setting::SPrimitive::TRIANGLES);
    // for (auto i:model->GetMeshes()){
    //     renderer.Draw(*i, SRender::Setting::SPrimitive::TRIANGLES);
    // }
    renderpass.Draw();
    rtcontext_.core_renderer_->SetRasterizationMode(SRender::Setting::SRasterization::FILL);
    shape_drawer.DrawGrid();
    fbo_.Unbind();
}

}