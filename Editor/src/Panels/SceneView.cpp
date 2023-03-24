#include "SEditor/Panels/SceneView.h"
#include "ECS/Component/MeshComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Core/GLRenderer.h"
#include "SRender/Passes/SimpleRenderPass.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SAnimation.h"
#include "SRender/Settings/GLSet.h"
#include "SResourceManager/ModelManager.h"
#include "SResourceManager/ShaderManager.h"
#include "SResourceManager/TextureManager.h"
#include "SResourceManager/Util.h"
#include "SWnd/Input/InputManager.h"
#include "SceneSys/SceneManager.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "imgui/imgui.h"
#include <spdlog/spdlog.h>
#include <SRender/Resources/SModel.h>
#include <SRender/Resources/SModelLoader.h>
#include <SEditor/Core/SimpleJoint.h>
#include <iostream>
#include <filesystem>
#include <memory>
#include <stdint.h>
#include <thread>
namespace SEditor::Panels{
SceneView::SceneView(Core::RuntimeContext& rtcontext):
rtcontext_(rtcontext),
shadermanager(SANASERVICE(ResourceManager::ShaderManager)),
texturemanager(SANASERVICE(ResourceManager::TextureManager)),
modelmanager(SANASERVICE(ResourceManager::ModelManager)),

simplerenderpass(rtcontext)
{
    name_="Scene View";
    has_cursor_=true;
    camctrl_.is_fps_cam_mod_=false;
    camctrl_.cam_->Setfovy(90);
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
    
    return;
    //model = modelmanager.CreateResources(R"(..\assets\models\Tile_+025_+034\Tile_+025_+034.obj)");
    //renderpass.render_resources_.push_back(model);
    namespace fs = std::filesystem;
    //fs::path tile_pth(R"(D:\beihang reconstruction data\dxobj)");
    //fs::path tile_pth(R"(E:\user\cnt0\beihang reconstruction data\dxobj)");
    
    fs::path tile_pth(R"(E:\ExperimentsData\Models\GovFacility\Data)");
    int cnt=3;
    int cur=0;
    int st=1;
    int ed=st+cnt-1;
    //simplerenderpass.render_resources_.resize(cnt);
    std::vector<std::thread>threadtest;
    if (std::filesystem::is_directory(tile_pth)){
        for (auto& entry:std::filesystem::directory_iterator(tile_pth)){
            cur++;
            if (cur<st) continue;
            if (entry.is_directory()){
                auto& tmppth = entry.path();
                auto objpth = tmppth/(tmppth.filename().string()+".obj");
                if (fs::exists(objpth)){
                    //spdlog::info(objpth.generic_string());
                    // renderpass.render_resources_.push_back(
                    //     modelmanager.CreateResources(objpth.generic_string()));
                    ///task1(objpth.generic_string(),cur-st);
                    threadtest.emplace_back(&SEditor::Panels::SceneView::task1,this,objpth.generic_string(),cur-st);
                }
            }
            if (cur==ed) break;
        }
        for (auto& t:threadtest){
            t.join();
        }
        modelmanager.UploadAll();
        texturemanager.UploadAll();
    }
}
void SceneView::task1(std::string pth,int idx){
    auto tmpmodel = modelmanager.CreateResources(pth,true);
    //renderpass.render_resources_[idx] = tmpmodel;
    auto& scenemanager = SANASERVICE(SceneSys::SceneManager);
    auto& tmpa=scenemanager.GetScene()->CreateActor();
    
    tmpa.AddComponent<ECS::Components::MeshComponent>(tmpmodel);
    if (tmpmodel->GetMaterial()){
        //add matcomp
    }
    tmpa.AddComponent<ECS::Components::TransformComponent>();
}





void SceneView::LogicTick(float deltat){
   
    
    //rtcontext_.shape_drawer_->SetViewPrj(cam_.GetProjectionMat()*cam_.GetViewMat());
}
void SceneView::RenderTick(float deltat){   
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
    

    fbo_.Bind();
    auto& renderer = *(rtcontext_.core_renderer_);
    auto& shape_drawer = *(rtcontext_.shape_drawer_);
    renderer.SetClearColor(0.2f, 0.2f, 0.2f);
    renderer.ApplyGLstate(SRender::Core::Default_GLstate);
    glStencilMask(0xFF); // stencil mask also influence glclear()
    renderer.ClearBuffer();
    glStencilMask(0x00);
    //ImGui::InputFloat("k:", &k);
    //ImGui::SliderFloat("k",&simplerenderpass.k,-3,3);
    ImGui::SliderFloat("scene-k",&scenerenderpass.k,-3,3);
    //renderer.Draw(*trimeshp,SRender::Setting::SPrimitive::TRIANGLES);
    // for (auto i:model->GetMeshes()){
    //     renderer.Draw(*i, SRender::Setting::SPrimitive::TRIANGLES);
    // }
    //renderpass.Draw();
   
    scenerenderpass.Draw();
    
    shape_drawer.DrawCursor(rtcontext_.scene_manager_->cursor_pos_, {0,1,0.8,1},camctrl_.cam_->GetViewMat());
    
    if (auto actor = rtcontext_.scene_manager_->GetSelectedActor();actor!=nullptr){
        renderer.DrawActorOutline(*actor);
        shape_drawer.DrawTransGizmo(actor->GetTransformComponent()->trans_.world_pos_,camctrl_.cam_->GetViewMat());
    }
   

    ActorPickerTick(deltat);
    //test
    
    //
    
    
    fbo_.Unbind();
}


void SceneView::ActorPickerTick(float deltat){
    actor_picker_fbo_.Resize(canvas_size_.first, canvas_size_.second);
    actor_picker_fbo_.Bind();
    rtcontext_.core_renderer_->SetClearColor(0.0f, 0.0f, 0.0f);

    glStencilMask(0xFF); // stencil mask also influence glclear()
    rtcontext_.core_renderer_->ClearBuffer();
    glStencilMask(0x00);
    actorpickerrenderpass.Draw();

    //input handle
    ImGuiIO& io = ImGui::GetIO();
    static bool mouse_start_in_view=0;
    if (hovered_){
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)){
            mouse_start_in_view=1;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_F)){
            if (auto actor = rtcontext_.scene_manager_->GetSelectedActor()){
                auto meshcomp = (ECS::Components::MeshComponent*)actor->GetComponent("MeshComponent");
                camctrl_.Move2Target(actor->GetTransformComponent()->trans_.world_pos_, meshcomp->GetModel()->GetBoundingSphere()->radius); 
            }
        }
    }
    if (mouse_start_in_view){
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
            mouse_start_in_view=0;
            auto x= static_cast<uint32_t>(io.MousePos.x)-canvas_pos_.first;
            auto y = canvas_size_.second-static_cast<uint32_t>(io.MousePos.y)+canvas_pos_.second;
            if (cursor_selecting){
                float depth=0;
                rtcontext_.core_renderer_->ReadPixels(x,y, 
                 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
                {
                    if (depth==1) {
                        auto tmpcenter = camctrl_.cam_->GetProjectionMat()*camctrl_.cam_->GetViewMat()*glm::vec4(camctrl_.GetCamcenter(),1);
                        depth = tmpcenter.z/tmpcenter.w;
                    }
                    glm::vec4 ndcpos(1);
                    ndcpos.x = 2.0f*x/canvas_size_.first-1.0f;
                    ndcpos.y = 2.0f*y/canvas_size_.second-1.0f;
                    ndcpos.z = 2.0f*depth-1.0f;
                    auto tmp =glm::inverse(camctrl_.cam_->GetProjectionMat()*camctrl_.cam_->GetViewMat())*ndcpos;
                    rtcontext_.scene_manager_->cursor_pos_ = tmp/tmp.w;
                }
            }else{
                uint32_t actorid=0;
                rtcontext_.core_renderer_->ReadPixels(x,y, 
                1, 1, GL_RGB, GL_UNSIGNED_BYTE, &actorid);

                rtcontext_.scene_manager_->SetSelectedActor(actorid);
            }

        }
    }
    actor_picker_fbo_.Unbind();

}
}