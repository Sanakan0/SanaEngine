#include "SEditor/Panels/SceneView.h"
#include "ECS/Actor.h"
#include "ECS/Component/CameraComponent.h"
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
#include "SceneSys/Scene.h"
#include "SceneSys/SceneManager.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
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
actorpickerrenderpass(camctrl_),
imgprj_depth_fbo_(0,0,1)
{
    name_="场景视图";
    has_cursor_=true;
    camctrl_.is_fps_cam_mod_=false;
    camctrl_.cam_->Setfovy(50);
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
    
    tmpa.AddComponent<ECS::Components::MeshComponent>().SetModel(tmpmodel);
    if (tmpmodel->GetMaterial()){
        //add matcomp
    }
    //tmpa.AddComponent<ECS::Components::TransformComponent>();
}


void SceneView::SetEngineUboControlMask(uint32_t mask){
    static auto& editor_ubo = SANASERVICE(SRender::Buffers::GLUniformBuffer);
    editor_ubo.BufferSubData(mask,sizeof(glm::mat4)*4+sizeof(glm::vec4));//set ubo ctl mask
}


void SceneView::LogicTick(float deltat){
    rtcontext_.scene_manager_->GetScene()->Update(deltat);
    
    //rtcontext_.shape_drawer_->SetViewPrj(cam_.GetProjectionMat()*cam_.GetViewMat());
}
void SceneView::RenderTick(float deltat){   
    UpdateViewCam(deltat);
    rtcontext_.core_renderer_->SetViewPort(0, 0,canvas_size_.first ,canvas_size_.second );
    // static bool rasflag=0;
    // if(ImGui::Button("switch ras")){
    //     rasflag^=1;
    // }
    if (enable_line){
        rtcontext_.core_renderer_->SetRasterizationMode(SRender::Setting::SRasterization::LINE);
        rtcontext_.core_renderer_->SetRasterizationLineWdith(1);
    }else if (enable_point){
        rtcontext_.core_renderer_->SetRasterizationMode(SRender::Setting::SRasterization::POINT);
        rtcontext_.core_renderer_->SetRasterizationPointSize(1);
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

    if (rtcontext_.scene_manager_->enable_img_prj_){
        // auto& editor_ubo = SANASERVICE(SRender::Buffers::GLUniformBuffer);
        // editor_ubo.BufferSubData((int)1,sizeof(glm::mat4)*4+sizeof(glm::vec4));//set ubo ctl flag
        SetEngineUboControlMask(1);

        auto w =rtcontext_.scene_manager_->img_tex_->width;
        auto h =rtcontext_.scene_manager_->img_tex_->height;
        imgprj_depth_fbo_.Resize(w,h);
        imgprj_depth_fbo_.Bind();
        
        glStencilMask(0xFF); // stencil mask also influence glclear()
        renderer.ClearBuffer();
        glStencilMask(0x00);
        rtcontext_.core_renderer_->SetViewPort(0, 0,w ,h );
        imgprjdepthmaprenderpass.Draw();
        imgprj_depth_fbo_.BindDepth(5);
        rtcontext_.scene_manager_->img_tex_->Bind(6);
        fbo_.Bind();
        rtcontext_.core_renderer_->SetViewPort(0, 0,canvas_size_.first ,canvas_size_.second );



        auto prjcam = rtcontext_.scene_manager_->GetActiveCamera();
        if (prjcam){
            auto& prjcamtrans= prjcam->GetTransformComponent().trans_;
            auto camcomp = static_cast<ECS::Components::CameraComponent*>(
                prjcam->GetComponent("CameraComponent"));
            
            

            auto hheight = camcomp->cam_.far_ * camcomp->cam_.Getsensor_size_h()/camcomp->cam_.Getfocal_length() * 0.5;
            auto wwidth =  hheight*camcomp->cam_.Getaspect_ratio();
            glm::vec3 tmp(0,0,-camcomp->cam_.far_);
            static int mvx[]={1,-1,1,-1};
            static int mvy[]={1,1,-1,-1};
            for (int i=0;i<4;++i){
                tmp.x=wwidth*mvx[i];
                tmp.y=hheight*mvy[i];
                auto delta = prjcamtrans.GetOrienW()*tmp;
                auto p2 = prjcamtrans.GetPosW()+delta;
                renderer.GetShapeDrawer()->DrawLine(prjcamtrans.GetPosW(),p2, {0,1,0});
            }
        }
        

    }else{
        imgprj_depth_fbo_.Resize(0, 0); //release when dont use
    }

    if (enable_normal) scenerenderpass.EnableNormal();
    else scenerenderpass.DisableNormal();
    scenerenderpass.Draw();

    SetEngineUboControlMask(0);
    
    ActorPickerTick(deltat);

    fbo_.Bind();
    shape_drawer.DrawCursor(rtcontext_.scene_manager_->cursor_pos_, {0,1,0.8,1},camctrl_.cam_->GetViewMat());
    
    if (auto actor = rtcontext_.scene_manager_->GetSelectedActor();actor!=nullptr){
        renderer.DrawActorOutline(*actor);
        shape_drawer.DrawTransGizmo(actor->GetTransformComponent().trans_.GetPosW(),camctrl_.cam_->GetViewMat());
    }
   
    

    rtcontext_.core_renderer_->SetRasterizationMode(SRender::Setting::SRasterization::FILL);

    rtcontext_.scene_manager_->GetScene()->DrawGizmo(deltat);

    for (auto& i:ctlpts){
        shape_drawer.DrawPoint(i, {1,0.4,0,1});
    }
    fbo_.Unbind();
    
}

void SceneView::HandleGizmoPick(float deltat){
    static ECS::ActorID tmpid;
    static glm::mat4 prjviewmat,inv_prjviewmat;
    static glm::vec3 disvec,positivevec,prjpoint,campos;
    static float dist;
    static glm::vec4 ndcv1,ndcv0;
    static glm::vec2 tmpn;
    static glm::vec2 cursor_delta,start_cursor;
    if (gizpickhandler_init_flag_) { //data initialization
        tmpid = selectedgizmoid_-SceneSys::Actor_ID_Max-1;
        auto gizvec = glm::vec3(0);
        gizvec[tmpid] =1;
        campos = camctrl_.extrinsic_->GetPosW();
        auto gizpos = rtcontext_.scene_manager_->GetSelectedActor()->GetTransformComponent().trans_.GetPosW();
        prjpoint = gizpos + gizvec * glm::dot(campos-gizpos, gizvec);
        disvec = prjpoint-campos;
        dist = glm::length(disvec);
        positivevec = glm::normalize( glm::cross(disvec,prjpoint+gizvec-campos));
        prjviewmat=camctrl_.cam_->GetProjectionMat()*camctrl_.cam_->GetViewMat();
        inv_prjviewmat=glm::inverse(prjviewmat);

        ndcv0 = prjviewmat*glm::vec4(gizpos,1);
        ndcv1 =prjviewmat*glm::vec4(gizpos+gizvec,1);
        ndcv0/=ndcv0.w;
        ndcv1/=ndcv1.w;
        tmpn = glm::normalize( glm::vec2(ndcv1)-glm::vec2(ndcv0));

        cursor_delta=glm::vec2(0,0);
        gizpickhandler_init_flag_=0;
    }
    auto[dx,dy] = rtcontext_.input_manager_->GetCursorDelta();
    cursor_delta.x+=2.0*dx/canvas_size_.first;
    cursor_delta.y-=2.0*dy/canvas_size_.second;


    glm::vec2 cursorpos;
    glm::vec2 cur_ndcpos2 = glm::vec2(ndcv0) + glm::dot(cursor_delta,tmpn)*tmpn;

    glm::vec4 cur_ndcpos = glm::vec4(cur_ndcpos2,0,1); //input
    
    auto tmpworld =  inv_prjviewmat*cur_ndcpos;//
    tmpworld/=tmpworld.w;
    glm::vec3 tmpvec = glm::normalize(glm::vec3(tmpworld)-campos);//
    
    auto theta = acos(glm::dot(disvec,tmpvec)/dist); //
   
    int sign = glm::dot(positivevec, glm::normalize( glm::cross(disvec,tmpvec)))>0?1:-1;//
    float delta = tan(theta)*dist*sign;//

    float res = prjpoint[tmpid]+delta;

    auto& actortrans = rtcontext_.scene_manager_->GetSelectedActor()->GetTransformComponent().trans_;
    auto tmppos = actortrans.GetPosW();
    tmppos[tmpid]=res;
    actortrans.SetPosW(tmppos);

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
        if ((!mouse_start_in_view)&&ImGui::IsMouseDown(ImGuiMouseButton_Left)){
            mouse_start_in_view=1;
            if (!cursor_selecting){
                uint32_t actorid=0;
                auto x= static_cast<uint32_t>(io.MousePos.x)-canvas_pos_.first;
                auto y = canvas_size_.second-static_cast<uint32_t>(io.MousePos.y)+canvas_pos_.second-1;
                rtcontext_.core_renderer_->ReadPixels(x,y, 
                        1, 1, GL_RGB, GL_UNSIGNED_BYTE, &actorid);
                if (actorid>SceneSys::Actor_ID_Max){
                        //Gizmo selected
                    selectedgizmoid_ = actorid;
                    gizpickhandler_init_flag_=1;
                }
            }
            
        }
        if (ImGui::IsKeyPressed(ImGuiKey_F)){
            if (auto actor = rtcontext_.scene_manager_->GetSelectedActor()){
                auto meshcomp = (ECS::Components::MeshComponent*)actor->GetComponent("MeshComponent");
                camctrl_.Move2Target(actor->GetTransformComponent().trans_.GetPosW(), meshcomp->GetModel()->GetBoundingSphere()->radius); 
            }
        }
    }
    if (mouse_start_in_view){
        auto x= static_cast<uint32_t>(io.MousePos.x)-canvas_pos_.first;
        auto y = canvas_size_.second-static_cast<uint32_t>(io.MousePos.y)+canvas_pos_.second-1;
        
        if(selectedgizmoid_){
            HandleGizmoPick(deltat);
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
            mouse_start_in_view=0;
            
            if (cursor_selecting){
                float depth=0;
                fbo_.Bind();
                rtcontext_.core_renderer_->ReadPixels(x,y, 
                 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
                actor_picker_fbo_.Bind();
                {
                    if (depth==1) {
                        auto tmpcenter = camctrl_.cam_->GetProjectionMat()*camctrl_.cam_->GetViewMat()*glm::vec4(camctrl_.GetCamcenter(),1);
                        depth = tmpcenter.z/tmpcenter.w;
                    }
                    glm::vec4 ndcpos(1);
                    ndcpos.x = 2.0f*x/canvas_size_.first+1.0f/canvas_size_.first-1.0f;
                    ndcpos.y = 2.0f*y/canvas_size_.second+1.0f/canvas_size_.second-1.0f;
                    ndcpos.z = 2.0f*depth-1.0f;
                    auto tmp =glm::inverse(camctrl_.cam_->GetProjectionMat()*camctrl_.cam_->GetViewMat())*ndcpos;
                    rtcontext_.scene_manager_->cursor_pos_ = tmp/tmp.w;
                }
            }else if(selectedgizmoid_){
                selectedgizmoid_ = 0;
            }
            else{
                uint32_t actorid=0;
                rtcontext_.core_renderer_->ReadPixels(x,y, 
                   1, 1, GL_RGB, GL_UNSIGNED_BYTE, &actorid);
                if (actorid>SceneSys::Actor_ID_Max){
                    //Gizmo selected

                }else{
                    rtcontext_.scene_manager_->SetSelectedActor(actorid);
                }
                
            }

        }
    }
    actor_picker_fbo_.Unbind();

}

void SceneView::UpdateEngineLights(SceneSys::Scene& scene){
    auto& lightcomps= scene.GetBasicRenderComponent().lightcomps;

}

}